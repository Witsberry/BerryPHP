/* BerryPHP/berryrt/db.c */
/* Async database queries for MySQL/PostgreSQL. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <mysql.h>

typedef struct {
    MYSQL *conn;
    zval callback;
} db_query_t;

/* Query callback */
static void db_query_cb(uv_async_t *handle) {
    db_query_t *query = handle->data;
    MYSQL_RES *res = mysql_store_result(query->conn);
    zval rows;
    array_init(&rows);
    MYSQL_ROW row;
    while ((row = mysql_fetch_row(res))) {
        zval r;
        array_init(&r);
        for (int i = 0; i < mysql_num_fields(res); i++) {
            add_next_index_string(&r, row[i] ? row[i] : "");
        }
        add_next_index_zval(&rows, &r);
    }
    zval params[1];
    ZVAL_ZVAL(&params[0], &rows, 1, 0);
    zval result;
    zend_call_function(&query->callback, NULL, &result, 1, params);
    zval_ptr_dtor(&result);
    mysql_free_result(res);
    efree(query);
}

/* Async database query */
PHP_FUNCTION(berry_db_query) {
    zval *loop_ptr, *callback;
    char *query;
    size_t query_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zsz", &loop_ptr, &query, &query_len, &callback) == FAILURE) {
        return;
    }

    MYSQL *conn = mysql_init(NULL);
    mysql_real_connect(conn, "localhost", "user", "password", "database", 0, NULL, 0);
    db_query_t *q = emalloc(sizeof(db_query_t));
    q->conn = conn;
    ZVAL_COPY(&q->callback, callback);
    mysql_query(conn, query);
    uv_async_t async;
    async.data = q;
    uv_async_init((uv_loop_t *)Z_PTR_P(loop_ptr), &async, db_query_cb);
    uv_async_send(&async);
    RETURN_TRUE;
}