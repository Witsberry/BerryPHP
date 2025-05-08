<?php
// BerryPHP/berryphp/src/Request.php
// Handles HTTP request parsing.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Request {
    public $method;
    public $path;
    public $headers = [];
    public $params;
    public $query;
    public $body;

    public function __construct($method, $path) {
        $this->method = $method;
        $this->path = $path;
        $this->query = $_GET;
        $this->body = json_decode(file_get_contents('php://input'), true) ?? [];
        $this->headers = getallheaders();
    }
}