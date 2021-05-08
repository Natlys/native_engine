#include "../nc_app_pch.h"
#include "nc_app_wnd.h"
#if (defined NC_API)
// ctor_dtor //
v1bit_t nc_app_wnd_ctor(nc_app_wnd_t* ref) {
    // checking //
    // action //
    // result //
    return NC_TRUTH;
}
v1bit_t nc_app_wnd_dtor(nc_app_wnd_t* ref) {
    // checking //
    // action //
    // result //
    return NC_TRUTH;
}
// getters //
// setters //
// predicates //
v1bit_t nc_app_wnd_set_title(nc_app_wnd_t* ref, cstr_t title) {
    memset(ref->title, 0, sizeof(ref->title));
    strcpy(ref->title, title);
    return NC_TRUTH;
}
// commands //
v1bit_t nc_app_wnd_init(nc_app_wnd_t* ref) {
    // checking //
    // action //
    // result //
    return NC_TRUTH;
}
v1bit_t nc_app_wnd_quit(nc_app_wnd_t* ref) {
    // checking //
    // action //
    // result //
    return NC_TRUTH;
}
v1bit_t nc_app_wnd_work(nc_app_wnd_t* ref) {
    // checking //
    // action //
    // result //
    return NC_TRUTH;
}
#endif  // NC_API //
// end_of_file //