#ifndef NC_MEM_CORE_HPP
#   define NC_MEM_CORE_HPP
// config //
#	if (defined NC_BUILD)
#		undef NC_BUILD
#	endif // NC_BUILD //
#	define NC_BUILD NC_BUILD_LIB
// // file_library // //
#   if !(defined NC_FLIB)
#       define NC_FLIB_STD 1 << 0
#       define NC_FLIB_OWN 1 << 1
#       define NC_FLIB     NC_FLIB_STD
#   endif   // NC_FLIB //
// includes //
#   include "nc_mem_pch.hpp"
// linkage //
// // remove old NC_API // //
#   if (defined NC_API)
#       undef NC_API
#   endif   // NC_API //
// // remake new NC_API // //
#   if !(defined NC_BUILD)
#   elif (NC_BUILD & NC_BUILD_EXE)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_LIB)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_DLL)
#       define NC_API NC_DLL_EXPORT
#   else
#       define NC_API NC_DLL_IMPORT
#   endif	// NC_BUILD //
// defines //
// // constants // //
#   if !(defined NC_MEM_DEFAULT_SIZE_FULL)
#       define NC_MEM_MINIMAL_SIZE_FULL ( 1ul << 10ul ) // 1 kb as an optimal value for a little test; //
#       define NC_MEM_AVERAGE_SIZE_FULL ( 1ul << 20ul ) // 1 mb as an optimal value for an applcation; //
#       define NC_MEM_MAXIMAL_SIZE_FULL ( 1ul << 30ul ) // 1 gb as a huge amount of memory for the release mode; //
#       define NC_MEM_DEFAULT_SIZE_FULL NC_MEM_AVERAGE_SIZE_FULL
#   endif   // NC_MEM_DEFAULT_SIZE_FULL //
#   if !(defined NC_MEM_DEFAULT_SIZE_CELL)
#       define NC_MEM_MINIMAL_SIZE_CELL ( 1ul << 3ul ) // 8 bytes to store a pointer or size //
#       define NC_MEM_AVERAGE_SIZE_CELL ( 1ul << 4ul ) // 16 bytes to store a pointer and size //
#       define NC_MEM_MAXIMAL_SIZE_CELL ( 1ul << 5ul ) // 32 bytes to store 2 pointers and 2 sizes //
#       define NC_MEM_DEFAULT_SIZE_CELL NC_MEM_AVERAGE_SIZE_CELL
#   endif   // NC_MEM_DEFAULT_SIZE_CELL //
// // functions // //
#   if !(defined NC_MEM_TAKE)
#       define NC_MEM_TAKE(ref, size) ({ /*_____________*/ \
            ref = NC_CAST(malloc(size), typeof(ref)); /**/ \
        })
#   endif   // NC_MEM_TAKE //
#   if !(defined NC_MEM_FREE)
#       define NC_MEM_FREE(ref, size) ({ /*_____________*/ \
            ::free(ref); ref = NC_NULL; /*______________*/ \
        })
#   endif   // NC_MEM_FREE //
#   if !(defined NC_MEM_MOVE)
#       define NC_MEM_MOVE(ref, sz_old, sz_new) ({ /*____*/ \
            if (ref != NC_NULL) { /*there is some data___*/ \
                if (sz_new == NC_ZERO) { /*make it free__*/ \
                NC_MEM_FREE(ref, sz_old); /*deallocate it*/ \
                } else { /*size has been changed_________*/ \
                    typeof(ref) cpy = ref; /*use old data*/ \
                    size_t sz_cpy = NC_MIN(sz_old, sz_new); \
                    NC_MEM_TAKE(ref, sz_new); /*take new_*/ \
                    memmove(ref, old, sz_cpy); /*move old*/ \
                    NC_MEM_FREE(old, sz_old); /*free old_*/ \
                } /*reallocation is done_________________*/ \
            } else { /*we want to allocate a memory______*/ \
                NC_MEM_TAKE(ref, size_new); /*allocate it*/ \
            } /*reallocation for a block of sz_new_______*/ \
        })
#   endif   // NC_MEM_MOVE //
#   if !(defined NC_MEM_NEW_ONE)
#       define NC_MEM_NEW_ONE(tname, tdata, ...) ({ /*___*/ \
            size_t size = sizeof(tname##_t) * number; /*_*/ \
            NC_MEM_TAKE(tdata, size); /*_________________*/ \
            tname##_ctor(tname, __VA_ARGS__); /*_________*/ \
        })
#   endif // NC_MEM_NEW_ONE //
#   if !(defined NC_MEM_NEW_ARR)
#       define NC_MEM_NEW_ARR(tname, tdata, number) ({ /**/ \
            size_t size = sizeof(tname##_t) * number; /*_*/ \
            NC_MEM_TAKE(tdata, size);     /*allocate_____*/ \
            memset(tdata, NC_ZERO, size); /*reset that___*/ \
        })
#   endif // NC_MEM_NEW_ARR //
#   if !(defined NC_MEM_DEL_ONE)
#       define NC_MEM_DEL_ONE(tname, tdata) ({ /*________*/ \
            size_t size = sizeof(tname##_t); /*__________*/ \
            tname##_dtor(tdata); /*destruct the object___*/ \
            NC_MEM_FREE(tdata, size); /*deallocate memory*/ \
        })
#   endif // NC_MEM_DEL_ONE //
#   if !(defined NC_MEM_DEL_ARR)
#       define NC_MEM_DEL_ARR(tname, tdata, number) ({ /**/ \
            size_t size = sizeof(tname##_t) * number; /*_*/ \
            tname##_t* itr = tdata; /*from beginning to__*/ \
            while (itr != (tdata + number)) { /*the end__*/ \
                tname##_dtor(itr++); /*destruct everyone_*/ \
            } /*so we can keep raii principle____________*/ \
            NC_MEM_FREE(tdata, size); /*deallocate_______*/ \
        })
#   endif // NC_MEM_DEL_ARR //
// end_of_file //
#endif  // NC_MEM_CORE_HPP //