#ifndef NC_TYPE_H
#   define NC_TYPE_H
// #include "../nc_core.hxx"
#   if (defined NC_API)
/* includes */
// types //
/// native_code_type
/// description:
/// ->
/// interaction:
/// ->
typedef struct {
    cstr_t name;
} nc_type_t;
typedef const nc_type_t nc_type_tc;
// ctor_dtor //
NC_API v1bit_t nc_type_ctor(nc_type_t* ref);
NC_API v1bit_t nc_type_dtor(nc_type_t* ref);
/* getters */
/* setters */
/* predicates */
/* commands */
NC_API v1bit_t nc_type_init(nc_type_t* ref);
NC_API v1bit_t nc_type_quit(nc_type_t* ref);
NC_API v1bit_t nc_type_work(nc_type_t* ref);
#   endif  /* NC_API */
/* end_of_file */
#endif // NC_TYPE_H //