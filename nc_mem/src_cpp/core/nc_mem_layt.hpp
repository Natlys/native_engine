#ifndef NC_MEM_LAYOUT_HPP
#	define NC_MEM_LAYOUT_HPP
#	include "../nc_lib_core.hpp"
		#if (defined NC_API)
#		include "../../nc_iop/src_cpp/cmp/nc_iop_cmp.hpp"
#		include "../std/nc_lib_tree.hpp"
#		include "nc_mem_ref.hpp"
/// memory_layout_type
/// description:
/// --this is a data structure which allows to:
/// construct tables out of elements;
/// describe any format i like;
/// set bytes of a given buffer;
class nc_mem_layt_t : public nc_tree_node_t<nc_mem_layt_t>, public nc_mem_user_t, public nc_iop_cmp_t
{
public:
	using layt_t = nc_mem_layt_t;
	using layt_tc = const layt_t;
	using tree_t = nc_tree_node_t<nc_mem_layt_t>;
	using tree_tc = const tree_t;
	using elem_t = nc_mem_layt_t;
	using elem_tc = const elem_t;
	using elems_t = tree_t::nodes_t;
	using elems_tc = tree_t::nodes_tc;
	using elem_list_t = tree_t::node_list_t;
	using elem_list_tc = tree_t::node_list_tc;
public:
	// ctor_dtor //
	inline nc_mem_layt_t() : nc_tree_node_t(), nc_mem_user_t(), nc_iop_cmp_t(), m_space(nc_type_info_t::get<tree_t>().m_size), m_offset(NC_NULL) { }
	inline nc_mem_layt_t(cstr_t key)                      : nc_mem_layt_t() { set_name(key); }
	inline nc_mem_layt_t(cstr_t key, elems_tc& elems)     : nc_mem_layt_t() { set_name(key); set_nodes(elems); }
	inline nc_mem_layt_t(elems_tc& elems)                 : nc_mem_layt_t() { set_nodes(elems); }
	inline nc_mem_layt_t(cstr_t key, elem_list_tc& elems) : nc_mem_layt_t() { set_name(key); set_nodes(elems); }
	inline nc_mem_layt_t(elem_list_tc& elems)             : nc_mem_layt_t() { set_nodes(elems); }
	inline nc_mem_layt_t(cstr_t key, type_tc type)        : nc_mem_layt_t() { set_name(key); set_type(type); }
	inline nc_mem_layt_t(type_tc type)                             : nc_mem_layt_t() { set_type(type); }
	inline nc_mem_layt_t(cstr_t key, type_tc type, size_tc offset) : nc_mem_layt_t() { set_name(key); set_type(type); set_offset(offset); }
	inline nc_mem_layt_t(type_tc type, size_tc offset)             : nc_mem_layt_t() { set_type(type); set_offset(offset); }
	inline nc_mem_layt_t(layt_tc& copy) : nc_mem_layt_t() { operator=(copy); }
	inline nc_mem_layt_t(layt_t&& copy) : nc_mem_layt_t() { operator=(copy); }
	inline ~nc_mem_layt_t() { }
	// getters //
	inline size_tc get_space() const  { return m_space; }
	inline size_tc get_offset() const { return m_offset; }
	// setters //
	inline layt_t& set_offset(size_tc offset) { m_offset = offset; return *this; }
	// predicates //
	inline v1bit_t has_offset(size_t offset = 1u) const { return m_offset >= offset; };
	inline v1bit_t has_space(size_t space = 1u) const { return m_space >= space; };
	// commands //
	inline v1bit_t remake() {
		if (is_leaf()) { m_space = get_type_size(); }
		else if (is_tree()) {
			//NC_CHECK(has_node(), "no leafs", return NC_FALSE);
			m_space = NC_NULL;
			for (auto& ielem : m_nodes) {
				NC_CHECK(ielem.remake(m_space + m_offset), "failed to remake", return NC_FALSE);
				m_space += ielem.get_space();
			}
		}
		else { NC_ERROR("type error!", return NC_FALSE); }
		return NC_TRUTH;
	}
	inline v1bit_t remake(size_tc offset) { set_offset(offset); return remake(); }
	// operators //
	inline layt_t& operator=(layt_tc& copy) { nc_tree_node_t::operator=(copy); NC_CHECK(remake(copy.m_offset), "remake error!", return *this); return *this; }
	inline layt_t& operator=(layt_t&& copy) { nc_tree_node_t::operator=(copy); NC_CHECK(remake(copy.m_offset), "remake error!", return *this); return *this; }
	virtual inline nc_ostream_t& operator<<(nc_ostream_t& stm) const override {
		stm << "{";
		stm << "name:" << get_name() << ";";
		stm << "type:" << get_type() << ";";
		stm << "space:" << get_space() << ";";
		stm << "offset:" << get_offset() << ";";
		stm << "count:" << get_count() << ";";
		size_t itr = 0u; for (auto& inode : get_nodes()) { stm << "elem[" << itr++ << "]:" << inode; }
		stm << "}" << ";";
		return stm;
	}
	virtual inline nc_istream_t& operator>>(nc_istream_t& stm) override {
		return stm;
	}
protected:
	size_t m_space;
	size_t m_offset;
};
/// --memory_layout_templated_type
template<typename tname>
class nc_mem_layt_tt : public nc_mem_layt_t
{
public:
public:
	// ctor_dtor //
	inline nc_mem_layt_tt() : nc_mem_layt_t(type_info_t::get_type<tname>()) { }
	inline nc_mem_layt_tt(cstr_t key) : nc_mem_layt_t(key, type_info_t::get_type<tname>()) { }
	inline ~nc_mem_layt_tt() { }
	// getters //
	// setters //
	// predicates //
	// commands //
	// operators //
protected:
};
#	endif	// NC_API //
// end_of_file //
#endif	// NC_MEM_LAYOUT_HPP