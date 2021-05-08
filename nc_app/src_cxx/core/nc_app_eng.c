#include "../nc_app_pch.h"
#include "nc_app_eng.h"
#if (defined NC_API)
// data //
/// application_engine_type
/// description:
/// ->
/// interface:
/// ->
struct nc_app_eng_t {
    nc_app_wnd_t window;
    v1bit_t work_flag : 1;
} nc_app_eng;
// getters //
nc_app_wnd_t* nc_app_eng_get_window() {
    return &nc_app_eng.window;
}
// setters //
// predicates //
v1bit_t nc_app_eng_has_work() {
    return nc_app_eng.work_flag;
}
// commands //
v1bit_t nc_app_eng_init() {
    // checking //
    NC_OLOG("init: work flag == %d", nc_app_eng.work_flag);
    NC_CHECK(nc_app_eng.work_flag == NC_TRUTH, "exec is required", return NC_FALSE);
    // action //
    // // systems // //
    nc_mem_sys_set_size_full(NC_MEM_DEFAULT_MEM_SIZE);
    NC_CHECK(nc_mem_sys_init(), "init error!", return NC_FALSE);
    NC_CHECK(nc_aud_sys_init(), "init error!", return NC_FALSE);
    // // window // //
    NC_CHECK(nc_app_wnd_init(&nc_app_eng.window), "init error!", return NC_FALSE);
    // logging //
    NC_OLOG("native_core_engine init:" "{"
        NC_STR_EOL "window:" "{"
            "};"
        "};"
    );
    // result //
    return NC_TRUTH;
}
v1bit_t nc_app_eng_quit() {
    // logging //
    NC_OLOG("native_core_engine quit:" "{"
        NC_STR_EOL "window:" "{"
            "};"
        "};"
    );
    // checking //
    NC_CHECK(nc_app_eng.work_flag == NC_FALSE, "stop is required", return NC_FALSE);
    // action //
    // // window // //
    NC_CHECK(nc_app_wnd_quit(&nc_app_eng.window), "quit error!", return NC_FALSE);
    // // systems // //
    NC_CHECK(nc_aud_sys_quit(), "quit error!", return NC_FALSE);
    NC_CHECK(nc_mem_sys_quit(), "quit error!", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
v1bit_t nc_app_eng_work() {
    // checking //
    // action //
    if (GetAsyncKeyState(NC_KEYCODE_ESCP)) { nc_app_eng_stop(); }
    // // systems // //
    NC_CHECK(nc_mem_sys_work(), "work error!", return NC_FALSE);
    NC_CHECK(nc_aud_sys_work(), "work error!", return NC_FALSE);
    // // window // //
    NC_CHECK(nc_app_wnd_work(&nc_app_eng.window), "work error!", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
NC_API v1bit_t nc_app_eng_exec() {
    // checking //
    //NC_CHECK(nc_app_eng.work_flag == NC_FALSE, "engine is running!", return NC_FALSE);
    // action //
    nc_app_eng.work_flag = NC_TRUTH;
    NC_CHECK(nc_app_eng_init(), "init error", return NC_FALSE);
    while (nc_app_eng.work_flag == NC_TRUTH) { nc_app_eng_work(); }
    NC_CHECK(nc_app_eng_quit(), "quit error", return NC_FALSE);
    // result //
    return NC_TRUTH;
}
NC_API v1bit_t nc_app_eng_stop() {
    // checking //
    NC_CHECK(nc_app_eng.work_flag == NC_TRUTH, "engine is not running!", return NC_FALSE);
    // action //
    nc_app_eng.work_flag = NC_FALSE;
    // result //
    return NC_TRUTH;
}
#endif  // NC_API //
// end_of_file //