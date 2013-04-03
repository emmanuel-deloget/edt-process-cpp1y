#include <sys/time.h>
#include <linux/unistd.h>
#include <linux/futex.h>
#include <sys/syscall.h>
#include <errno.h>
#include <unistd.h>

#include <bits/gen_all.h>
#include <bits/posix_all.h>

namespace std { namespace __bits {

	pid_t __gettid()
	{ 
		return syscall(__NR_gettid); 
	}
	
	int __futex(int *__f, __futex_op __op, int __v, const struct timespec *__to)
	{ 
		int __fop; 
		
		switch (__op) {
		case __futex_op::wait:
			__fop = FUTEX_WAIT;
			break;
		case __futex_op::wake:
			__fop = FUTEX_WAKE;
			break;
		default:
			__throw(EINVAL, "invalid futex operation");
		}
		
		return syscall(__NR_futex, __f, __fop, __v, __to); 
	}

} }
