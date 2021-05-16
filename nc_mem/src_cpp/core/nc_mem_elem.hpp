#ifndef NC_MEM_ELEMENT_HPP
#	define NC_MEM_ELEMENT_HPP
#	include "../nc_lib_core.hpp"
#	if (defined NC_API)
#		include "../std/nc_lib_type.hpp"
#		include "nc_mem_layt.hpp"
	/// memory_element_type
class nc_mem_elem_t : public mem_user_t
{
public:
	using layt_t = nc_mem_layt_t;
	using layt_tc = const layt_t;
	using elem_t = nc_mem_elem_t;
	using elem_tc = const elem_t;
	using type_t = layt_t::type_t;
	using type_tc = layt_t::type_tc;
public:
	inline nc_mem_elem_t(byte_t* data, layt_t& layt) : mem_user_t(), m_data(data), m_layt(layt) { }
	inline ~nc_mem_elem_t() { }
	// getters //
	inline byte_t* get_data()         { return m_data; }
	inline byte_tc* get_data() const  { return m_data; }
	inline layt_t& get_layt()         { return m_layt; }
	inline layt_tc& get_layt() const  { return m_layt; }
	inline size_tc get_space() const  { return get_layt().get_space(); }
	inline size_tc get_count() const  { return get_layt().get_count(); }
	inline size_tc get_offset() const { return get_layt().get_offset(); }
	inline elem_t get_elem(v1u_tc key)        { return elem_t(m_data + m_layt[key].get_offset(), m_layt[key]); }
	inline elem_tc get_elem(v1u_tc key) const { return elem_t(m_data + m_layt[key].get_offset(), m_layt[key]); }
	inline elem_t get_elem(cstr_t key)        { return elem_t(m_data + m_layt[key].get_offset(), m_layt[key]); }
	inline elem_tc get_elem(cstr_t key) const { return elem_t(m_data + m_layt[key].get_offset(), m_layt[key]); }
	inline byte_t* get_data(type_tc type)        { NC_CHECK(get_layt().has_type(type), "type error!", return NC_NULL); return get_data(); }
	inline byte_tc* get_data(type_tc type) const { NC_CHECK(get_layt().has_type(type), "type error!", return NC_NULL); return get_data(); }
	template<typename tname> tname* get_data()             { return reinterpret_cast<tname*>(get_data(type_info_t::get_type<tname>())); }
	template<typename tname> const tname* get_data() const { return reinterpret_cast<const tname*>(get_data(type_info_t::get_type<tname>())); }
	template<typename tname> tname& get()             { return *reinterpret_cast<tname*>(get_data(type_info_t::get_type<tname>())); }
	template<typename tname> const tname& get() const { return *reinterpret_cast<const tname*>(get_data(type_info_t::get_type<tname>())); }
	// setters //
	inline v1nil_t set_data(ptr_tc buffer)               { memcpy(get_data(), buffer, get_space()); }
	inline v1nil_t set_data(ptr_tc buffer, type_tc type) { memcpy(get_data(type), buffer, get_space()); }
	template<typename tname> v1nil_t set_data(const tname* buffer) { set_data(buffer, type_info_t::get_type<tname>()); }
	template<typename tname> v1nil_t set(const tname& buffer)      { set_data(&buffer, type_info_t::get_type<tname>()); }
	template<typename tname>
	v1nil_t set_elem(v1u_tc key, tname&& buffer) { get_elem(key).set<tname>(std::forward<tname>(buffer)); }
	template<typename tfirst, typename ... trest>
	v1nil_t set_elem(v1u_tc key, tfirst&& first, trest&& ... rest) {
		set_elem(key, std::forward<tfirst>(first));
		set_elem(key + 1u, std::forward<trest>(rest)...);
	}
	// operators //
	template<typename tname> operator tname* ()             { return get_data<tname>(); }
	template<typename tname> operator const tname* () const { return get_data<tname>(); }
	template<typename tname> operator tname& ()             { return get<tname>(); }
	template<typename tname> operator const tname& () const { return get<tname>(); }
	template<typename tname> inline v1nil_t operator=(const tname* buffer) { set_data<tname>(buffer); }
	template<typename tname> inline v1nil_t operator=(const tname& buffer) { set<tname>(buffer); }
	inline elem_t operator[](v1u_tc key)        { return get_elem(key); }
	inline elem_tc operator[](v1u_tc key) const { return get_elem(key); }
	inline elem_t operator[](cstr_t key)        { return get_elem(key); }
	inline elem_tc operator[](cstr_t key) const { return get_elem(key); }
private:
	byte_t* m_data;
	layt_t& m_layt;
};
/// memory_element_type_constant
class nc_mem_elem_tc
{
public:
	using layt_t = nc_mem_layt_t;
	using layt_tc = const layt_t;
	using elem_t = nc_mem_elem_tc;
	using elem_tc = const elem_t;
	using vtype_t = layt_t::type_t;
	using vtype_tc = layt_t::type_tc;
public:
	// ctor_dtor //
	inline nc_mem_elem_tc(byte_tc* data, layt_tc& layt) : m_data(data), m_layt(layt) { }
	inline ~nc_mem_elem_tc() {}
	// getters //
	inline byte_tc* get_data() const { return m_data; }
	inline layt_tc& get_layt() const { return m_layt; }
	inline size_tc get_space() const { return get_layt().get_space(); }
	inline size_tc get_count() const { return get_layt().get_count(); }
	inline size_tc get_offset() const { return get_layt().get_offset(); }
	inline elem_tc get_elem(v1u_tc key) const { return elem_t(m_data + get_layt()[key].get_offset(), m_layt); }
	inline elem_tc get_elem(cstr_t key) const { return elem_t(m_data + get_layt()[key].get_offset(), m_layt); }
	inline byte_tc* get_data(vtype_tc type) const { NC_CHECK(get_layt().has_type(type), "type error!", return NC_NULL); return get_data(); }
	template<typename tname> const tname* get_data() const { return reinterpret_cast<const tname*>(get_data(type_info_t::get_type<tname>())); }
	template<typename tname> const tname& get() const { return *reinterpret_cast<const tname*>(get_data(type_info_t::get_type<tname>())); }
	// operators //
	template<typename tname> operator const tname* () const { return get_data<tname>(); }
	template<typename tname> operator const tname& () const { return get<tname>(); }
	inline elem_tc operator[](v1u_tc key) const { return get_elem(key); }
	inline elem_tc operator[](cstr_t key) const { return get_elem(key); }
private:
	byte_tc* m_data;
	layt_tc& m_layt;
};
#	endif	// NC_API
// end_of_file //
#endif	// NC_MEM_ELEMENT_HPP