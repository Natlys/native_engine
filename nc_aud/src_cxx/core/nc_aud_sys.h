#ifndef NC_AUD_SYSTEM_H
#   define NC_AUD_SYSTEM_H
#include "../nc_aud_core.h"
#   if (defined NC_API)
// includes //
// getters //
// setters //
NC_API v1bit_t nc_aud_sys_set_samp_rate(v1u_t sample_rate);
NC_API v1bit_t nc_aud_sys_set_cell_numb(v1u_t nof_channels);
NC_API v1bit_t nc_aud_sys_set_chnl_numb(v1u_t nof_channels);
// predicates //
// commands //
NC_API v1bit_t nc_aud_sys_init();
NC_API v1bit_t nc_aud_sys_quit();
NC_API v1bit_t nc_aud_sys_work();
#   endif  // NC_API //
// end_of_file //
#endif // NC_AUD_SYSTEM_H //