<?php
// BerryPHP/berryphp/src/Logger.php
// Logging utility.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class Logger {
    private $loop;
    private $file;

    public function __construct($loop, $file) {
        $this->loop = $loop;
        $this->file = $file;
    }

    public function info($message) {
        Async::run($this->loop, function ($async) use ($message) {
            file_put_contents($this->file, "[INFO] $message\n", FILE_APPEND);
        })->await();
    }
}