#include "nc_lib_pch.hpp"
#include "nc_mem_giver.hpp"
#if (defined NC_API)
// ctor_dtor //
nc_mem_giver_t::nc_mem_giver_t() : m_data(NC_ZERO), m_size_full(NC_ZERO), m_size_back(NC_ZERO), m_size_used(NC_ZERO) { }
nc_mem_giver_t::nc_mem_giver_t(ptr_t data, size_t size) : nc_mem_giver_t() { NC_CHECK(init(data, size), "init error!", return); }
nc_mem_giver_t::nc_mem_giver_t(giver_tc& copy) : nc_mem_giver_t() { operator=(copy); }
nc_mem_giver_t::nc_mem_giver_t(giver_t&& copy) : nc_mem_giver_t() { operator=(copy); }
nc_mem_giver_t::~nc_mem_giver_t() { NC_CHECK(!has_size_used(), "quit required!", return); }
// setters //
nc_mem_giver_t::giver_t& nc_mem_giver_t::set_size_full(size_tc size) { m_size_full = size; m_size_back = NC_ZERO; m_size_used = NC_ZERO; return *this; }
// commands //
v1bit_t nc_mem_giver_t::init()
{
	// checking //
	NC_PCALL({
		NC_CHECK(has_data() == NC_FALSE, "data must be empty!", return NC_FALSE);
		NC_CHECK(has_size_full() == NC_TRUTH, "this needs full size!", return NC_FALSE);
		NC_CHECK(has_size_free() == NC_TRUTH, "must be free!", return NC_FALSE);
	}, "init error!", return NC_FALSE);
	// actuion //
	NC_PCALL({
	}, "init error", return NC_FALSE);
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_giver_t::quit()
{
	// checking //
	NC_PCALL({
		NC_CHECK(has_data() == NC_TRUTH, "data must not be empty!", return NC_FALSE);
		NC_CHECK(has_size_full() == NC_TRUTH, "this needs full size!", return NC_FALSE);
		NC_CHECK(has_size_free() == NC_TRUTH, "must be free!", return NC_FALSE);
	}, "quit error!", return NC_FALSE);
	// actuion //
	NC_PCALL({
	}, "quit error", return NC_FALSE);
	// result //
	return NC_TRUTH;
}
#endif	// NC_API