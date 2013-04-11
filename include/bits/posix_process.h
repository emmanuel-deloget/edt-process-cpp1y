#ifndef __bits_posix_process_h_included
#define __bits_posix_process_h_included

#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

#include <sys/wait.h>
#include <unistd.h>

namespace std {

	namespace __bits {
		void __unpack_to_strings(vector<char*>& __l)
		{ __l.push_back(NULL); }

		// this private function transforms an argument to a
		// asciiz string and store the resulting string into
		// a vector
		template <typename _Arg0, typename... _Args>
		void __unpack_to_strings(vector<char*>& __l, _Arg0&& __arg0, _Args&&... __args)
		{
			stringstream __stream;
			__stream << __arg0;
			string __out = __stream.str();
			char *__s = new char[__out.length()+1];
			copy(__out.begin(), __out.end(), __s);
			__s[__out.length()] = 0;
			__l.push_back(__s);
			__unpack_to_strings(__l, __args...);
		}

		template <>
		struct __process_impl<__posix_tag>
		{
			typedef ::pid_t native_handle_type;

			static native_handle_type fetch_native_handle()
			{
				return ::getpid();
			}

			static int join(native_handle_type __h)
			{
				if (::waitpid(__h, NULL, 0) < 0)
					return ESRCH;
				return 0;
			}

			static native_handle_type fork()
			{
				return ::fork();
			}

			template <typename... _Args>
			static int exec(const string& __cmd, _Args&&... __args)
			{
				vector<char*> __p;
				__unpack_to_strings(__p, __cmd, __args...);
				int __e = ::execv(__cmd.c_str(), &__p.front());
				// we need to destroy the allocated strings or we'll leak memory
				for_each(__p.begin(), __p.end(), [](char *__s) {
					delete [] __s;
				});
				if (__e < 0)
					return errno;
				return 0;
			}

			template <typename... _Args>
			static int fork_exec(const string& __cmd, _Args&&... __args)
			{
				int __p = ::fork();
				if (__p == 0) {
				vector<char*> __p;
					__unpack_to_strings(__p, __cmd, __args...);
					int __e = ::execv(__cmd.c_str(), &__p.front());
					// we need to destroy the allocated strings or we'll leak memory
					for_each(__p.begin(), __p.end(), [](char *__s) {
						delete [] __s;
					});
				}
				return __p;
			}

			static void detach_process(native_handle_type __h)
			{
				thread([](native_handle_type ____h){
					::waitpid(____h, NULL, 0);
				}, __h).detach();
			}

		};
	}
}

#endif // __bits_posix_process_h_included
