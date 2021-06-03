#ifndef NC_PRJ_CORE_H
#   define NC_PRJ_CORE_H
/* config */
#   define NC_BUILD NC_BUILD_EXE
/* includes */
#   include "nc_prj_pch.hxx"
/* linkage */
#   if (defined NC_API)
#       undef NC_API
#   endif   /* NC_API */
#   if !(defined NC_BUILD)
#   elif (NC_BUILD & NC_BUILD_EXE)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_LIB)
#       define NC_API extern
#   elif (NC_BUILD & NC_BUILD_DLL)
#       define NC_API NC_DLL_EXPORT
#   else
#       define NC_API NC_DLL_IMPORT
#   endif	/* NC_BUILD */
/* end_of_file */
#endif  /* NC_PRJ_CORE_H */