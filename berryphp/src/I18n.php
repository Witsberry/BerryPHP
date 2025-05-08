<?php
// BerryPHP/berryphp/src/I18n.php
// Internationalization support.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class I18n {
    private $locale = 'en';
    private $translations = [];

    public function setLocale($locale) {
        $this->locale = $locale;
        $file = __DIR__ . "/../lang/$locale.json";
        if (file_exists($file)) {
            $this->translations = json_decode(file_get_contents($file), true);
        }
    }

    public function t($key, $params = []) {
        $translation = $this->translations[$key] ?? $key;
        foreach ($params as $k => $v) {
            $translation = str_replace(":$k", $v, $translation);
        }
        return $translation;
    }
}