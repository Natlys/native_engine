#ifndef NC_MEM_ARENA_HPP
#	define NC_MEM_ARENA_HPP
#	include "../nc_mem_core.hpp"
#	if (defined NC_API)
#	include "nc_mem_giver.hpp"
/// memory_link_type
struct nc_mem_link_t
{
	using link_t = nc_mem_link_t;
	using link_tc = const link_t;
public:
	link_t* m_link;
	size_t m_size;
public:
	// ctor_dtor //
	inline nc_mem_link_t() = default;
	inline ~nc_mem_link_t() = default;
	// getters //
	inline link_t* get_link(size_t size) {
		link_t* data = NC_NULL;
		if (m_link != NC_NULL) { // try own links;
			if (data = m_link->get_link(size)) {
				if (m_link->m_size == NC_ZERO) { // no more space; maybe it is going to be used;
					m_link = m_link->m_link;
				}
			}
		}
		if (data == NC_NULL && this->has_size(size)) { // no one link can help; try own storage;
			data = this;
			m_size -= size;
		}
		return data;
	}
	// setters //
	// predicates //
	inline v1bit_t has_size(size_t size) const { return m_size == size; }
	inline v1bit_t has_link(size_t size) const { if (m_link) { return m_link->has_link(size); } return has_size(size); }
	// commands //
	// operators //
	template <typename tname> operator tname* () { return reinterpret_cast<tname*>(this); }
	template <typename tname> operator const tname* () const { return reinterpret_cast<const tname*>(this); }
	template <typename tname> operator tname& () { return *reinterpret_cast<tname*>(this); }
	template <typename tname> operator const tname& () const { return *reinterpret_cast<const tname*>(this); }
};
/// memory_giver_arena_type
/// description:
/// --just a chunk of bytes works with ptr and char* pointers
class nc_mem_arena_t : public nc_mem_giver_t
{
public:
	// ctor_dtor //
	nc_mem_arena_t();
	nc_mem_arena_t(ptr_t data, size_t size);
	virtual ~nc_mem_arena_t();
	// getters //
	// setters //
	// predicates //
	// commands //
	virtual v1bit_t init() override;
	virtual v1bit_t quit() override;
	virtual v1bit_t take(nil_t** data, size_t size, size_t alig) override;
	virtual v1bit_t free(nil_t** data, size_t size, size_t alig) override;
	// operators //
private:
	nc_mem_link_t* m_free_list;
};
#	endif	// NC_API //
// end_of_file //
#endif	// NC_MEM_ARENA_HPP //