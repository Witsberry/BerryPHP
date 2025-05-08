<?php
// BerryPHP/berryphp/src/Auth.php
// Authentication and RBAC.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Auth {
    private $loop;
    private $cache;

    public function __construct($loop, Cache $cache) {
        $this->loop = $loop;
        $this->cache = $cache;
    }

    public function authenticate($req, $res, callable $next) {
        $token = $req->headers['Authorization'] ?? '';
        if (!$token) {
            $res->status(401)->send("Unauthorized");
            return;
        }
        
        $user = $this->cache->get("session:$token");
        if (!$user) {
            $res->status(401)->send("Invalid token");
            return;
        }
        
        $req->user = json_decode($user);
        $next();
    }

    public function role($role) {
        return function ($req, $res, $next) use ($role) {
            if (!isset($req->user->role) || $req->user->role !== $role) {
                $res->status(403)->send("Forbidden");
                return;
            }
            $next();
        };
    }
}