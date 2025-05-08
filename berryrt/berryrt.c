/* BerryPHP/berryrt/berryrt.c */
/* Core PHP extension for BerryPHP, managing event loop and module entry. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <uv.h>
#include <zend_fiber.h>
#include "http.h"
#include "ws.h"
#include "sse.h"
#include "graphql.h"
#include "async.h"
#include "db.h"
#include "redis.h"

/* Global event loop */
static uv_loop_t *global_loop;

/* Initialize event loop */
PHP_FUNCTION(berry_loop_create) {
    global_loop = emalloc(sizeof(uv_loop_t));
    uv_loop_init(global_loop);
    RETURN_PTR(global_loop);
}

/* Run event loop */
PHP_FUNCTION(berry_loop_run) {
    zval *loop_ptr;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &loop_ptr) == FAILURE) {
        return;
    }
    uv_loop_t *loop = (uv_loop_t *)Z_PTR_P(loop_ptr);
    uv_run(loop, UV_RUN_DEFAULT);
}

/* JIT optimization (basic opcache config) */
PHP_FUNCTION(berry_jit_optimize) {
    zval *loop_ptr;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "z", &loop_ptr) == FAILURE) {
        return;
    }
    /* Enable JIT in opcache */
    zend_string *ini = zend_string_init("opcache.jit_buffer_size", 21, 0);
    zend_alter_ini_entry_chars(ini, "100M", 4, PHP_INI_SYSTEM, PHP_INI_STAGE_RUNTIME);
    zend_string_release(ini);
    RETURN_TRUE;
}

/* Function declarations */
static const zend_function_entry berryrt_functions[] = {
    PHP_FE(berry_loop_create, NULL)
    PHP_FE(berry_loop_run, NULL)
    PHP_FE(berry_jit_optimize, NULL)
    PHP_FE(berry_http_create, NULL)
    PHP_FE(berry_cluster_fork, NULL)
    PHP_FE(berry_sse_init, NULL)
    PHP_FE(berry_ws_init, NULL)
    PHP_FE(berry_async_file_read, NULL)
    PHP_FE(berry_db_query, NULL)
    PHP_FE(berry_redis_get, NULL)
    PHP_FE(berry_redis_set, NULL)
    PHP_FE(berry_graphql_parse, NULL)
    PHP_FE_END
};

/* Module entry */
zend_module_entry berryrt_module_entry = {
    STANDARD_MODULE_HEADER,
    "berryrt",
    berryrt_functions,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    "1.0",
    STANDARD_MODULE_PROPERTIES
};

ZEND_GET_MODULE(berryrt)