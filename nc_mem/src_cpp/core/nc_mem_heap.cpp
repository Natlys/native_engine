#include "nc_lib_pch.hpp"
#include "nc_mem_heap.hpp"
#if (defined NC_API)
// ctor_dtor //
nc_mem_heap_t::nc_mem_heap_t() { }
nc_mem_heap_t::~nc_mem_heap_t() { }
// commands //
v1bit_t nc_mem_heap_t::init()
{
	// checking //
	NC_CHECK(has_data(), "this allocator must use only heap!", return NC_FALSE);
	NC_CHECK(has_size_full(), "this allocator must use only heap!", return NC_FALSE);
	// action //
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_heap_t::quit()
{
	// checking //
	NC_CHECK(has_data(), "this allocator must use only heap!", return NC_FALSE);
	NC_CHECK(has_size_full(), "this allocator must use only heap!", return NC_FALSE);
	// action //
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_heap_t::take(nil_t** data, size_t size, size_t alig)
{
	NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
	NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
	NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
	size = NC_ALIGN(size, alig);
	m_size_used += size;
	m_size_back = NC_MAX(m_size_back, m_size_used);
	return NC_TRUTH;
}
v1bit_t nc_mem_heap_t::free(nil_t** data, size_t size, size_t alig)
{
	NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
	NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
	NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
	size = NC_ALIGN(size, alig);
	m_size_used -= size;
	m_size_back = NC_MAX(m_size_back, m_size_used);
	NC_MEM_FREE(data, size);
}
#endif	// NC_API