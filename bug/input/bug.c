#include <dac.h>

int main(int argc, char **argv) {
    da_atlas_t atlas;
    da_status_t status;
    int da_inited = 0;
    void *ptr = NULL;
    da_evidence_t *evidence = NULL;

    printf("Init\n");
    status = da_init();
    if (status != DA_OK) goto main_err;
    da_inited = 1;

    printf("Read bin\n");
    size_t ptr_size;
    status = da_atlas_read_mapped("/work/data.bin", NULL, &ptr, &ptr_size);
    if (status != DA_OK) goto main_err;

    printf("Open db\n");
    da_property_decl_t extraprops[] = {{ 0, 0 }};
    status = da_atlas_open(&atlas, extraprops, ptr, ptr_size);
    if (status != DA_OK) goto main_err;

    printf("Populate evidence (count=%d)\n", (argc - 1) / 2);
    evidence = calloc(argc - 1, sizeof(*evidence));
    size_t evidence_len = 0;
    int i;
    for (i = 1; i < argc; i += 2) {
        printf("  %32s => %s\n", argv[i], argv[i + 1]);
        da_evidence_id_t evidence_id = da_atlas_header_evidence_id(&atlas, argv[i]);
        if (evidence_id == (da_evidence_id_t)-1) continue;
        evidence[evidence_len].key = evidence_id;
        evidence[evidence_len].value = argv[i + 1];
        evidence_len += 1;
    }

    printf("Search for device\n");
    da_deviceinfo_t device;
    // This segfaults if using the dajsonconv version (data.bin)
    // It works if using data.json + da_atlas_compile
    status = da_searchv(&atlas, &device, evidence, evidence_len);
    if (status != DA_OK) goto main_err;

    printf("Print device props\n");
    da_propid_t *prop;
    const char *prop_name;
    da_type_t prop_type;
    status = da_getfirstprop(&device, &prop);
    while (status == DA_OK) {
        da_getpropname(&device, *prop, &prop_name);
        da_getproptype(&device, *prop, &prop_type);
        switch (prop_type) {
            case DA_TYPE_NUMBER:
            case DA_TYPE_INTEGER:
                long lval;
                da_getpropinteger(&device, *prop, &lval);
                printf("%s: %ld\n", prop_name, lval);
                break;
            case DA_TYPE_BOOLEAN:
                bool bval;
                da_getpropboolean(&device, *prop, &bval);
                printf("%s: %s\n", prop_name, bval ? "true" : "false");
                break;
            case DA_TYPE_STRING:
                const char *sval;
                da_getpropstring(&device, *prop, &sval);
                printf("%s: %s\n", prop_name, sval);
                break;
            default:
                // TODO: Warn?
                break;
        }
        status = da_getnextprop(&device, &prop);
    }

    da_close(&device);
    return 0;

main_err:
    printf("Error\n");
    if (da_inited) da_fini();
    if (ptr) free(ptr);
    if (evidence) free(evidence);
    return 1;
}
