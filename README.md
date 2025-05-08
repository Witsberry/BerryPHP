# BerryPHP Framework

BerryPHP is a high-performance, event-driven PHP framework for modern web applications and APIs. Created by **Rikaz Zarook**, founder of **Witsberry**.

## Features
- **Async I/O**: Non-blocking operations using PHP Fibers and berryrt.
- **Protocols**: HTTP/1.1, HTTP/2, WebSocket, SSE, GraphQL.
- **BerryView**: Blade-like templating engine.
- **Built-in**: Auth, RBAC, i18n, rate limiting, metrics, logging, clustering, serverless.
- **Composer**: Supports Packagist packages (e.g., `monolog/monolog`).
- **Performance**: ~60,000–80,000 req/s, ~160MB for 4 workers.

## Installation
```bash
git clone https://github.com/witsberry/BerryPHP
cd BerryPHP
./build.sh
cd berryphp
composer install
```

## Getting Started
```php
<?php
require_once 'vendor/autoload.php';
require_once 'berryphp/src/App.php';

use BerryPHP\App;

$app = new App();

$app->get('/', fn($req, $res) => $res->view('welcome', ['name' => 'User']));

$app->listen(3000, fn() => echo "Running on port 3000\n");
```

# Using Composer
```bash
berryphp add composer:monolog/monolog
# OR
composer require monolog/monolog
```

# Using BerryView
In views/welcome.berry:

```html
<h1>Welcome, {{ $name }}!</h1>
<p>{{ trans('welcome') }}</p>
```

Render:
```php
$app->get('/', fn($req, $res) => $res->view('welcome', ['name' => 'User']));
```

# Documentation
See docs/.

# Community
Forum

GitHub

Packages

# License
MIT License (see LICENSE)


**BerryPHP/CHANGELOG.md**
```markdown
# Changelog

## 1.0.0 - 2025-05-08
- Initial release by Rikaz Zarook, Witsberry.
- Features:
  - HTTP/1.1, HTTP/2, WebSocket, SSE, GraphQL.
  - Async I/O for files, networks, MySQL, PostgreSQL, Redis.
  - BerryView templating.
  - Auth, RBAC, i18n, rate limiting, metrics, logging.
  - Composer support.
  - Clustering, load balancing, serverless.
  - Package manager and registry.
  - Documentation and CI/CD.




