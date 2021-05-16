#ifndef NC_MEM_BUFFER_HPP
#	define NC_MEM_BUFFER_HPP
#	include "../nc_lib_core.hpp"
#	if (defined NC_API)
#		include "../../nc_iop/src_cpp/cmp/nc_iop_cmp.hpp"
#		include "nc_mem_layt.hpp"
#		include "nc_mem_elem.hpp"
/// memory_buffer_type
/// description:
/// --manages a buffer of bytes;
/// --allows to read and write bytes due to elements;
class nc_mem_buf_t : public nc_mem_user_t, public nc_iop_cmp_t
{
public:
	using buf_t = nc_mem_buf_t;
	using buf_tc = const buf_t;
	using elem_t = nc_mem_elem_t;
	using elem_tc = nc_mem_elem_tc;
	using layt_t = nc_mem_layt_t;
	using layt_tc = const layt_t;
	using size_t = v1u_t;
	using size_tc = v1u_tc;
public:
	// ctor_dtor //
	nc_mem_buf_t();
	nc_mem_buf_t(layt_tc& layout, size_t count);
	nc_mem_buf_t(layt_tc& layout, size_t count, ptr_tc data);
	nc_mem_buf_t(buf_tc& copy);
	nc_mem_buf_t(buf_t&& copy);
	virtual ~nc_mem_buf_t();
	// getters //
	// // --layout
	inline layt_t& get_layt()        { return m_layt; }
	inline layt_tc& get_layt() const { return m_layt; }
	// // --size
	inline size_t get_stride() const { return m_layt.get_space(); }
	inline size_t get_space() const { return m_count * m_layt.get_space(); }
	inline size_t get_count() const { return m_count; }
	// // --data
	inline byte_t* get_data()        { return &m_data[0]; }
	inline byte_tc* get_data() const { return &m_data[0]; }
	inline byte_t* get_data(size_t key)        { NC_CHECK(has_space(key), "key error!", return NC_NULL); return get_data() + key; }
	inline byte_tc* get_data(size_t key) const { NC_CHECK(has_space(key), "key error!", return NC_NULL); return get_data() + key; }
	inline byte_t* get_data(cstr_t key)        { return get_data(m_layt[key].get_offset()); }
	inline byte_tc* get_data(cstr_t key) const { return get_data(m_layt[key].get_offset()); }
	// // --elems
	inline elem_t get_elem(size_t key)        { NC_CHECK(has_count(key), "key error!", return elem_t(NC_NULL, m_layt)); return elem_t(get_data(get_stride() * key), m_layt); }
	inline elem_tc get_elem(size_t key) const { NC_CHECK(has_count(key), "key error!", return elem_tc(NC_NULL, m_layt)); return elem_tc(get_data(get_stride() * key), m_layt); }
	inline elem_t get_elem(cstr_t key)        { return elem_t(get_data(m_layt[key].get_offset()), m_layt[key]); }
	inline elem_tc get_elem(cstr_t key) const { return elem_tc(get_data(m_layt[key].get_offset()), m_layt[key]); }
	// setters //
	// // --layout
	buf_t& set_layt(layt_tc& layout);
	inline buf_t& set_layt() { return set_layt(layt_t()); }
	// // --space
	buf_t& set_count(size_t count);
	buf_t& set_space(size_t space);
	// // --data
	virtual buf_t& set_data(size_t key, ptr_tc data, size_t count);
	inline buf_t& set_data(size_t key, ptr_tc data)   { return set_data(key, data, get_count() - key); }
	inline buf_t& set_data(ptr_tc data, size_t count) { return set_data(0u, data, count); }
	inline buf_t& set_data(ptr_tc data)               { return set_data(0u, data, get_count()); }
	inline buf_t& set_data()                          { return set_data(0u, get_data(), get_count()); }
	template<typename tname> buf_t& set_elem(size_t key, const tname& data) { get_elem(key) = data; return *this; }
	template<typename tname> buf_t& set_elem(cstr_t key, const tname& data) { get_elem(key) = data; return *this; }
	// predicates //
	inline v1bit_t has_stride(size_t stride = 1u) const { return get_stride() >= stride; }
	inline v1bit_t has_space(size_t space = 1u) const { return get_space() >= space; }
	inline v1bit_t has_count(size_t count = 1u) const { return get_count() >= count; }
	inline v1bit_t has_data() const              { return get_data() != NC_NULL; }
	inline v1bit_t has_data(byte_tc* data) const { return data >= get_data() && data <= get_data(get_space()); }
	inline v1bit_t has_elem(elem_tc& elem) const { return elem.get_data() >= get_data() && elem.get_data() <= get_data(get_space()); }
	inline v1bit_t has_elem(size_t key) const    { return get_space() >= key / get_stride(); }
	inline v1bit_t has_elem(cstr_t key) const    { return get_layt().has_node(key); }
	// operators //
	// // --assignment
	inline buf_t& operator=(buf_tc& copy) { NC_CHECK(remake(copy.get_layt(), copy.get_count(), copy.get_data()), "remake error!", return *this); return *this; }
	inline buf_t& operator=(buf_t&& copy) { NC_CHECK(remake(copy.get_layt(), copy.get_count(), copy.get_data()), "remake error!", return *this); return *this; }
	// // --convertion
	inline operator elem_t ()        { return elem_t(get_data(), get_layt()); }
	inline operator elem_tc () const { return elem_tc(get_data(), get_layt()); }
	// // --accessors
	inline elem_t operator[](size_t key)        { return get_elem(key); }
	inline elem_tc operator[](size_t key) const { return get_elem(key); }
	inline elem_t operator[](cstr_t key)        { return get_elem(key); }
	inline elem_tc operator[](cstr_t key) const { return get_elem(key); }
	// // --input_output
	virtual nc_ostream_t& operator<<(nc_ostream_t& stm) const override;
	virtual nc_istream_t& operator>>(nc_istream_t& stm) override;
	// commands //
	virtual v1bit_t remake();
	inline v1bit_t remake(layt_tc& layt, size_t count) { set_layt(layt).set_count(count); return remake(); }
	inline v1bit_t remake(layt_tc& layt, size_t count, ptr_tc data) { NC_CHECK(remake(layt, count), "remake error!", return NC_FALSE); set_data(data); return NC_TRUTH; }
protected:
	layt_t m_layt;
	size_t m_count;
	byte_t* m_data;
};
#	endif	// NC_API //
// end_of_file //
#endif	// NC_MEM_BUFFER_HPP //