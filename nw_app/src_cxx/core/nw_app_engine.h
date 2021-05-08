#ifndef NW_APP_ENGINE_H
#   define NW_APP_ENGINE_H
#   include "../nw_app_core.h"
#   if (defined NW_API)
#   include "nw_app_window.h"
#   if (defined __cplusplus)
extern "C" {
#   endif // __cplusplus //
/// engine_type
/// description:
typedef struct {
    nw_app_window_t* window;
    v1bit_t work_flag;
} nw_app_engine_t;
// getters //
// setters //
// predicates //
// commands //
v1bit_t NW_API nw_app_engine_init(nw_app_engine_t* ref);
v1bit_t NW_API nw_app_engine_quit(nw_app_engine_t* ref);
v1bit_t NW_API nw_app_engine_work(nw_app_engine_t* ref);
#   if (defined __cplusplus)
}
#   endif // __cplusplus //
#   endif  // NW_API //
// end_of_file //
#endif  // NW_APP_ENGINE_H //