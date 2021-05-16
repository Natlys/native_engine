#include "nc_lib_pch.hpp"
#include "nc_mem_buf.hpp"
#if (defined NC_API)
#	include "nc_mem_sys.hpp"
// ctor_dtor //
nc_mem_buf_t::nc_mem_buf_t() : nc_iop_cmp_t(), nc_mem_user_t(), m_layt(layt_t()), m_count(NC_NULL), m_data(NC_NULL) { }
nc_mem_buf_t::nc_mem_buf_t(layt_tc& layout, size_t space) : nc_mem_buf_t() { NC_CHECK(remake(layout, space), "remake error!", return); }
nc_mem_buf_t::nc_mem_buf_t(layt_tc& layout, size_t space, ptr_tc data) : nc_mem_buf_t() { NC_CHECK(remake(layout, space, data), "remake error!", return); }
nc_mem_buf_t::nc_mem_buf_t(buf_tc& copy) : nc_mem_buf_t() { operator=(copy); }
nc_mem_buf_t::nc_mem_buf_t(buf_t&& copy) : nc_mem_buf_t() { operator=(copy); }
nc_mem_buf_t::~nc_mem_buf_t() { set_count(NC_NULL); }
// setters //
nc_mem_buf_t::buf_t& nc_mem_buf_t::set_layt(layt_tc& layout) { m_layt = layout; return set_space(get_space()); }
nc_mem_buf_t::buf_t& nc_mem_buf_t::set_count(size_t count) { return set_space(count * m_layt.get_space()); }
nc_mem_buf_t::buf_t& nc_mem_buf_t::set_space(size_t space) {
	NC_CHECK(has_stride(), "no stride!", return *this);
	size_tc old_space = get_space();
	size_tc new_space = space + (space % get_stride());
	if (get_space() == new_space) { return *this; }
	size_tc cpy_space = NC_NUM_MIN(old_space, new_space);
	byte_t* old_data = get_data();
	byte_t* new_data = NC_NULL;
	if (new_space) { new_data = nc_mem_sys::get().new_arr<byte_t>(new_space); if (cpy_space && old_data) { memmove(new_data, old_data, cpy_space); } }
	if (old_space) { nc_mem_sys::get().del_arr<byte_t>(old_data, old_space); }
	m_data = new_data;
	m_count = new_space / get_stride();
	return *this;
}
nc_mem_buf_t::buf_t& nc_mem_buf_t::set_data(size_t key, ptr_tc data, size_t count) {
	NC_CHECK(data != NC_NULL, "no data!", return *this);
	memcpy(get_data(key * get_stride()), data, count * get_stride());
	return *this;
}
// operators //
nc_ostream_t& nc_mem_buf_t::operator<<(nc_ostream_t& stm) const {
	stm << "{" NC_STR_EOL;
	stm << "layout:" << get_layt() << NC_STR_EOL;
	stm << "count:" << get_count() << ";" NC_STR_EOL;
	stm << "space:" << get_space() << ";" NC_STR_EOL;
	(stm << "data:").write(reinterpret_cast<byte_tc*>(get_data()), get_space()) << ";" NC_STR_EOL;
	stm << "};" NC_STR_EOL;
	return stm;
}
nc_istream_t& nc_mem_buf_t::operator>>(nc_istream_t& stm) { return stm; }
// commands //
v1bit_t nc_mem_buf_t::remake()
{
	NC_CHECK(m_layt.remake(), "remake error!", return NC_FALSE);
	NC_CHECK(has_stride(), "no stride!", return NC_FALSE);
	NC_CHECK(has_count(), "no count!", return NC_FALSE);
	NC_CHECK(has_space(), "no space!", return NC_FALSE);
	NC_CHECK(has_data(), "no data!", return NC_FALSE);
		return NC_TRUTH;
}
#endif	// NC_API //
// end_of_file //