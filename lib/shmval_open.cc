#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include <bits/gen_all.h>

namespace std { namespace __bits {

	void *__shmval_open_and_map(const char *__n, size_t __s, bool& __o)
	{
		bool __own = false;
		void *__pval;
		
		int __fd = shm_open(__n, O_RDWR|O_CREAT|O_EXCL|O_TRUNC, 0600);
		if (__fd > 0) {
			__own = true;
		} else {
			__fd = shm_open(__n, O_RDWR, 0600);
		}
		if (__fd < 0)
			__throw(errno, "cannot create space to store the named_mutex");

		if (__own && ftruncate(__fd, __s) < 0) {
			int __esave = errno;
			close(__fd);
			shm_unlink(__n);
			__throw(__esave, "cannot grow space to store the named_mutex");
		}

		__pval = mmap(0, __s, PROT_READ|PROT_WRITE, MAP_SHARED, __fd, 0);
		if (!__pval) {
			int __esave = errno;
			close(__fd);
			if (__own)
				shm_unlink(__n);
			__throw(__esave, "cannot grow space to store the named_mutex");
		}
		// we can close the __fd: it's not needed anymore
		close(__fd);
		
		__o = __own;

		return __pval;
	}

} }