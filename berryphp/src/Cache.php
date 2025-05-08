<?php
// BerryPHP/berryphp/src/Cache.php
// Redis caching.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Cache {
    private $loop;

    public function __construct($loop) {
        $this->loop = $loop;
    }

    public function get($key) {
        return Async::run($this->loop, function ($async) use ($key) {
            return berry_redis_get($key, function ($value) {
                return $value;
            });
        })->await();
    }

    public function set($key, $value, $ttl) {
        Async::run($this->loop, function ($async) use ($key, $value, $ttl) {
            berry_redis_set($key, $value, $ttl, function () {});
        })->await();
    }
}