#ifndef __bits_posix_all_h_included
#define __bits_posix_all_h_included

#include <sys/time.h>
#include <sys/types.h>

namespace std {

	namespace __bits {

		struct __posix_tag { };

		enum struct __futex_op
		{ wait, wake };

		pid_t __gettid();

		int __futex(int *__f, __futex_op __op, int __v, 
			const struct timespec *__to);

	}

	// implementation tag (itag)
	typedef __bits::__posix_tag __itag;

}

#endif // __bits_posix_all_h_included
