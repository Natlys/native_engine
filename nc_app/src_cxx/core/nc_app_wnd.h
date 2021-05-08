#ifndef NC_APP_WINDOW_H
#   define NC_APP_WINDOW_H
#   include "../nc_app_core.h"
#   if (defined NC_API)
#   if (defined __cplusplus)
extern "C" {
#   endif // __cplusplus //
/// window_type
/// description:
typedef struct {
    str_t title;
} nc_app_wnd_t;
typedef const nc_app_wnd_t nc_app_wnd_tc;
// ctor_dtor //
NC_API v1bit_t nc_app_wnd_ctor(nc_app_wnd_t* ref);
NC_API v1bit_t nc_app_wnd_dtor(nc_app_wnd_t* ref);
// getters //
// setters //
NC_API v1bit_t nc_app_wnd_set_title(nc_app_wnd_t* ref, cstr_t title);
// predicates //
// commands //
NC_API v1bit_t nc_app_wnd_init(nc_app_wnd_t* ref);
NC_API v1bit_t nc_app_wnd_quit(nc_app_wnd_t* ref);
NC_API v1bit_t nc_app_wnd_work(nc_app_wnd_t* ref);
#   if (defined __cplusplus)
}
#   endif // __cplusplus //
#   endif  // NC_API //
// end_of_file //
#endif  // NC_APP_WINDOW_H //