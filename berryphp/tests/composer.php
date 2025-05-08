<?php
// BerryPHP/berryphp/tests/composer.php
// Tests Composer integration with Monolog.
// Author: Rikaz Zarook, Witsberry

require_once __DIR__ . '/../vendor/autoload.php';
require_once __DIR__ . '/../src/App.php';

use BerryPHP\App;

$app = new App();

$app->get('/', function ($req, $res) use ($app) {
    $app->log('Handling request to /', 'info');
    $res->view('welcome', ['name' => 'User']);
});

$app->listen(3000, function () {
    echo "BerryPHP running on port 3000 with Composer support\n";
});