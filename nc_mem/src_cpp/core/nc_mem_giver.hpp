#ifndef NC_MEM_GIVER_HPP
#	define NC_MEM_GIVER_HPP
#	include "../nc_mem_core.hpp"
#	if (defined NC_API)
// includes //
#   	include <new>
// types //
/// memory_giver_type
/// description:
class nc_mem_giver_t
{
public:
	using giver_t = nc_mem_giver_t;
	using giver_tc = const giver_t;
public:
	// ctor_dtor //
	nc_mem_giver_t();
	nc_mem_giver_t(ptr_t data, size_t size);
	nc_mem_giver_t(giver_tc& copy);
	nc_mem_giver_t(giver_t&& copy);
	virtual ~nc_mem_giver_t();
	// getters //
	inline ptr_t get_data()        { return m_data; }
	inline ptr_tc get_data() const { return m_data; }
	inline ptr_t get_data(size_tc offset)        { return &m_data[offset]; }
	inline ptr_tc get_data(size_tc offset) const { return &m_data[offset]; }
	inline size_tc get_size_full() const { return m_size_full; }
	inline size_tc get_size_back() const { return m_size_back; }
	inline size_tc get_size_used() const { return m_size_used; }
	inline size_tc get_size_free() const { return m_size_full - m_size_used; }
	// setters //
	giver_t& set_data(ptr_t data);
	giver_t& set_size_full(size_tc size);
	// predicates //
	inline v1bit_t has_data() const            { return m_data != NC_NULL; }
	inline v1bit_t has_data(ptr_tc data) const { return (data >= &m_data[0]) && (data <= &m_data[m_size_full]); }
	inline v1bit_t has_size_full(size_tc size = 1u) const { return (m_size_full) >= size; }
	inline v1bit_t has_size_back(size_tc size = 1u) const { return (m_size_full - m_size_back) >= size; }
	inline v1bit_t has_size_used(size_tc size = 1u) const { return (m_size_used) >= size; }
	inline v1bit_t has_size_free(size_tc size = 1u) const { return (m_size_full - m_size_used) >= size; }
	// commands //
	inline v1bit_t init(ptr_t data, size_tc size) { set_data(data); set_size_full(size); return init(); }
	virtual v1bit_t init();
	virtual v1bit_t quit();
	virtual v1bit_t take(nil_t** data, size_t size, size_t align) = 0;
	virtual v1bit_t free(nil_t** data, size_t size, size_t align) = 0;
	template<typename tname> v1bit_t take(tname** data, size_t numb) { return this->take(data, numb * sizeof(tname), alignof(tname)); }
	template<typename tname> v1bit_t free(tname** data, size_t numb) { return this->free(data, numb * sizeof(tname), alignof(tname)); } 
	template<typename tname, typename ... targs> v1bit_t new_one(tname** data, targs&& ... args) {
		NC_CHECK(this->take<tname>(data, 1u), "memory error!", return NC_FALSE);
		new(*data)tname(std::forward<targs>(args)...);
		return NC_TRUTH;
	}
	template<typename tname, typename ... targs> v1bit_t new_arr(tname** data, size_t numb, targs&& ... args) {
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(numb != NC_ZERO, "no size!", return NC_FALSE);
		NC_CHECK(this->take<tname>(data, numb), "memory error!", return NC_FALSE);
		for (tname* itr = *data, end = &(*data)[numb]; itr < end; itr++) { new(itr)tname(std::forward<targs>(args)...); }
		return NC_TRUTH;
	}
	template<typename tname> v1bit_t del_one(tname** data) {
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		(*data)->~tname();
		return this->free<tname>(data, 1u);
	}
	template <typename tname> v1bit_t del_arr(tname** data, size_t numb) {
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(numb != NC_ZERO, "no size!", return NC_FALSE);
		for (tname* itr = *data, end = &(*data)[numb]; itr < end; itr++) { itr->~tname(); }
		return this->free<tname>(data, numb);
	}
	// operators //
	inline giver_t& operator=(giver_tc& copy) { NC_CHECK(init(copy.m_data, copy.m_size_full), "init error!", NC_DEFAULT); return *this; }
	inline giver_t& operator=(giver_t&& copy) { NC_CHECK(init(copy.m_data, copy.m_size_full), "init error!", NC_DEFAULT); return *this; }
protected:
	data_t m_data; // chunck of bytes;
	size_t m_size_full;  // current chunk space;
	size_t m_size_back;  // maximally used space;
	size_t m_size_used;  // currently used space;
};
#endif	// NC_API //
// end_of_file //
#endif	// NC_MEM_GIVER_HPP //