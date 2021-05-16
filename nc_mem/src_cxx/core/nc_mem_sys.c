#include "../nc_mem_pch.h"
#include "nc_mem_sys.h"
#if (defined NC_API)
// data //
/// memory_link_type
/// description:
/// ->
/// interface:
/// ->
typedef struct nc_mem_link_t {
    size_t size;
    data_t link;
} nc_mem_link_t;
typedef const nc_mem_link_t nc_mem_sys_tc;
/// memory_system_type
/// description:
/// ->
/// interface:
/// ->
struct nc_mem_sys_t {
    data_t head;
    data_t back;
    data_t curr;
    size_t used;
    nc_mem_list_t list;
} nc_mem_sys = {
    .head = NC_NULL,
    .back = NC_NULL,
    .curr = NC_NULL,
    .list = NC_NULL,
    .used = NC_ZERO
};
// getters //
size_t nc_mem_sys_get_size_full() {
    return nc_mem_sys.back - nc_mem_sys.head;
}
size_t nc_mem_sys_get_size_free() {
    return nc_mem_sys.back - nc_mem_sys.curr;
}
size_t nc_mem_sys_get_size_curr() {
    return nc_mem_sys.used;
}
size_t nc_mem_sys_get_size_back() {
    return nc_mem_sys.back;
}
data_t nc_mem_sys_get_data_full() {
    return nc_mem_sys.data;
}
data_t nc_mem_sys_get_data_free() {
    return nc_mem_sys.back;
}
data_t nc_mem_sys_get_data_curr() {
    return nc_mem_sys.curr;
}
data_t nc_mem_sys_get_data_used() {
    return nc_mem_sys.head + nc_mem_sys.used;
}
// setters //
v1bit_t nc_mem_sys_set_size_full(size_t size) {
    NC_PCALL({ // checking //
        NC_CHECK(size <= NC_MEM_MAXIMAL_SIZE_FULL, "too much memory is requested!", return NC_FALSE);
    }, "setting error!", return NC_FALSE);
    NC_PCALL({ // action //
        nc_mem_sys.data = NC_NULL;
        nc_mem_sys.list = NC_NULL;
        nc_mem_sys.used = NC_ZERO;
        nc_mem_sys.back = NC_ZERO;
        nc_mem_sys.full = size;
    }, "setting error!", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
// predicates //
v1bit_t nc_mem_sys_has_size_full(size_t size) {
    return (nc_mem_sys.full) >= size;
}
v1bit_t nc_mem_sys_has_size_used(size_t size) {
    return (nc_mem_sys.used) >= size;
}
v1bit_t nc_mem_sys_has_size_free(size_t size) {
    return (nc_mem_sys.full - nc_mem_sys.used) >= size;
}
v1bit_t nc_mem_sys_has_data_full(ptr_tc data) {
    return (data >= nc_mem_sys.data) && data <= (nc_mem_sys.data + nc_mem_sys.full);
}
v1bit_t nc_mem_sys_has_data_free(ptr_tc data) {
    return (data >= nc_mem_sys.data) && data <= (nc_mem_sys.data + nc_mem_sys.full - nc_mem_sys.used);
}
v1bit_t nc_mem_sys_has_data_used(ptr_tc data) {
    return (data >= nc_mem_sys.data) && data <= (nc_mem_sys.data + nc_mem_sys.used);
}
// commands //
v1bit_t nc_mem_sys_init() {
    // checking //
    NC_PCALL({
        NC_CHECK(nc_mem_sys_has_size_full(1u) == NC_TRUTH, "init error!", return NC_FALSE);
        NC_CHECK(nc_mem_sys_has_size_used(1u) == NC_FALSE, "init error!", return NC_FALSE);
    }, "init error!", return NC_FALSE);
    // action //
    NC_PCALL({
        NC_MEM_TAKE(nc_mem_sys.data, nc_mem_sys.full);
    }, "init error!", return NC_FALSE);
    // logging //
    NC_OLOG("memory system init:" "{" NC_STR_EOL
        "   data_address: %d;" NC_STR_EOL
        "   size_full: %d;" NC_STR_EOL
        "   size_back: %d;" NC_STR_EOL
        "   size_used: %d;" NC_STR_EOL
        "}",
        nc_mem_sys.data,
        nc_mem_sys.full,
        nc_mem_sys.back,
        nc_mem_sys.used
    );
    // result //
    NC_OLOG("memory system init is done;");
    return NC_TRUTH;
}
v1bit_t nc_mem_sys_quit() {
    // logging //
        NC_OLOG("memory system quit:" "{" NC_STR_EOL
        "   data_address: %d;" NC_STR_EOL
        "   size_full: %d;" NC_STR_EOL
        "   size_back: %d;" NC_STR_EOL
        "   size_used: %d;" NC_STR_EOL
        "}",
        nc_mem_sys.data,
        nc_mem_sys.full,
        nc_mem_sys.back,
        nc_mem_sys.used
    );
    NC_PCALL({ // checking //
        NC_CHECK(nc_mem_sys_has_size_full(1u) == NC_TRUTH, "quit error!", return NC_FALSE);
        NC_CHECK(nc_mem_sys_has_size_used(1u) == NC_FALSE, "quit error!", return NC_FALSE);
    }, "init error", return NC_FALSE);
    NC_PCALL({ // action //
        NC_MEM_FREE(nc_mem_sys.data, nc_mem_sys.full);
        nc_mem_sys_set_size_full(NC_ZERO);
    }, "init error", return NC_FALSE);
    // result //
    NC_OLOG("memory system quit is done;");
    return NC_TRUTH;
}
v1bit_t nc_mem_sys_work() {
    NC_PCALL({ // checking //
        NC_CHECK(nc_mem_sys.data != NC_NULL, "work error!", return NC_FALSE);
    }, "work error!", return NC_FALSE);
    NC_PCALL({ // action //
    }, "work error!", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
NC_API v1bit_t nc_mem_sys_take(nil_t** data, size_t size, size_t alig) {
    // checking //
    NC_PCALL({
        NC_CHECK(data != NC_NULL, "destination must be specified!", return NC_FALSE);
        NC_CHECK(size != NC_ZERO, "cannot allocate zero memory!", return NC_FALSE);
        NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
    }, "memory error!", return NC_FALSE);
    NC_PCALL({ // action //
        alig = NC_MIN(alig, NC_MEM_DEFAULT_CELL_SIZE);
        size = NC_MEM_ALIGN(size, alig);
        if (nc_mem_sys.list.head != NC_NULL) {
            nc_mem_list_get(nc_mem_sys.list, *data, size, NC_NUM_MEQU);
            return *data != NC_NULL;
        }
        if (nc_mem_sys_has_size_free(size)) {
            *data = nc_mem_sys_get_data_back();
            nc_mem_sys.back += size;
            nc_mem_sys.used += size;
            return NC_TRUTH;
        }
    }, "memory error!", return NC_FALSE);
    // result //
    NC_ERROR("memory error!", return NC_FALSE);
    return NC_FALSE;
}
NC_API v1bit_t nc_mem_sys_free(nil_t** data, size_t size, size_t alig) {
    nc_mem_list_t list;
    NC_PCALL({ // checking //
        NC_CHECK(data != NC_NULL, "data must be specified!", return NC_FALSE);
        NC_CHECK(size != NC_ZERO, "cannot deallocate zero memory!", return NC_FALSE);
        NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
        NC_CHECK(nc_mem_sys_has_data_used(*data), "memory error!", return NC_FALSE);
    }, "memory error!", return NC_FALSE);
    NC_PCALL({ // action //
        alig = NC_MIN(alig, NC_MEM_DEFAULT_CELL_SIZE);
        size = NC_MEM_ALIGN(size, alig);
        if (nc_mem_sys_has_size_used(size)) {
            nc_mem_list_add(nc_mem_sys.list, data, size);
            return NC_TRUTH;
        }
    }, "memory error!", return NC_FALSE);
    // result //
    NC_ERROR("memory error!", return NC_FALSE);
    return NC_FALSE;
}
// end_of_file //
#endif  // NC_API //