<?php
// BerryPHP/berryphp/src/Response.php
// Handles HTTP response creation.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Response {
    public $client_fd;
    private $status = 200;
    private $headers = [];

    public function __construct($client_fd) {
        $this->client_fd = $client_fd;
        $this->headers['Content-Type'] = 'text/plain';
    }

    public function status($code) {
        $this->status = $code;
        return $this;
    }

    public function header($key, $value) {
        $this->headers[$key] = $value;
        return $this;
    }

    public function send($body) {
        header("HTTP/1.1 {$this->status}");
        foreach ($this->headers as $key => $value) {
            header("$key: $value");
        }
        echo $body;
    }

    public function json($data) {
        $this->header('Content-Type', 'application/json');
        $this->send(json_encode($data));
    }

    public function view($view, $data = []) {
        $this->header('Content-Type', 'text/html');
        $view = new BerryView($view, $data);
        $this->send($view->render());
    }
}