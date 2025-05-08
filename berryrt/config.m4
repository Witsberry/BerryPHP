dnl BerryPHP/berryrt/config.m4
dnl Build configuration for berryrt with PHP 8.4.
dnl Author: Rikaz Zarook, Witsberry

PHP_ARG_ENABLE(berryrt, whether to enable berryrt support,
[  --enable-berryrt           Enable berryrt support])

if test "$PHP_BERRYRT" != "no"; then
    dnl Get PHP include path
    PHP_EVAL_INCLINE($($PHP_CONFIG --includes))

    dnl Check for PHP 8.4 headers
    PHP_CHECK_HEADER([Zend/zend_fiber.h], [], [
        dnl Fallback check for common paths
        if test -f "/usr/include/php/20240829/Zend/zend_fiber.h" || test -f "/usr/include/php/20240924/Zend/zend_fiber.h"; then
            PHP_EVAL_INCLINE(-I/usr/include/php/20240829 -I/usr/include/php/20240829/Zend -I/usr/include/php/20240924 -I/usr/include/php/20240924/Zend)
        else
            AC_MSG_ERROR([zend_fiber.h not found in $($PHP_CONFIG --includes) or common paths, ensure php8.4-dev is installed])
        fi
    ])

    dnl Check for libraries
    PHP_CHECK_LIBRARY(uv, uv_loop_init,
        [PHP_ADD_LIBRARY_WITH_PATH(uv, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([libuv not found])], [-L/usr/lib])
    PHP_CHECK_LIBRARY(http_parser, http_parser_init,
        [PHP_ADD_LIBRARY_WITH_PATH(http_parser, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([http-parser not found])], [-L/usr/lib])
    PHP_CHECK_LIBRARY(nghttp2, nghttp2_session_server_new,
        [PHP_ADD_LIBRARY_WITH_PATH(nghttp2, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([nghttp2 not found])], [-L/usr/lib])
    PHP_CHECK_LIBRARY(websockets, lws_create_context,
        [PHP_ADD_LIBRARY_WITH_PATH(websockets, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([libwebsockets not found])], [-L/usr/lib])
    PHP_CHECK_LIBRARY(mysqlclient, mysql_init,
        [PHP_ADD_LIBRARY_WITH_PATH(mysqlclient, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([mysqlclient not found])], [-L/usr/lib])
    PHP_CHECK_LIBRARY(hiredis, redisConnect,
        [PHP_ADD_LIBRARY_WITH_PATH(hiredis, /usr/lib, BERRYRT_SHARED_LIBADD)],
        [AC_MSG_ERROR([hiredis not found])], [-L/usr/lib])

    PHP