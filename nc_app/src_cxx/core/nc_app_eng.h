#ifndef NC_APP_ENGINE_H
#   define NC_APP_ENGINE_H
#   include "../nc_app_core.h"
#   if (defined NC_API)
#   include "nc_app_wnd.h"
#   if (defined __cplusplus)
extern "C" {
#   endif // __cplusplus //
// getters //
NC_API nc_app_wnd_t* nc_app_eng_get_window();
// setters //
// predicates //
NC_API v1bit_t nc_app_eng_has_work();
// commands //
NC_API v1bit_t nc_app_eng_init();
NC_API v1bit_t nc_app_eng_quit();
NC_API v1bit_t nc_app_eng_work();
NC_API v1bit_t nc_app_eng_exec();
NC_API v1bit_t nc_app_eng_stop();
#   if (defined __cplusplus)
}
#   endif // __cplusplus //
#   endif  // NC_API //
// end_of_file //
#endif  // NC_APP_ENGINE_H //