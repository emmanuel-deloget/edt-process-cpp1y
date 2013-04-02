#ifndef __gen_hash_h_included
#define __gen_hash_h_included

/*
 * Hash functions
 * 
 * These hash functions are based upon FNV-1, which is described
 * at http://www.isthe.com/chongo/tech/comp/fnv/
 * 
 * This is not a cryptographic hash, but it should give us good
 * enough results for our use case (which is mapping a 16 bit value
 * on a size_t). 
 */

namespace std {
	namespace __details {
		template <size_t _Size>
		struct __fnv1_gen_hash_impl;
		
		template <size_t _Prime, size_t _Offset>
		struct __fnv1_gen_hash_base
		{ 
			size_t operator()(const void *__p, size_t __s)
			{
				size_t __hash = _Offset;
				const unsigned char *__pc = static_cast<const unsigned char*>(__p);
				for (size_t __n = 0; __n < __s; ++__n) {
					__hash ^= static_cast<size_t>(__pc[__n]);
					__hash *= _Prime;
				}
				return __hash;
			}
		};

#if defined(__LP64__)
		// the values for __prime and __offset comes from
		// http://www.isthe.com/chongo/tech/comp/fnv/
		template <>
		struct __fnv1_gen_hash_impl<sizeof(size_t)>: 
			__fnv1_gen_hash_base<
				1099511628211ULL,
				14695981039346656037ULL
			>
		{ };
#else
		// the values for _Prime and _Offset comes from
		// http://www.isthe.com/chongo/tech/comp/fnv/
		template <>
		struct __fnv1_gen_hash_impl<sizeof(size_t)>: 
			__fnv1_gen_hash_base<
				16777619UL,
				2166136261UL
			>
		{ };
#endif

		using __fnv1_hash_bytes = __fnv1_gen_hash_impl<sizeof(size_t)>;
	}
}

#endif // __gen_hash_h_included
