#ifndef __bits_gen_all_h_included
#define __bits_gen_all_h_included

#include <system_error>

namespace std {

	namespace __bits {
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
#endif // __bits_gen_all_h_included
