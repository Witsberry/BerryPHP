<?php
// BerryPHP/berryphp/src/App.php
// Core application class for routing and server setup.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

if (file_exists(__DIR__ . '/../../vendor/autoload.php')) {
    require_once __DIR__ . '/../../vendor/autoload.php';
}

require_once __DIR__ . '/Request.php';
require_once __DIR__ . '/Response.php';
require_once __DIR__ . '/Async.php';
require_once __DIR__ . '/Logger.php';
require_once __DIR__ . '/Security.php';
require_once __DIR__ . '/Metrics.php';
require_once __DIR__ . '/Cache.php';
require_once __DIR__ . '/Auth.php';
require_once __DIR__ . '/I18n.php';
require_once __DIR__ . '/BerryView.php';
require_once __DIR__ . '/GraphQL.php';

use Monolog\Logger;
use Monolog\Handler\StreamHandler;

class App {
    private $loop;
    private $routes = [];
    private $middleware = [];
    private $logger;
    private $metrics;
    private $cache;
    private $auth;
    private $i18n;
    private $monolog;

    public function __construct() {
        $this->loop = berry_loop_create();
        berry_jit_optimize($this->loop);
        $this->logger = new Logger($this->loop, 'app.log');
        $this->metrics = new Metrics($this->loop);
        $this->cache = new Cache($this->loop);
        $this->auth = new Auth($this->loop, $this->cache);
        $this->i18n = new I18n();
        $this->monolog = new Logger('berryphp');
        $this->monolog->pushHandler(new StreamHandler('app_monolog.log', Logger::INFO));
    }

    private function pathToRegex($path) {
        $pattern = preg_replace('#:([\w]+)#', '([^/]+)', $path);
        return "#^" . $pattern . "$#";
    }

    public function use(callable $middleware) {
        $this->middleware[] = $middleware;
    }

    public function get($path, callable $callback) {
        $this->routes['GET'][$path] = [
            'callback' => $callback,
            'pattern' => $this->pathToRegex($path)
        ];
    }

    public function post($path, callable $callback) {
        $this->routes['POST'][$path] = [
            'callback' => $callback,
            'pattern' => $this->pathToRegex($path)
        ];
    }

    public function ws($path, callable $callback) {
        $this->routes['WS'][$path] = [
            'callback' => $callback,
            'pattern' => $this->pathToRegex($path)
        ];
    }

    public function graphql($path, $schema) {
        $this->routes['POST'][$path] = [
            'callback' => function ($req, $res) use ($schema) {
                $body = json_decode(file_get_contents('php://input'), true);
                $query = $body['query'] ?? '';
                $variables = $body['variables'] ?? [];
                $graphql = new GraphQL($this->loop, $schema);
                $result = $graphql->handle($query, $variables)->await();
                $res->json($result);
            },
            'pattern' => $this->pathToRegex($path)
        ];
    }

    public function sse($path, callable $callback) {
        $this->routes['GET'][$path] = [
            'callback' => function ($req, $res) use ($callback) {
                berry_sse_init($this->loop, $res->client_fd, function ($client_fd) use ($callback, $req, $res) {
                    $callback($req, $res);
                });
            },
            'pattern' => $this->pathToRegex($path)
        ];
    }

    private function matchRoute($method, $path) {
        if (!isset($this->routes[$method])) {
            return null;
        }
        
        foreach ($this->routes[$method] as $routePath => $route) {
            if (preg_match($route['pattern'], $path, $matches)) {
                $params = [];
                if (preg_match_all('#:([\w]+)#', $routePath, $paramNames)) {
                    array_shift($matches);
                    foreach ($paramNames[1] as $i => $name) {
                        $params[$name] = $matches[$i] ?? null;
                    }
                }
                return [
                    'callback' => $route['callback'],
                    'params' => $params
                ];
            }
        }
        return null;
    }

    public function log($message, $level = 'info') {
        $this->monolog->$level($message);
    }

    public function listen($port, $callback = null, $workers = 0, $use_tls = false) {
        $this->use(function ($req, $res, $next) {
            $this->metrics->trace('request', function () use ($req, $res, $next) {
                $start = microtime(true);
                $locale = $req->headers['Accept-Language'] ?? 'en';
                $this->i18n->setLocale(substr($locale, 0, 2));
                $this->logger->info("Request: {$req->method} {$req->path}");
                $this->log("Request: {$req->method} {$req->path}", 'info');
                Security::rateLimit($req, $res, function () use ($req, $res, $next, $start) {
                    $this->metrics->counter('http_requests_total', 1, ['method' => $req->method, 'path' => $req->path]);
                    $next();
                    $this->metrics->gauge('http_request_duration_ms', (microtime(true) - $start) * 1000, ['method' => $req->method, 'path' => $req->path]);
                });
            });
        });

        if ($workers > 0) {
            berry_cluster_fork($this->loop, $port, function ($client_fd, $method, $path) {
                $req = new Request($method, $path);
                $res = new Response($client_fd);
                
                $index = 0;
                $next = function () use (&$index, $req, $res, &$next) {
                    if ($index < count($this->middleware)) {
                        $middleware = $this->middleware[$index++];
                        $middleware($req, $res, $next);
                    } else {
                        $route = $this->matchRoute($req->method, $req->path);
                        if ($route) {
                            $req->params = (object)$route['params'];
                            $route['callback']($req, $res);
                        } else {
                            $res->status(404)->send("Not Found");
                        }
                    }
                };
                $next();
            }, $workers, $use_tls);
        } else {
            berry_http_create($this->loop, $port, function ($client_fd, $method, $path) {
                $req = new Request($method, $path);
                $res = new Response($client_fd);
                
                $index = 0;
                $next = function () use (&$index, $req, $res, &$next) {
                    if ($index < count($this->middleware)) {
                        $middleware = $this->middleware[$index++];
                        $middleware($req, $res, $next);
                    } else {
                        $route = $this->matchRoute($req->method, $req->path);
                        if ($route) {
                            $req->params = (object)$route['params'];
                            $route['callback']($req, $res);
                        } else {
                            $res->status(404)->send("Not Found");
                        }
                    }
                };
                $next();
            }, $use_tls);
            
            if ($callback) {
                $callback();
            }
            
            berry_loop_run($this->loop);
        }
    }
}