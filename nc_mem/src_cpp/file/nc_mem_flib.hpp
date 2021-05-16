#ifndef NC_MEM_FILE_LIBRARY_HPP
#   define NC_MEM_FILE_LIBRARY_HPP
#   include "../nc_mem_core.hpp"
#   if (defined NC_FLIB)
// includes //
#       include <experimental/filesystem>
// types //
#       if (NC_FLIB & NC_FLIB_STD)
#           define flib              std::experimental::filesystem
#           define nc_fpath_t        flib::path;
#           define nc_fpath_tc       const flib::path;
#           define nc_fdir_info_t    flib::directory_entry
#           define nc_fdir_info_tc   const flib::directory_entry
#           define nc_fdir_iter_t    flib::directory_iterator
#           define nc_fdir_iter_tc   const flib::directory_iterator;
#           define nc_fspace_info_t  flib::space_info;
#           define nc_fspace_info_tc const flib::space_info;
#       endif   // NC_FLIB_STD //
#       if (NC_FLIB & NC_FLIB_OWN)
#       endif   // NC_FLIB_OWN //
#   endif    // NC_FLIB //
// end_of_file //
#endif  // NC_MEM_FILE_LIBRARY_HPP //