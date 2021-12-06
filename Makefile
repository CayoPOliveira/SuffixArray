all: main

main: main.o dc3_lcp.o
		gcc main.o dc3_lcp.o -lm -o main
main.o: main.c dc3_lcp.h
		gcc -Wall -Werror -c main.c
dc3_lcp.o: dc3_lcp.c
		gcc -Wall -Werror -c dc3_lcp.c