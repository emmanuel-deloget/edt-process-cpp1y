#ifndef __bits_gen_all_h_included
#define __bits_gen_all_h_included

#include <system_error>

namespace std {

	namespace __bits {
		inline void __throw(int __e, const char* __w)
		{
			if (__e) {
				errc __r = static_cast<errc>(__e);
				throw system_error(make_error_code(__r), __w);
			}
		}

		inline void __throw(errc __ec, const char *__w)
		{
			throw system_error(make_error_code(__ec), __w);
		}

		inline void __do_exit(int __e) noexcept
		{
			exit(__e);
		}

		template <class _C0, class _C1>
		struct __exec
		{
			static int invoke(_C1 __c)
			{
				__c();
				return 0;
			}
		};

		template <class _C1>
		struct __exec<int, _C1>
		{
			static int invoke(_C1 __c)
			{
				return __c();
			}
		};

	}

}
#endif // __bits_gen_all_h_included
