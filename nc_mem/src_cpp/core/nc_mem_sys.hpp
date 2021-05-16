#ifndef NC_MEM_SYSTEM_HPP
#	define NC_MEM_SYSTEM_HPP
#	include "../nc_mem_core.hpp"
#	if (defined NC_API)
#		include "nc_mem_arena.hpp"
/// memory_system_type
class nc_mem_sys : public nc_system_tt<nc_mem_sys>, public nc_mem_arena_t
{
public:
public:
	// ctor_dtor //
	nc_mem_sys();
	~nc_mem_sys();
	// getters //
	// setters //
	// predicates //
	// commands //
	inline v1bit_t init(size_tc size) { set_size_full(size); return init(); }
	v1bit_t init();
	v1bit_t quit();
	v1bit_t work();
	static inline v1bit_t static_take(nil_t** data, size_t size, size_t alig) { return nc_mem_sys::get().take(data, size, alig); }
	static inline v1bit_t static_free(nil_t** data, size_t size, size_t alig) { return nc_mem_sys::get().free(data, size, alig); }
	// operators //
};
/// memory_user_type
/// description:
/// --similarly to nc_mem_user_tt uses custom allocator for new and delete;
/// --allocator is always nc_mem_sys
class nc_mem_user_t
{
public:
public:
	// ctor_dtor //
	inline nc_mem_user_t() = default;
	inline ~nc_mem_user_t() = default;
	// getters //
	// setters //
	// predicates //
	// operators //
	inline ptr_t operator new(size_t size, ptr_t data) { return ::operator new(size, data); }
	inline ptr_t operator new(size_t size)   { ptr_t data; nc_mem_sys::static_take(&data, size, NC_MEM_DEFAULT_SIZE_CELL); return data; }
	inline ptr_t operator new[](size_t size) { ptr_t data; nc_mem_sys::static_take(&data, size, NC_MEM_DEFAULT_SIZE_CELL); return data; }
	inline v1nil_t operator delete(ptr_t data, size_t size)   { nc_mem_sys::static_free(&data, size, NC_MEM_DEFAULT_SIZE_CELL); }
	inline v1nil_t operator delete[](ptr_t data, size_t size) { nc_mem_sys::static_free(&data, size, NC_MEM_DEFAULT_SIZE_CELL); }
	// commands //
};
#	endif	// NC_API //
// end_of_file //
#endif	// NC_MEM_SYSTEM_HPP //