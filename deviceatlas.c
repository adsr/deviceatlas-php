#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include "php.h"
#include "ext/standard/info.h"
#include "php_deviceatlas.h"
#include "deviceatlas_arginfo.h"

#include <dac.h>

ZEND_DECLARE_MODULE_GLOBALS(deviceatlas)
static PHP_MINFO_FUNCTION(deviceatlas);
static PHP_MINIT_FUNCTION(deviceatlas);
static PHP_MSHUTDOWN_FUNCTION(deviceatlas);
static size_t deviceatlas_filereader(void *ctx, size_t count, char *buf);
static da_status_t deviceatlas_fileseeker(void *ctx, off_t pos);
static void deviceatlas_error_handler(da_severity_t severity, da_status_t status, const char *msg, va_list args);
static int deviceatlas_maybe_initialize(void);
static void deviceatlas_error_cb(da_severity_t severity, da_status_t status, const char *msg, va_list args);
static void deviceatlas_clear_last_err(void);
static void deviceatlas_set_last_err_va(int err, const char *fmt, va_list vl);
static void deviceatlas_set_last_err(int err, const char *fmt, ...);


zend_module_entry deviceatlas_module_entry = {
    STANDARD_MODULE_HEADER,
    "deviceatlas",
    ext_functions,
    PHP_MINIT(deviceatlas),
    PHP_MSHUTDOWN(deviceatlas),
    NULL,
    NULL,
    PHP_MINFO(deviceatlas),
    PHP_DEVICEATLAS_VERSION,
    STANDARD_MODULE_PROPERTIES
};

#ifdef COMPILE_DL_DEVICEATLAS
#ifdef ZTS
ZEND_TSRMLS_CACHE_DEFINE()
#endif
ZEND_GET_MODULE(deviceatlas)
#endif

PHP_INI_BEGIN()
    STD_PHP_INI_ENTRY("deviceatlas.bin_path", "", PHP_INI_ALL, OnUpdateString, bin_path, zend_deviceatlas_globals, deviceatlas_globals)
    STD_PHP_INI_ENTRY("deviceatlas.json_path", "", PHP_INI_ALL, OnUpdateString, json_path, zend_deviceatlas_globals, deviceatlas_globals)
    STD_PHP_INI_ENTRY("deviceatlas.cache_size", "0", PHP_INI_ALL, OnUpdateLong, cache_size, zend_deviceatlas_globals, deviceatlas_globals)
PHP_INI_END()

static PHP_MINFO_FUNCTION(deviceatlas) {
    php_info_print_table_start();
    php_info_print_table_header(2, "deviceatlas support", "enabled");
    php_info_print_table_header(2, "extension version", PHP_DEVICEATLAS_VERSION);
    php_info_print_table_header(2, "library version", DATLAS_DA_VERSION);
    php_info_print_table_end();
}

static PHP_MINIT_FUNCTION(deviceatlas) {
    REGISTER_INI_ENTRIES();
    register_deviceatlas_symbols(module_number);
    da_seterrorfunc(deviceatlas_error_cb);
    deviceatlas_clear_last_err();
    deviceatlas_maybe_initialize();
    return SUCCESS;
}

static PHP_MSHUTDOWN_FUNCTION(deviceatlas) {
    if (DA_G(initialized)) {
        da_atlas_close(&DA_G(atlas));
        free(DA_G(atlas_ptr));
        da_fini();
        DA_G(initialized) = 0;
    }
    if (DA_G(evidence_size) > 0) {
        free(DA_G(evidence));
        DA_G(evidence_size) = 0;
    }
    UNREGISTER_INI_ENTRIES();
    return SUCCESS;
}

static size_t deviceatlas_filereader(void *ctx, size_t count, char *buf) {
   return fread(buf, 1, count, ctx);
}

static da_status_t deviceatlas_fileseeker(void *ctx, off_t pos) {
   return fseek(ctx, pos, SEEK_SET) != -1 ? DA_OK : DA_SYS;
}

static void deviceatlas_error_handler(da_severity_t severity, da_status_t status, const char *msg, va_list args) {
    if (status != DA_OK) {
        deviceatlas_set_last_err_va(status, msg, args);
    }
}

static int deviceatlas_maybe_initialize(void) {
    if (DA_G(initialized)) {
        // Already initialized
        return 1;
    }

    // Do we have a JSON or compiled db?
    int has_bin  = DA_G(bin_path)  && strlen(DA_G(bin_path))  > 0 ? 1 : 0;
    int has_json = DA_G(json_path) && strlen(DA_G(json_path)) > 0 ? 1 : 0;
    if (!has_bin && !has_json) {
        deviceatlas_set_last_err(DA_PHP_EMPTY_PATH, "Empty bin_path and json_path");
        return 0;
    }

    // Set error handler. The library invokes this when there's an error.
    // We use it to set last_err.
    da_seterrorfunc(deviceatlas_error_handler);

    // Start init
    da_status_t status;
    if ((status = da_init()) != DA_OK) {
        return 0;
    }

    void *ptr = NULL;
    size_t ptr_size = 0;
    void *m = NULL;

    if (has_bin) {
        // Read binary db
        if ((status = da_atlas_read_mapped(DA_G(bin_path), m, &ptr, &ptr_size)) != DA_OK) {
            goto deviceatlas_maybe_initialize_err;
        }
    } else if (has_json) {
        // Read JSON
        FILE *json = fopen(DA_G(json_path), "r");
        if (!json) {
            deviceatlas_set_last_err(DA_PHP_BAD_PATH, "Could not read json_path");
            goto deviceatlas_maybe_initialize_err;
        }

        // Compile JSON into db
        status = da_atlas_compile(json, deviceatlas_filereader, deviceatlas_fileseeker, &ptr, &ptr_size);
        fclose(json);
        if (status != DA_OK) {
            goto deviceatlas_maybe_initialize_err;
        }
    }

    // Open db
    da_property_decl_t extraprops[] = {{ 0, 0 }};
    if ((status = da_atlas_open(&DA_G(atlas), extraprops, ptr, ptr_size)) != DA_OK) {
        goto deviceatlas_maybe_initialize_err;
    }

    // Finish init
    DA_G(atlas).config.cache_size = DA_G(cache_size);
    DA_G(atlas_ptr) = ptr;
    DA_G(atlas_ptr_size) = ptr_size;
    DA_G(initialized) = 1;

    return 1;

deviceatlas_maybe_initialize_err:

    if (ptr) free(ptr);
    da_fini();
    return 0;
}

static void deviceatlas_error_cb(da_severity_t severity, da_status_t status, const char *msg, va_list args) {
    DA_G(last_err) = status;
    vsnprintf(DA_G(last_err_msg), sizeof(DA_G(last_err_msg)), msg, args);
}

static void deviceatlas_clear_last_err(void) {
    DA_G(last_err) = DA_OK;
    DA_G(last_err_msg)[0] = '\0';
}

static void deviceatlas_set_last_err_va(int err, const char *fmt, va_list vl) {
    DA_G(last_err) = err;
    vsnprintf(DA_G(last_err_msg), sizeof(DA_G(last_err_msg)), fmt, vl);
}

static void deviceatlas_set_last_err(int err, const char *fmt, ...) {
    va_list vl;
    va_start(vl, fmt);
    deviceatlas_set_last_err_va(err, fmt, vl);
}

PHP_FUNCTION(deviceatlas_get_properties) {
    HashTable *headers;

    ZEND_PARSE_PARAMETERS_START(1, 1)
        Z_PARAM_ARRAY_HT(headers)
    ZEND_PARSE_PARAMETERS_END();

    deviceatlas_clear_last_err();

    if (!deviceatlas_maybe_initialize()) {
        RETURN_FALSE;
    }

    // (Re)alloc evidence memory
    if (zend_hash_num_elements(headers) > DA_G(evidence_size)) {
        DA_G(evidence) = realloc(DA_G(evidence), sizeof(da_evidence_t) * zend_hash_num_elements(headers));
        if (!DA_G(evidence)) {
            deviceatlas_set_last_err(DA_PHP_NO_MEM, "realloc failed");
            RETURN_FALSE;
        }
        DA_G(evidence_size) = zend_hash_num_elements(headers);
    }

    // Populate evidence
    zend_string *key;
    zval *entry;
    zval entry_str;
    size_t evidence_len = 0;
    ZEND_HASH_FOREACH_STR_KEY_VAL(headers, key, entry) {
        if (!key) continue;
        convert_to_string(entry);
        da_evidence_id_t evidence_id = da_atlas_header_evidence_id(&DA_G(atlas), ZSTR_VAL(key));
        if (evidence_id == (da_evidence_id_t)-1) continue;
        DA_G(evidence)[evidence_len].key = evidence_id;
        DA_G(evidence)[evidence_len].value = Z_STRVAL_P(entry);
        evidence_len += 1;
    } ZEND_HASH_FOREACH_END();

    // Search for device
    da_status_t status;
    da_deviceinfo_t device;
    if ((status = da_searchv(&DA_G(atlas), &device, DA_G(evidence), evidence_len)) != DA_OK) {
        RETURN_FALSE;
    }

    // Return device props
    da_propid_t *prop;
    const char *prop_name;
    da_type_t prop_type;
    array_init(return_value);
    status = da_getfirstprop(&device, &prop);
    while (status == DA_OK) {
        da_getpropname(&device, *prop, &prop_name);
        da_getproptype(&device, *prop, &prop_type);
        switch (prop_type) {
            case DA_TYPE_NUMBER:
            case DA_TYPE_INTEGER:
                long lval;
                da_getpropinteger(&device, *prop, &lval);
                add_assoc_long(return_value, prop_name, lval);
                break;
            case DA_TYPE_BOOLEAN:
                bool bval;
                da_getpropboolean(&device, *prop, &bval);
                add_assoc_bool(return_value, prop_name, bval);
                break;
            case DA_TYPE_STRING:
                const char *sval;
                size_t val_len;
                da_getpropstring(&device, *prop, &sval);
                add_assoc_string(return_value, prop_name, sval);
                break;
            default:
                // TODO: Warn?
                break;
        }
        status = da_getnextprop(&device, &prop);
    }

    da_close(&device);
}

PHP_FUNCTION(deviceatlas_is_initialized) {
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_BOOL(DA_G(initialized));
}

PHP_FUNCTION(deviceatlas_last_error) {
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_LONG(DA_G(last_err));
}

PHP_FUNCTION(deviceatlas_last_error_msg) {
    ZEND_PARSE_PARAMETERS_NONE();
    RETURN_STRING(DA_G(last_err_msg));
}
