ARCHFLAGS =
CFLAGS = -std=c++11 -I./include -g -O0 $(ARCHFLAGS) -D_GNU_SOURCE
LDFLAGS = $(ARCHFLAGS) -lrt
CXX = g++-4.7

all: ptest

ptest: ptest.o
	$(CXX) -pthread $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: src/%.cc
	$(CXX) -pthread $(CFLAGS) -c $< -o $@

ptest.o: Makefile
ptest.o: src/ptest.cc
ptest.o: include/process
ptest.o: include/bits/gen_process.h
ptest.o: include/bits/posix_process.h
ptest.o: include/bits/gen_all.h
ptest.o: include/bits/gen_hash.h
ptest.o: include/bits/posix_all.h

clean:
	rm ptest.o ptest
