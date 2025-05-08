/* BerryPHP/berryrt/http.c */
/* HTTP/1.1 and HTTP/2 server implementation. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <uv.h>
#include <http_parser.h>
#include <nghttp2/nghttp2.h>

typedef struct {
    uv_tcp_t handle;
    zval callback;
    int use_tls;
} http_server_t;

/* HTTP parser callback */
static int on_message_complete(http_parser *parser) {
    http_server_t *server = parser->data;
    zval params[3];
    ZVAL_LONG(&params[0], parser->data); // Client FD
    ZVAL_STRING(&params[1], http_method_str(parser->method));
    ZVAL_STRING(&params[2], parser->data); // Path
    zval result;
    zend_call_function(&server->callback, NULL, &result, 3, params);
    zval_ptr_dtor(&result);
    return 0;
}

/* Create HTTP server */
PHP_FUNCTION(berry_http_create) {
    zval *loop_ptr, *callback;
    long port;
    zend_bool use_tls;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zlzb", &loop_ptr, &port, &callback, &use_tls) == FAILURE) {
        return;
    }

    uv_loop_t *loop = (uv_loop_t *)Z_PTR_P(loop_ptr);
    http_server_t *server = emalloc(sizeof(http_server_t));
    server->use_tls = use_tls;
    ZVAL_COPY(&server->callback, callback);

    uv_tcp_init(loop, &server->handle);
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);
    uv_tcp_bind(&server->handle, (const struct sockaddr *)&addr, 0);

    uv_listen((uv_stream_t *)&server->handle, 128, NULL);
    RETURN_TRUE;
}