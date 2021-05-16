#ifndef NC_MEM_UTILITY_H
#   define NC_MEM_UTILITY_H
#   include "../nc_mem_core.h"
#   if (defined NC_API)
// data //
/// memory_iterator_type
/// description:
/// ->holds the size of the block where it is allocated;
/// ->references next block in other location;
/// interface:
/// ->emplace it in some memory block;
/// ->write the size of the block in the iterator;
/// ->construct a chain of blocks and change it;
typedef struct nc_mem_iter_t nc_mem_iter_t;
typedef struct nc_mem_iter_t {
    nc_mem_iter_t* next;
    size_t size;
} nc_mem_iter_t;
typedef const nc_mem_iter_t nc_mem_iter_tc;
// data //
/// memory_list_type
/// description:
/// ->holds a memory iterator;
/// ->connects the entire chain of memory blocks;
/// interface:
/// ->add memory blocks one by one;
/// ->get and remove iterators if you need;
/// ->find matching blocks using size;
typedef struct nc_mem_list_t {
    nc_mem_iter_t* head;
} nc_mem_list_t;
typedef const nc_mem_list_t nc_mem_list_tc;
// ctor_dtor //
#       define nc_mem_list_ctor(ref) ({ \
            ref->head = NC_NULL;        \
        })
#       define nc_mem_list_dtor(ref) ({ \
            ref->head = NC_NULL;        \
        })
// getters //
#       define nc_mem_list_get(ref, data, sz__, oper) ({    \
            nc_mem_iter_t* iter = ref.head;                 \
            while(iter->next != NC_NULL) {                  \
                size_t size = iter->next->size;             \
                if (##oper(size, sz__)) {                   \
                        iter->next = iter->next->next;      \
                    if (size == sz__) {                     \
                    } else if (size > sz__) {               \
                        iter->next->size -= sz__;           \
                    } else {                                \
                        NC_ERROR("size error", NC_DEFAULT); \
                    }                                       \
                    break;                                  \
                }                                           \
                else { iter = iter->next; }                 \
            }                                               \
        })
#       define nc_mem_list_add(ref, data, size) ({ \
            nc_mem_iter_t* temp = ref.head;        \
            ref.head = (nc_mem_iter_t*)data;       \
            ref.head->size = size;                 \
            ref.head->next = temp;                 \
        })
#       define nc_mem_list_rmv(ref, data, size) ({   \
            nc_mem_link_t* temp = ref.head;          \
            while(temp->next != data) {              \
                temp = temp->next;                   \
            }                                        \
            if (temp->next->size == size) {          \
                temp->next = temp->next->next;       \
            } else if (temp->next->size > size) {    \
                temp->next->size -= size; \
            } else {                                 \
                NC_ERROR("iter error!", NC_DEFAULT); \
            }                                        \
        })
// setters //
// predicates //
// commands //
#   endif   // NC_API //
// end_of_file //
#endif  // NC_MEM_UTILITY_H //