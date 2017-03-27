#pragma once

#if defined(_MSC_VER)
	#include <intrin.h>
#endif


namespace sltbench {
	
namespace priv {
	
void empty_function(volatile const char*);
 
} // namespace priv

#if defined(__GNUC__)
	#define SLT_FORCE_INLINE __attribute__((always_inline))
#elif defined(_MSC_VER) && !defined(__clang__)
	#define SLT_FORCE_INLINE __forceinline
#else
	#define SLT_FORCE_INLINE
#endif
	
// The DoNotOptimize function prevents a value from being optimized.
#if defined(__GNUC__)
	template <class T>
	inline SLT_FORCE_INLINE void DoNotOptimize(const T& v)
	{
		asm volatile("" : : "g"(v) : "memory");
	}
#elif defined(_MSC_VER)
	template <class T>
	inline SLT_FORCE_INLINE void DoNotOptimize(const T& v)
	{
		// TODO: add dno to doc
		priv::empty_function(reinterpret_cast<volatile const char*>(&v));
		_ReadWriteBarrier();
	}
#else
	template <class T>
	inline SLT_FORCE_INLINE void DoNotOptimize(const T& v)
	{
		priv::empty_function(reinterpret_cast<volatile const char*>(&v));
	}
#endif

#undef SLT_FORCE_INLINE
	
} // namespace sltbench