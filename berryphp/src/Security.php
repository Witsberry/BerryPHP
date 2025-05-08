<?php
// BerryPHP/berryphp/src/Security.php
// Rate limiting and security utilities.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Security {
    public static function rateLimit($req, $res, callable $next) {
        $ip = $_SERVER['REMOTE_ADDR'];
        $key = "rate_limit:$ip";
        $cache = new Cache(null);
        $count = (int)$cache->get($key) ?: 0;
        
        if ($count >= 100) {
            $res->status(429)->send("Too Many Requests");
            return;
        }
        
        $cache->set($key, $count + 1, 60);
        $next();
    }
}