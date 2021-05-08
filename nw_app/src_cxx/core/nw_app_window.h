#ifndef NW_APP_WINDOW_H
#   define NW_APP_WINDOW_H
#   include "../nw_app_core.h"
#   if (defined NW_API)
#   if (defined __cplusplus)
extern "C" {
#   endif // __cplusplus //
/// engine_type
/// description:
typedef struct {
    char_t title[NW_MAX_NAME];
} nw_app_window_t;
// getters //
// setters //
v1bit_t NW_API nw_app_window_set_title(nw_app_window_t* ref, cstr_t title);
// predicates //
// commands //
v1bit_t NW_API nw_app_window_init(nw_app_window_t* ref);
v1bit_t NW_API nw_app_window_quit(nw_app_window_t* ref);
#   if (defined __cplusplus)
}
#   endif // __cplusplus //
#   endif  // NW_API //
// end_of_file //
#endif  // NW_APP_WINDOW_H //