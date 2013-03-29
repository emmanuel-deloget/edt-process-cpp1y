ARCHFLAGS =
CFLAGS = -std=c++11 -I./include -g -O0 $(ARCHFLAGS)
LDFLAGS = $(ARCHFLAGS)

all: ptest

ptest: ptest.o
	g++ -pthread $(LDFLAGS) -o $@ $^ $(LIBS)

%.o: src/%.cc
	g++ -pthread $(CFLAGS) -c $< -o $@

ptest.o: Makefile
ptest.o: src/ptest.cc 
ptest.o: include/process 
ptest.o: include/bits/gen_process.h 
ptest.o: include/bits/posix_process.h

clean:
	rm ptest.o ptest
