/* BerryPHP/berryrt/graphql.c */
/* GraphQL query parsing. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <graphql-c_parser/parser.h>

typedef struct {
    graphql_ast_t *ast;
    zval callback;
} graphql_query_t;

/* Parse GraphQL query */
PHP_FUNCTION(berry_graphql_parse) {
    zval *loop_ptr;
    char *query;
    size_t query_len;
    zval *callback;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zsz", &loop_ptr, &query, &query_len, &callback) == FAILURE) {
        return;
    }

    graphql_query_t *q = emalloc(sizeof(graphql_query_t));
    ZVAL_COPY(&q->callback, callback);
    q->ast = graphql_parse_string(query, NULL);
    if (!q->ast) {
        php_error_docref(NULL, E_WARNING, "Failed to parse GraphQL query");
        efree(q);
        RETURN_FALSE;
    }

    zval result;
    ZVAL_STRING(&result, "parsed");
    zend_call_function(&q->callback, NULL, &result, 1, &result);
    zval_ptr_dtor(&result);
    graphql_ast_free(q->ast);
    efree(q);
    RETURN_TRUE;
}