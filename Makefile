CFLAGS = -Wall -Werror -lm -lrt -ldl

LIBOBJ = \
				lib/DC3.o\
				lib/MM90.o\
				lib/Naive.o\
				lib/LCP.o\

EXTERNALOBJ = \
				external/SUS/lib/utils.o\
				external/SUS/external/malloc_count/malloc_count.o\

#INPUT = 


#################
all: main_sa

main_sa: main.c ${LIBOBJ} ${EXTERNALOBJ}
	gcc main.c ${LIBOBJ} ${EXTERNALOBJ} $(CFLAGS) -o main_sa

lib/%.o: lib/%.c lib/%.h
	gcc $(CFLAGS) $< -c -o $@

external/%.o: external/%.c external/%.h
	gcc $(CFLAGS) $< -c -o $@

clean_obj:
	rm -f ${LIBOBJ} $(EXTERNALOBJ) *.o

clean:
	rm -f main_sa ${LIBOBJ} $(EXTERNALOBJ) *.o

debug: main
	valgrind --leak-check=full -s ./main_sa

run:
	./main