<?php
// BerryPHP/berryphp/src/BerryView.php
// Blade-like templating engine.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class BerryView {
    private $view;
    private $data;
    private $i18n;

    public function __construct($view, $data = []) {
        $this->view = $view;
        $this->data = $data;
        $this->i18n = new I18n();
    }

    public function render() {
        $file = __DIR__ . "/../views/{$this->view}.berry";
        if (!file_exists($file)) {
            throw new \Exception("View {$this->view} not found");
        }

        $content = file_get_contents($file);
        $content = preg_replace_callback('/\{\{\s*\$([a-zA-Z_][a-zA-Z0-9_]*)\s*\}\}/', function ($matches) {
            return $this->data[$matches[1]] ?? '';
        }, $content);
        $content = preg_replace_callback('/\{\{\s*trans\(\'([a-zA-Z_][a-zA-Z0-9_]*)\'\)\s*\}\}/', function ($matches) {
            return $this->i18n->t($matches[1], $this->data);
        }, $content);
        return $content;
    }
}