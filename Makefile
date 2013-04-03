OPT ?= -g -O0
ARCHFLAGS =
CFLAGS = -std=c++11 -I./include $(OPT) $(ARCHFLAGS) -D_GNU_SOURCE
LDFLAGS = $(ARCHFLAGS) -lrt -L.
CXX = g++-4.7

all: ptest

ptest: ptest.o libstd++proc.a
	$(CXX) -pthread $(LDFLAGS) -o $@ $^ $(LIBS) -l std++proc

libstd++proc.a: lib/shmval_open.o lib/shmval_close.o lib/linux_syscalls.o
	$(AR) cru $@ $^

%.o: src/%.cc
	$(CXX) -pthread $(CFLAGS) -c $< -o $@

lib/%.o: lib/%.cc
	$(CXX) -pthread $(CFLAGS) -c $< -o $@

ptest.o: Makefile
ptest.o: src/ptest.cc
ptest.o: include/process
ptest.o: include/bits/gen_process.h
ptest.o: include/bits/posix_process.h
ptest.o: include/bits/gen_all.h
ptest.o: include/bits/gen_hash.h
ptest.o: include/bits/posix_all.h
ptest.o: include/named_mutex
ptest.o: include/bits/posix_mutex.h
ptest.o: include/bits/gen_mutex.h

lib/shmval_open.o: Makefile
lib/shmval_open.o: include/bits/posix_mutex.h
lib/shmval_open.o: include/bits/gen_all.h

lib/shmval_close.o: Makefile
lib/shmval_close.o: include/bits/posix_mutex.h

clean:
	rm -f ptest.o ptest lib/*.o libstd++proc.a

