/* BerryPHP/berryrt/sse.c */
/* Server-Sent Events implementation. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <uv.h>

typedef struct {
    uv_timer_t timer;
    zval callback;
    long client_fd;
} sse_client_t;

static void sse_timer_cb(uv_timer_t *handle) {
    sse_client_t *client = handle->data;
    zval params[1];
    ZVAL_LONG(¶ms[0], client->client_fd);
    zval result;
    zend_call_function(&client->callback, NULL, &result, 1, params);
    zval_ptr_dtor(&result);
}

PHP_FUNCTION(berry_sse_init) {
    zval *loop_ptr, *callback;
    long client_fd;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zllz", &loop_ptr, &client_fd, &callback) == FAILURE) {
        return;
    }

    uv_loop_t *loop = (uv_loop_t *)Z_PTR_P(loop_ptr);
    sse_client_t *client = emalloc(sizeof(sse_client_t));
    client->client_fd = client_fd;
    ZVAL_COPY(&client->callback, callback);
    uv_timer_init(loop, &client->timer);
    client->timer.data = client;
    uv_timer_start(&client->timer, sse_timer_cb, 1000, 1000);
    RETURN_TRUE;
}