#include "../nw_app_pch.h"
#include "nw_app_engine.h"
#if (defined NW_API)
// commands //
v1bit_t nw_app_engine_init(nw_app_engine_t* ref) {
    printf("app_engine_init;\n");
    NW_CREATE(nw_app_engine_t, ref);
    nw_app_window_init(ref->window);
    return NW_TRUE;
}
v1bit_t nw_app_engine_quit(nw_app_engine_t* ref) {
    printf("app_engine_quit;\n");
    nw_app_window_quit(ref->window);
    NW_DELETE(nw_app_engine_t, ref);
    return NW_TRUE;
}
v1bit_t nw_app_engine_work(nw_app_engine_t* ref) {
    printf("app_engine_work;\n");
    nw_app_engine_init(ref);
    nw_app_engine_quit(ref);
    return NW_TRUE;
}
#endif  // NW_API //
// end_of_file //