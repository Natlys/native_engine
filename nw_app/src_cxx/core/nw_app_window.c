#include "../nw_app_pch.h"
#include "nw_app_window.h"
#if (defined NW_API)
// getters //
// setters //
// predicates //
v1bit_t nw_app_window_set_title(nw_app_window_t* ref, cstr_t title) {
    memset(ref->title, 0, sizeof(ref->title));
    strcpy(ref->title, title);
    return NW_TRUE;
}
// commands //
v1bit_t nw_app_window_init(nw_app_window_t* ref) {
    NW_CREATE(nw_app_window_t, ref);
    return NW_TRUE;
}
v1bit_t nw_app_window_quit(nw_app_window_t* ref) {
    NW_DELETE(nw_app_window_t, ref);
    return NW_TRUE;
}
#endif  // NW_API //
// end_of_file //