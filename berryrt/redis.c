/* BerryPHP/berryrt/redis.c */
/* Async Redis operations. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <hiredis/hiredis.h>
#include <hiredis/async.h>
#include <zend_fiber.h>

typedef struct {
    redisAsyncContext *context;
    zval callback;
    zend_fiber_t *fiber;
} redis_op_t;

/* Redis get callback */
static void redis_get_cb(redisAsyncContext *c, void *r, void *privdata) {
    redis_op_t *op = privdata;
    redisReply *reply = r;
    zval params[1];
    if (reply && reply->type == REDIS_REPLY_STRING) {
        ZVAL_STRINGL(¶ms[0], reply->str, reply->len);
    } else {
        ZVAL_NULL(¶ms[0]);
    }
    zval result;
    zend_call_function(&op->callback, NULL, &result, 1, params);
    zval_ptr_dtor(&result);
    if (op->fiber) {
        zend_fiber_resume(op->fiber, NULL, 0);
    }
    efree(op);
}

/* Async Redis get */
PHP_FUNCTION(berry_redis_get) {
    zval *loop_ptr, *callback;
    char *key;
    size_t key_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zsz", &loop_ptr, &key, &key_len, &callback) == FAILURE) {
        return;
    }

    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    redis_op_t *op = emalloc(sizeof(redis_op_t));
    op->context = c;
    ZVAL_COPY(&op->callback, callback);
    op->fiber = zend_fiber_current();
    redisAsyncCommand(c, redis_get_cb, op, "GET %s", key);
    if (op->fiber) {
        zend_fiber_suspend();
    }
    RETURN_TRUE;
}

/* Async Redis set */
PHP_FUNCTION(berry_redis_set) {
    zval *loop_ptr, *callback;
    char *key, *value;
    size_t key_len, value_len;
    long ttl;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zsszl", &loop_ptr, &key, &key_len, &value, &value_len, &ttl, &callback) == FAILURE) {
        return;
    }

    redisAsyncContext *c = redisAsyncConnect("127.0.0.1", 6379);
    redis_op_t *op = emalloc(sizeof(redis_op_t));
    op->context = c;
    ZVAL_COPY(&op->callback, callback);
    op->fiber = zend_fiber_current();
    redisAsyncCommand(c, NULL, op, "SET %s %s EX %d", key, value, ttl);
    if (op->fiber) {
        zend_fiber_suspend();
    }
    efree(op);
    RETURN_TRUE;
}