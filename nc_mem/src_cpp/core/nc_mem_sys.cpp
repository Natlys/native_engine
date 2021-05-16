#include "nc_lib_pch.hpp"
#include "nc_mem_sys.hpp"
#if (defined NC_API)
nc_mem_sys::nc_mem_sys() : nc_system_tt(), nc_mem_arena_t() { }
nc_mem_sys::~nc_mem_sys() { NC_CHECK(!has_data() && !has_size_full(), "no quit!", return); }
// commands //
v1bit_t nc_mem_sys::init()
{
	NC_PCALL({ // checking //
		NC_CHECK(!has_data(), "init is already done!!", return NC_FALSE);
		NC_CHECK(has_size_full(), "no size!", return NC_FALSE);
	}, "init error!", return NC_FALSE);
	NC_PCALL({ // action //
		NC_CHECK(NC_MEM_TAKE(m_data, get_size_full()), "memory error!", return NC_FALSE);
		NC_CHECK(nc_system_tt::init(), "init error", return NC_FALSE);
		NC_CHECK(nc_mem_arena_t::init(), "init error!", return NC_FALSE);
	}, "init error!", return NC_FALSE);
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_sys::quit()
{
	// checking //
	NC_PCALL({
		NC_CHECK(has_data(), "quit is already done!", return NC_FALSE);
		NC_CHECK(has_size_full(), "no size!", return NC_FALSE);
	}, "quit error!", return NC_FALSE);
	// action //
	NC_PCALL({
		NC_CHECK(NC_MEM_FREE(m_data, get_size_full()), "memory error!", return NC_FALSE);
		delete[] get_data(); set_data(NC_NULL);
		NC_CHECK(nc_system_tt::quit(), "quit error", return NC_FALSE);
		NC_CHECK(nc_mem_arena_t::quit(), "quit error", return NC_FALSE);
	}, "quit error!", return NC_FALSE);
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_sys::work() {
	NC_PCALL({ // checking //
		NC_CHECK(nc_system_tt::work(), "work error", return NC_FALSE);
	}, "work error!", return NC_FALSE);
	// result //
	return NC_TRUTH;
}
#endif	// NC_API