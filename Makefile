CC = g++
CFLAGS = -Wall -g -std=c++11

all: main

clean:
	rm -rf *o
	rm checksum

main.o:
	${CC} ${CFLAGS} -c checksum_ex.cxx

main: main.o
	${CC} ${CFLAGS} checksum_ex.o -o checksum
