<?php
// BerryPHP/berryphp/src/GraphQL.php
// GraphQL query handler.
// Author: Rikaz Zarook, Witsberry

namespace BerryPHP;

class GraphQL {
    private $loop;
    private $schema;

    public function __construct($loop, $schema) {
        $this->loop = $loop;
        $this->schema = $schema;
    }

    public function handle($query, $variables) {
        return Async::run($this->loop, function ($async) use ($query, $variables) {
            return berry_graphql_parse($query, function ($result) use ($variables) {
                return ['data' => ['parsed' => $result]];
            });
        });
    }
}