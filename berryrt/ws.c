/* BerryPHP/berryrt/ws.c */
/* WebSocket server implementation. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <uv.h>
#include <libwebsockets.h>

typedef struct {
    lws_context *context;
    zval callback;
} ws_server_t;

static int ws_callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len) {
    ws_server_t *server = (ws_server_t *)user;
    if (reason == LWS_CALLBACK_RECEIVE) {
        zval params[2];
        ZVAL_LONG(¶ms[0], (zend_long)wsi);
        ZVAL_STRINGL(¶ms[1], in, len);
        zval result;
        zend_call_function(&server->callback, NULL, &result, 2, params);
        zval_ptr_dtor(&result);
    }
    return 0;
}

PHP_FUNCTION(berry_ws_init) {
    zval *loop_ptr, *callback;
    long port;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zllz", &loop_ptr, &port, &callback) == FAILURE) {
        return;
    }

    ws_server_t *server = emalloc(sizeof(ws_server_t));
    ZVAL_COPY(&server->callback, callback);
    struct lws_context_creation_info info = {0};
    info.port = port;
    info.protocols = (struct lws_protocols[]){{"ws", ws_callback, 0, 0}, {NULL, NULL, 0, 0}};
    info.user = server;
    server->context = lws_create_context(&info);
    RETURN_TRUE;
}