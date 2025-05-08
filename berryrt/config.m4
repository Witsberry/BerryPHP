dnl BerryPHP/berryrt/config.m4
dnl Build configuration for berryrt with PHP 8.4.
dnl Author: Rikaz Zarook, Witsberry

PHP_ARG_ENABLE(berryrt, whether to enable berryrt support,
[  --enable-berryrt           Enable berryrt support])

if test "$PHP_BERRYRT" != "no"; then
    dnl Check for PHP 8.4 headers
    PHP_CHECK_HEADER(zend_fiber.h, [zend_fiber.h not found], [
        AC_MSG_ERROR([PHP 8.4 with zend_fiber.h is required])
    ])
    PHP_ADD_LIBRARY_WITH_PATH(uv, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(http_parser, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(nghttp2, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(websockets, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(mysqlclient, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_ADD_LIBRARY_WITH_PATH(hiredis, /usr/lib, BERRYRT_SHARED_LIBADD)
    PHP_SUBST(BERRYRT_SHARED_LIBADD)
    PHP_NEW_EXTENSION(berryrt, berryrt.c http.c ws.c sse.c graphql.c async.c db.c redis.c, $ext_shared)
fi