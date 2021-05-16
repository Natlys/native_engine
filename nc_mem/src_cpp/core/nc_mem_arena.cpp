#include "../nc_mem_pch.hpp"
#include "nc_mem_arena.hpp"
#if (defined NC_API)
// ctor_dtor //
nc_mem_arena_t::nc_mem_arena_t() : nc_mem_giver_t(), m_free_list(NC_NULL) { }
nc_mem_arena_t::nc_mem_arena_t(ptr_t data, size_t size) : nc_mem_giver_t(data, size), m_free_list(NC_NULL) { }
nc_mem_arena_t::~nc_mem_arena_t() { }
// commands //
v1bit_t nc_mem_arena_t::init()
{
	// checking //
	NC_CHECK(nc_mem_giver_t::init(), "init error!", return NC_FALSE);
	// action //
	m_free_list = NC_NULL;
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_arena_t::quit()
{
	// checking //
	NC_CHECK(nc_mem_giver_t::quit(), "quit error!", return NC_FALSE);
	// action //
	m_free_list = NC_NULL;
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_arena_t::take(nil_t** data, size_t size, size_t alig) {
	NC_PCALL({ // checking //
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
		NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
	}, "memory error!", return NC_FALSE);
	NC_PCALL({ // action //
		*data = NC_NULL;
		// 100% essential thing for this allocator!;
		alig = NC_NUM_MAX(alig, NC_MEM_DEFAULT_CELL_SIZE);
		size = NC_ALIGN(size, alig);
		NC_CHECK(has_size_free(size), "not sufficient arena size!", return NC_FALSE);
		if (m_free_list != NC_NULL) { // free list allocation is possible;
			if ((*data = m_free_list->get_link(size)) != NC_NULL) { // free list search;
				if (m_free_list->m_size == NC_ZERO) { // no more space; advance the head;
					m_free_list = m_free_list->m_link;
				}
			}
		}
		if (*data == NC_NULL) { // failed to get free_list block; default allocation with back-pointer advance;
			NC_CHECK(has_size_back(size), "not sufficient arena size!", return NC_FALSE);
			*data = get_data(get_size_back());
			m_size_back += size;
		}
	}, "memory error!", return NC_FALSE);
	m_size_used += size;
	// result //
	return NC_TRUTH;
}
v1bit_t nc_mem_arena_t::free(nil_t** data, size_t size, size_t alig) {
	// checking //
	NC_PCALL({ // action //
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "cannot align to zero!", return NC_FALSE);
		NC_CHECK(has_data(*data), "this data does not belong this arena!", return NC_FALSE);
	}, "memory error!", return NC_FALSE);
	NC_PCALL({ // action //
		// 100% essential thing for this allocator!;
		alig = NC_MAX(alig, NC_MEM_DEFAULT_CELL_SIZE);
		size = NC_ALIGN(size, alig);
		NC_CHECK(has_size_used(size), "too much bytes to delete!", return NC_FALSE);
		// memset(data, NC_NULL, size); dangerous to use because it can erase our free_list;
		nc_mem_link_t* next_free_list = new(data)nc_mem_link_t();
		next_free_list->m_link = m_free_list;
		next_free_list->m_size = size;
		m_free_list = next_free_list;
	}, "memory error!", return NC_FALSE);
	// result //
	// m_size_back -= size; it should not be changed back!
	m_size_used -= size;
	return NC_TRUTH;
}
// end_of_file //
#endif	// NC_API //