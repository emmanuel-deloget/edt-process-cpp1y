#ifndef bits_gen_process_h_included
#define bits_gen_process_h_included

#include <system_error>

namespace std {

	namespace __bits {
		template <typename _Tag>
		struct __process_impl;

		void __throw(int __e, const char* __w)
		{
			if (__e) {
				errc __r = static_cast<errc>(__e);
				throw system_error(make_error_code(__r), __w);
			}
		}
		
		void __throw(errc __ec, const char *__w)
		{
			throw system_error(make_error_code(__ec), __w);
		}
	}

}

#endif // bits_gen_process_h_included
