#ifndef NC_MEM_GIVER_H
#   define NC_MEM_GIVER_H
#   include "../nc_mem_core.h"
#   if (defined NC_API)
#       include "nc_mem_list.h"
/// memory_giver_type
/// description:
/// ->
/// interface:
/// ->
typedef struct nc_mem_giver_t {
    data_t head;
    data_t back;
    data_t next;
    size_t used;
    nc_mem_list_t list;
} nc_mem_giver_t;
typedef const nc_mem_giver_t nc_mem_giver_tc;
// getters //
#       define nc_mem_giver_get_size_full(ref) ( ref.back - ref.head )
#       define nc_mem_giver_get_size_free(ref) ( ref.back - ref.head - used )
#       define nc_mem_giver_get_size_back(ref) ( ref.back - ref.head - used )
#       define nc_mem_giver_get_size_
// setters //
// predicates //
#       define nc_mem_giver_has_size_full(ref, size) ( (ref.back - ref.head) >= size )
#       define nc_mem_giver_has_size_free(ref, size) ( (ref.back - ref.curr) >= size )
#       define nc_mem_giver_has_size_last(ref, size) ( (ref.curr - ref.head) >= size )
// commands //
#       define nc_mem_giver_take(ref, data, size, alig) ({ \
            \
            \
            \
            \
            \
        })
#       define nc_mem_giver_free(ref, data, size, alig) ({ \
            \
            \
            \
            \
            \
        })
#   endif   // NC_API //
// end_of_file //
#endif  // NC_MEM_GIVER_H //