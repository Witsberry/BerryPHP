# BerryPHP/berryrt/configure.ac
# Autoconf configuration for berryrt with PHP 8.4.
# Author: Rikaz Zarook, Witsberry

AC_INIT([berryrt], [1.0], [rikaz@witsberry.com])
AC_CONFIG_SRCDIR([berryrt.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs
AC_PROG_CC
AC_PROG_CXX
LT_INIT

# Checks for PHP
AC_CHECK_PROG(PHPIZE, phpize, phpize)
if test -z "$PHPIZE"; then
    AC_MSG_ERROR([phpize not found, please install PHP 8.4 development tools])
fi

# Find PHP include path using php-config
AC_CHECK_PROG(PHP_CONFIG, php-config, php-config)
if test -z "$PHP_CONFIG"; then
    AC_MSG_ERROR([php-config not found, please install php8.4-dev])
fi

PHP_INCLUDE_PATH=`$PHP_CONFIG --includes`
if test -z "$PHP_INCLUDE_PATH"; then
    AC_MSG_ERROR([Could not determine PHP include path])
fi

# Checks for zend_fiber.h with fallback paths
SAVE_CFLAGS="$CFLAGS"
CFLAGS="$CFLAGS $PHP_INCLUDE_PATH -I/usr/include/php/20240829 -I/usr/include/php/20240829/Zend -I/usr/include/php/20240924 -I/usr/include/php/20240924/Zend"
AC_CHECK_FILE([/usr/include/php/20240829/Zend/zend_fiber.h], [FOUND_FIBER_H=yes], [
    AC_CHECK_FILE([/usr/include/php/20240924/Zend/zend_fiber.h], [FOUND_FIBER_H=yes], [
        AC_MSG_ERROR([zend_fiber.h not found in $PHP_INCLUDE_PATH, /usr/include/php/20240829/Zend, or /usr/include/php/20240924/Zend; ensure php8.4-dev is installed])
    ])
])
AC_CHECK_HEADER([Zend/zend_fiber.h], [], [
    AC_MSG_ERROR([zend_fiber.h header check failed, ensure php8.4-dev is installed])
])
CFLAGS="$SAVE_CFLAGS"

# Checks for libraries
AC_CHECK_LIB([uv], [uv_loop_init], [], [AC_MSG_ERROR([libuv not found])])
AC_CHECK_LIB([http_parser], [http_parser_init], [], [AC_MSG_ERROR([http-parser not found])])
AC_CHECK_LIB([nghttp2], [nghttp2_session_server_new], [], [AC_MSG_ERROR([nghttp2 not found])])
AC_CHECK_LIB([websockets], [lws_create_context], [], [AC_MSG_ERROR([libwebsockets not found])])
AC_CHECK_LIB([mysqlclient], [mysql_init], [], [AC_MSG_ERROR([mysqlclient not found])])
AC_CHECK_LIB([hiredis], [redisConnect], [], [AC_MSG_ERROR([hiredis not found])])

# Generate files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT