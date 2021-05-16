#ifndef NC_MEM_SYSTEM_H
#   define NC_MEM_SYSTEM_H
#   include "../nc_mem_core.h"
#   if (defined NC_API)
#       include "../std/nc_mem_list.h"
#       include "../std/nc_mem_giver.h"
// defines //
#   if (NC_FALSE)
#       if (defined NC_MEM_TAKE)
#           undef NC_MEM_TAKE
#       endif // NC_MEM_TAKE
#       define NC_MEM_TAKE(ref, size) ({ nc_mem_sys_take(&ref, size, NC_MEM_MINIMAL); })
#       if (defined NC_MEM_FREE)
#           undef NC_MEM_FREE
#       endif // NC_MEM_FREE
#       define NC_MEM_FREE(ref, size) ({ nc_mem_sys_free(&ref, size, NC_MEM_MINIMAL); })
#       if (defined NC_MEM_MOVE)
#           undef NC_MEM_MOVE
#       endif // NC_MEM_MOVE
#       define NC_MEM_MOVE(ref, sz_old, sz_new) ({ nc_mem_sys_move(&ref, sz_old, sz_new); })
#   endif
// getters //
NC_API size_t nc_mem_sys_get_size_full();
NC_API size_t nc_mem_sys_get_size_free();
NC_API size_t nc_mem_sys_get_size_used();
NC_API size_t nc_mem_sys_get_size_back();
NC_API data_t nc_mem_sys_get_data_full();
NC_API data_t nc_mem_sys_get_data_free();
NC_API data_t nc_mem_sys_get_data_curr();
NC_API data_t nc_mem_sys_get_data_used();
// setters //
NC_API v1bit_t nc_mem_sys_set_size(size_t size);
// predicates //
NC_API v1bit_t nc_mem_sys_has_size_full(size_t size);
NC_API v1bit_t nc_mem_sys_has_size_free(size_t size);
NC_API v1bit_t nc_mem_sys_has_size_curr(size_t size);
NC_API v1bit_t nc_mem_sys_has_size_used(size_t size);
NC_API v1bit_t nc_mem_sys_has_data_full(ptr_tc data);
NC_API v1bit_t nc_mem_sys_has_data_free(ptr_tc data);
NC_API v1bit_t nc_mem_sys_has_data_curr(ptr_tc data);
NC_API v1bit_t nc_mem_sys_has_data_used(ptr_tc data);
// commands //
NC_API v1bit_t nc_mem_sys_init();
NC_API v1bit_t nc_mem_sys_quit();
NC_API v1bit_t nc_mem_sys_work();
NC_API v1bit_t nc_mem_sys_take(nil_t** data, size_t size, size_t alig);
NC_API v1bit_t nc_mem_sys_free(nil_t** data, size_t size, size_t alig);
#   endif  // NC_API //
// end_of_file //
#endif // NC_MEM_SYSTEM_H //