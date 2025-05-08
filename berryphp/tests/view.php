<?php
// BerryPHP/berryphp/tests/view.php
// Tests BerryView templating.
// Author: Rikaz Zarook, Witsberry

require_once __DIR__ . '/../vendor/autoload.php';
require_once __DIR__ . '/../src/App.php';

use BerryPHP\App;

$app = new App();

$app->get('/', function ($req, $res) {
    $res->view('welcome', ['name' => 'Developer']);
});

$app->listen(3000, function () {
    echo "BerryPHP running on port 3000 with BerryView\n";
});