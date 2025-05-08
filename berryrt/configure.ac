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
    AC_MSG_ERROR([phpize not found, please install PHP development tools])
fi

# Checks for libraries
AC_CHECK_LIB([uv], [uv_loop_init], [], [AC_MSG_ERROR([libuv not found])])
AC_CHECK_LIB([http_parser], [http_parser_init], [], [AC_MSG_ERROR([http-parser not found])])
AC_CHECK_LIB([nghttp2], [nghttp2_session_server_new], [], [AC_MSG_ERROR([nghttp2 not found])])
AC_CHECK_LIB([websockets], [lws_create_context], [], [AC_MSG_ERROR([libwebsockets not found])])
AC_CHECK_LIB([mysqlclient], [mysql_init], [], [AC_MSG_ERROR([mysqlclient not found])])
AC_CHECK_LIB([hiredis], [redisConnect], [], [AC_MSG_ERROR([hiredis not found])])

# Checks for header files
AC_CHECK_HEADERS([zend_fiber.h], [], [AC_MSG_ERROR([zend_fiber.h not found, PHP 8.4 required])])

# Generate files
AC_CONFIG_FILES([Makefile])
AC_OUTPUT