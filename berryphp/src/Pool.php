<?php
// BerryPHP/berryphp/src/Pool.php
// Connection pooling for database and Redis.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Pool {
    private $loop;
    private $connections = [];
    private $maxConnections = 10;

    public function __construct($loop) {
        $this->loop = $loop;
    }

    public function get($type) {
        if (count($this->connections[$type] ?? []) > 0) {
            return array_pop($this->connections[$type]);
        }
        
        if ($type === 'mysql') {
            $conn = new \mysqli('localhost', 'user', 'password', 'database');
            if ($conn->connect_error) {
                throw new \Exception("Connection failed: " . $conn->connect_error);
            }
            return $conn;
        } elseif ($type === 'redis') {
            $conn = new \Redis();
            $conn->connect('127.0.0.1', 6379);
            return $conn;
        }
        
        throw new \Exception("Unsupported connection type: $type");
    }

    public function release($type, $conn) {
        if (count($this->connections[$type] ?? []) < $this->maxConnections) {
            $this->connections[$type][] = $conn;
        } else {
            if ($type === 'mysql') {
                $conn->close();
            } elseif ($type === 'redis') {
                $conn->close();
            }
        }
    }
}