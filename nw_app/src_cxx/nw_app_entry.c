#include "nw_app_pch.h"
#include "nw_app.h"

int main(int arg_num, const char* arg_str[])
{
    v1u_t iarg;
    for (iarg = 0u; iarg < arg_num; iarg += 1u) {
        printf("arg_%d: %s;\n", iarg, arg_str[iarg]);
    }
    nw_app_engine_t* ref = NW_NULL;
    nw_app_engine_work(ref);
    system("pause");
    return 0;
}