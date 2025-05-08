<?php
// BerryPHP/berryphp/src/Async.php
// Async I/O utilities using Fibers.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Async {
    private $loop;
    private $fiber;

    public function __construct($loop) {
        $this->loop = $loop;
    }

    public static function run($loop, callable $callback) {
        $async = new self($loop);
        $async->fiber = new Fiber(function () use ($async, $callback) {
            $result = $callback($async);
            Fiber::suspend($result);
        });
        return $async;
    }

    public function await() {
        return $this->fiber->start() ?? $this->fiber->resume();
    }
}