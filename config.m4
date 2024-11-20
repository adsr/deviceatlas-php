PHP_ARG_WITH([deviceatlas],
  [for deviceatlas support],
  [AS_HELP_STRING([--with-deviceatlas],
    [Include deviceatlas support])])

AS_VAR_IF([PHP_DEVICEATLAS], [no],, [
  SEARCH_PATH="/usr/local /usr"
  SEARCH_FOR="/include/dac.h"
  AS_IF([test -r $PHP_DEVICEATLAS/$SEARCH_FOR],
    [DEVICEATLAS_DIR=$PHP_DEVICEATLAS],
    [
      for i in $SEARCH_PATH; do
        AS_IF([test -r $i/$SEARCH_FOR],
          [DEVICEATLAS_DIR=$i; break;])
      done
    ])
  AC_MSG_CHECKING([for deviceatlas library package])
  AS_VAR_IF([DEVICEATLAS_DIR],, [
    AC_MSG_RESULT([not found])
    AC_MSG_ERROR([Please reinstall the deviceatlas library package])
  ], [AC_MSG_RESULT([found in $DEVICEATLAS_DIR])])
  PHP_ADD_INCLUDE([$DEVICEATLAS_DIR/include])

  LIBNAME=da
  LIBSYMBOL=da_init
  PHP_CHECK_LIBRARY([$LIBNAME], [$LIBSYMBOL], [
      PHP_ADD_LIBRARY_WITH_PATH([$LIBNAME],
        [$DEVICEATLAS_DIR/$PHP_LIBDIR],
        [DEVICEATLAS_SHARED_LIBADD])
      AC_DEFINE([HAVE_DEVICEATLAS_FEATURE], [1],
        [Define to 1 if deviceatlas has the 'FEATURE'.])
    ],
    [AC_MSG_FAILURE([da_init not supported by your deviceatlas library.])],
    [-L$DEVICEATLAS_DIR/$PHP_LIBDIR -lm])

  PHP_SUBST([DEVICEATLAS_SHARED_LIBADD])

  PHP_NEW_EXTENSION([deviceatlas],
    [deviceatlas.c],
    [$ext_shared],,
    [-DZEND_ENABLE_STATIC_TSRMLS_CACHE=1])
])
