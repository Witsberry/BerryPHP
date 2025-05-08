/* BerryPHP/berryrt/async.c */
/* Async I/O operations using Fibers. */
/* Author: Rikaz Zarook, Witsberry */

#include <php.h>
#include <uv.h>
#include <zend_fiber.h>

typedef struct {
    uv_fs_t req;
    zval callback;
    char *buffer;
} async_file_t;

/* File read callback */
static void async_file_read_cb(uv_fs_t *req) {
    async_file_t *file = req->data;
    zval params[1];
    ZVAL_STRINGL(&params[0], file->buffer, req->result);
    zval result;
    zend_call_function(&file->callback, NULL, &result, 1, params);
    zval_ptr_dtor(&result);
    efree(file->buffer);
    efree(file);
    uv_fs_req_cleanup(req);
}

/* Async file read */
PHP_FUNCTION(berry_async_file_read) {
    zval *loop_ptr, *callback;
    char *path;
    size_t path_len;
    if (zend_parse_parameters(ZEND_NUM_ARGS(), "zsz", &loop_ptr, &path, &path_len, &callback) == FAILURE) {
        return;
    }

    uv_loop_t *loop = (uv_loop_t *)Z_PTR_P(loop_ptr);
    async_file_t *file = emalloc(sizeof(async_file_t));
    ZVAL_COPY(&file->callback, callback);
    file->buffer = emalloc(8192);
    file->req.data = file;
    uv_fs_read(loop, &file->req, uv_fs_open(loop, &file->req, path, O_RDONLY, 0, NULL), file->buffer, 8192, -1, async_file_read_cb);
    RETURN_TRUE;
}