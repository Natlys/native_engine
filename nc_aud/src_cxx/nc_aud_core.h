#ifndef NC_AUD_CORE_H
#   define NC_AUD_CORE_H
// config //
#   if (defined NC_BUILD)
#       undef NC_BUILD
#   endif   // NC_BUILD //
#   define NC_BUILD NC_BUILD_LIB
// includes //
#   include "nc_aud_pch.h"
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
#   if !(defined NC_AUD_SAMP_NUMB)
#       define NC_AUD_MINIMAL_SAMP_NUMB 128u // 128 samples this amount of time to produce a sound //
#       define NC_AUD_AVERAGE_SAMP_NUMB 256u // 256 samples is optimal value //
#       define NC_AUD_MAXIMAL_SAMP_NUMB 512u // 128 samples  //
#       define NC_AUD_DEFAULT_SAMP_NUMB NC_AUD_AVERAGE_SAMP_NUMB
#   endif // NC_AUD_DEFAULT_SMP_NUMB //
#   if !(defined NC_AUD_DEFAULT_SAMP_RATE)
#       define NC_AUD_MINIMAL_SAMP_RATE 8000u  // 8'000 herz as a bare minumum //
#       define NC_AUD_AVERAGE_SAMP_RATE 44100u // 44'100 herz as a normal value for most systems //
#       define NC_AUD_MAXIMAL_SAMP_RATE 192000u // 192'000 herz is a way more than i usually have //
#       define NC_AUD_DEFAULT_SAMP_RATE NC_AUD_AVERAGE_SAMP_RATE
#   endif // NC_AUD_DEFAULT_SAMP_RATE //
#   if !(defined NC_AUD_DEFAULT_CHNL_NUMB)
#       define NC_AUD_MINIMAL_CHNL_NUMB 1u // 1 channel at least to play sound //
#       define NC_AUD_AVERAGE_CHNL_NUMB 2u // 2 channels can be in many cases //
#       define NC_AUD_MAXIMAL_CHNL_NUMB 4u // 4 channels quadratic record //
#       define NC_AUD_DEFAULT_CHNL_NUMB NC_AUD_MINIMAL_CHNL_NUMB
#   endif // NC_AUD_DEFAULT_CHNL_NUMB //
#   if !(defined NC_AUD_DEFAULT_CELL_NUMB)
#       define NC_AUD_MINIMAL_CELL_NUMB 4u  // 4 cells of samples //
#       define NC_AUD_AVERAGE_CELL_NUMB 8u  // 8 cells of samples //
#       define NC_AUD_MAXIMAL_CELL_NUMB 16u // 16 cells of samples //
#       define NC_AUD_DEFAULT_CELL_NUMB NC_AUD_AVERAGE_CELL_NUMB
#   endif // NC_AUD_DEFAULT_CELL_NUMB //
#   if !(defined NC_AUD_DEFAULT_CELL_SIZE)
#       define NC_AUD_MINIMAL_CELL_SIZE sizeof(v1u08_t) // 1 byte //
#       define NC_AUD_AVERAGE_CELL_SIZE sizeof(v1u16_t) // 2 bytes //
#       define NC_AUD_MAXIMAL_CELL_SIZE sizeof(v1u32_t) // 3 bytes //
#       define NC_AUD_DEFAULT_CELL_SIZE NC_AUD_AVERAGE_CELL_SIZE
#   endif // NC_AUD_DEFAULT_CELL_SIZE //
// end_of_file //
#endif  // NC_AUD_CORE_H //