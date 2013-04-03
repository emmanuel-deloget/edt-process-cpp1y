#include <sys/mman.h>
#include <unistd.h>

namespace std { namespace __bits {

	void __shmval_unmap_and_close(const char *__n, bool __o, void *__p, size_t __s)
	{
		munmap(__p, __s);
		if (__o)
			shm_unlink(__n);
	}

} }
