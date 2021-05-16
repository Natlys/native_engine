#ifndef NC_MEM_HEAP_HPP
#	define NC_MEM_HEAP_HPP
#	include "../nc_lib_core.hpp"
#	if (defined NC_API)
#		include "nc_mem_giver.hpp"
class nc_mem_heap_t : public nc_mem_giver_t
{
public:
public:
	// ctor_dtor //
	nc_mem_heap_t();
	virtual ~nc_mem_heap_t();
	// getters //
	// setters //
	// commands //
	virtual v1bit_t init() override;
	virtual v1bit_t quit() override;
	virtual v1bit_t take(nil_t** data, size_t size, size_t alig) override;
	virtual v1bit_t free(nil_t** data, size_t size, size_t alig) override;
	static inline v1bit_t static_take(nil_t** data, size_t size, size_t alig) {
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "cannot align to zero!", return NC_FALSE);
		size = NC_ALIGN(size, alig);
		NC_MEM_TAKE(data, size);
		return NC_TRUTH;
	}
	static inline v1bit_t static_free(nil_t** data, size_t size, size_t alig) {
		NC_CHECK(data != NC_NULL, "no data!", return NC_FALSE);
		NC_CHECK(size != NC_ZERO, "no size!", return NC_FALSE);
		NC_CHECK(alig != NC_ZERO, "cannot align to zero!", return NC_FALSE);
		size = NC_ALIGN(size, alig);
		NC_MEM_FREE(data, NC_ALIGN(size, alig));
		return NC_TRUTH;
	}
	// operators //
private:
};
#	endif	// NC_API //
#endif	// NC_MEM_HEAP_HPP //