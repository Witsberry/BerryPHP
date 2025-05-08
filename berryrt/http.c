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

/* Client connection data */
typedef struct {
    uv_tcp_t *client;
    http_parser parser;
    zval *callback;
    char *path;
} http_client_t;

/* HTTP parser callbacks */
static int on_url(http_parser *parser, const char *at, size_t length) {
    http_client_t *client = parser->data;
    client->path = estrndup(at, length);
    return 0;
}

static int on_message_complete(http_parser *parser) {
    http_client_t *client = parser->data;
    zval params[3];
    ZVAL_LONG(¶ms[0], (zend_long)client->client);
    ZVAL_STRING(¶ms[1], http_method_str(parser->method));
    ZVAL_STRING(¶ms[2], client->path);
    zval result;
    zend_call_function(client->callback, NULL, &result, 3, params);
    zval_ptr_dtor(&result);
    efree(client->path);
    return 0;
}

/* Client connection callback */
static void on_connect(uv_stream_t *server, int status) {
    if (status < 0) return;
    
    http_server_t *http_server = server->data;
    http_client_t *client = emalloc(sizeof(http_client_t));
    client->client = emalloc(sizeof(uv_tcp_t));
    client->callback = &http_server->callback;
    uv_tcp_init(server->loop, client->client);
    uv_accept(server, (uv_stream_t *)client->client);
    
    http_parser_settings settings = {0};
    settings.on_url = on_url;
    settings.on_message_complete = on_message_complete;
    http_parser_init(&client->parser, HTTP_REQUEST);
    client->parser.data = client;
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
    server->handle.data = server;
    struct sockaddr_in addr;
    uv_ip4_addr("0.0.0.0", port, &addr);
    uv_tcp_bind(&server->handle, (const struct sockaddr *)&addr, 0);

    uv_listen((uv_stream_t *)&server->handle, 128, on_connect);
    RETURN_TRUE;
}