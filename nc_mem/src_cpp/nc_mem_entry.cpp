#include "nc_mem_pch.hpp"
#include "nc_mem.hpp"

#if (defined NC_BUILD)
#   if (NC_BUILD & NC_BUILD_EXE)
int main(int arg_num, const char* arg_str[])
{
    NC_OLOG(NC_GREETING);
    system("pause");
    return 0;
}
#   endif
#endif // NC_BUILD //
// end_of_file //