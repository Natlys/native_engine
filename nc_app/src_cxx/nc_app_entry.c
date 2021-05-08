#include "nc_app_pch.h"
#include "nc_app.h"

#if (defined NC_BUILD)
#   include "../../nc_lib/src_cxx/core/nc_lib_load.h"
#   if (NC_BUILD & NC_BUILD_EXE)
int main(int arg_num, const char* arg_str[])
{
    NC_OLOG(NC_GREETING);
    NC_CHECK(nc_app_eng_exec(), "exec error!", ({ system("pause"); return -1; }));
    NC_BREAK();

    return 0;
}
#   endif   // NC_BUILD_EXE //
#endif  // NC_BUILD //
// end_of_file //