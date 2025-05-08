# BerryPHP Framework 1.0

BerryPHP is a high-performance, event-driven PHP framework by **Rikaz Zarook**, **Witsberry**.

## Features
- HTTP/1.1, HTTP/2, WebSocket, SSE, GraphQL
- Async I/O with PHP 8.4 Fibers for database, file, network
- BerryView templating
- Redis caching, MySQL/PostgreSQL
- Clustering, load balancing
- Auth, RBAC, i18n
- Composer support

## Installation
```bash
git clone https://github.com/witsberry/BerryPHP
cd BerryPHP
./build.sh
cd berryphp
composer install

Getting Started
php

<?php
require_once 'vendor/autoload.php';
require_once 'berryphp/src/App.php';

use BerryPHP\App;

$app = new App();

$app->get('/', fn($req, $res) => $res->view('welcome', ['name' => 'User']));

$app->listen(3000, fn() => echo "Running on port 3000\n");

Composer
bash

berryphp add composer:monolog/monolog

Example:
php

use Monolog\Logger;
use Monolog\Handler\StreamHandler;

$app->get('/log', function ($req, $res) {
    $logger = new Logger('example');
    $logger->pushHandler(new StreamHandler('example.log', Logger::INFO));
    $logger->info('Request received');
    $res->view('welcome', ['name' => 'Logger']);
});

BerryView
In views/welcome.berry:
html

<h1>Welcome, {{ $name }}!</h1>
<p>{{ trans('welcome') }}</p>

Render:
php

$app->get('/', fn($req, $res) => $res->view('welcome', ['name' => 'User']));

Community
Forum

GitHub

Packages

**BerryPHP/docs/tutorial.md**
```markdown
# BerryPHP Tutorial

Build a simple BerryPHP app with BerryView and Composer.

## Setup
```bash
git clone https://github.com/witsberry/BerryPHP
cd BerryPHP
./build.sh
cd berryphp
composer install

Create View
Edit views/welcome.berry:
html

<h1>Hello, {{ $name }}!</h1>
<p>{{ trans('welcome') }}</p>

Create Routes
Create app.php:
php

<?php
require_once 'vendor/autoload.php';
require_once 'src/App.php';

use BerryPHP\App;

$app = new App();

$app->get('/', fn($req, $res) => $res->view('welcome', ['name' => 'BerryPHP User']));

$app->listen(3000, fn() => echo "Running on port 3000\n");

Run
bash

../run.sh app.php

Visit http://localhost:3000.
Add Composer Package
bash

composer require guzzlehttp/guzzle

Use Guzzle:
php

$app->get('/api', function ($req, $res) {
    $client = new \GuzzleHttp\Client();
    $response = \BerryPHP\Async::run($app->loop, fn() => $client->get('https://api.example.com'))->await();
    $res->json(['data' => $response->getBody()->getContents()]);
});

Contribute
Explore src/.

Contribute: https://github.com/witsberry/BerryPHP

#### **website Directory**

**BerryPHP/website/index.html**
```html
<!DOCTYPE html>
<html>
<head>
    <title>BerryPHP - High-Performance PHP Framework</title>
</head>
<body>
    <h1>BerryPHP</h1>
    <p>A high-performance PHP framework by Rikaz Zarook, Witsberry.</p>
    <ul>
        <li><a href="/docs">Documentation</a></li>
        <li><a href="/forum.php">Forum</a></li>
        <li><a href="https://github.com/witsberry/BerryPHP">GitHub</a></li>
    </ul>
</body>
</html>

