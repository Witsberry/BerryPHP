<?php
// BerryPHP/berryphp/src/Metrics.php
// Performance metrics.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Metrics {
    private $loop;

    public function __construct($loop) {
        $this->loop = $loop;
    }

    public function counter($name, $value, $labels = []) {
        Async::run($this->loop, function ($async) use ($name, $value, $labels) {
            $labelStr = json_encode($labels);
            file_put_contents('metrics.log', "counter:$name:$value:$labelStr\n", FILE_APPEND);
        })->await();
    }

    public function gauge($name, $value, $labels = []) {
        Async::run($this->loop, function ($async) use ($name, $value, $labels) {
            $labelStr = json_encode($labels);
            file_put_contents('metrics.log', "gauge:$name:$value:$labelStr\n", FILE_APPEND);
        })->await();
    }

    public function trace($name, callable $callback) {
        $start = microtime(true);
        $result = $callback();
        $duration = (microtime(true) - $start) * 1000;
        $this->gauge("trace_$name", $duration);
        return $result;
    }
}