#ifndef PHP_DEVICEATLAS_H
#define PHP_DEVICEATLAS_H

#include <dac.h>

extern zend_module_entry deviceatlas_module_entry;
#define phpext_deviceatlas_ptr &deviceatlas_module_entry

#define PHP_DEVICEATLAS_VERSION "0.1.0"

ZEND_BEGIN_MODULE_GLOBALS(deviceatlas)
    int initialized;
    // char *bin_path;
    char *json_path;
    zend_long cache_size;
    void *atlas_ptr;
    size_t atlas_ptr_size;
    da_atlas_t atlas;
    da_evidence_t *evidence;
    size_t evidence_size;
    da_status_t last_err;
    char last_err_msg[1024];
ZEND_END_MODULE_GLOBALS(deviceatlas)

enum deviceatlas_php_status {
    DA_PHP_EMPTY_PATH = DA_STATUS_LAST + 1,
    DA_PHP_BAD_PATH,
    DA_PHP_NO_MEM
};

ZEND_EXTERN_MODULE_GLOBALS(deviceatlas)
#define DA_G(v) ZEND_MODULE_GLOBALS_ACCESSOR(deviceatlas, v)

#if defined(ZTS) && defined(COMPILE_DL_DEVICEATLAS)
ZEND_TSRMLS_CACHE_EXTERN()
#endif

#endif // PHP_DEVICEATLAS_H
