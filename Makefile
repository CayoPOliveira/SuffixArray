CFLAGS = -Wall -Werror

LIBOBJ = \
				lib/DC3.o\
				lib/MM90.o\
				lib/Naive.o\
				lib/LCP.o\

#INPUT = 


#################
all: main_sa

main_sa: main.c ${LIBOBJ}
	gcc main.c ${LIBOBJ} $(CFLAGS) -lm -o main_sa

lib/%.o(${LIBOBJ}): lib/%.c lib/%.h
	gcc $(CFLAGS) $< -o $@

clean_obj:
	rm -f lib/*.o *.o

clean:
	rm -f main_sa lib/*.o *.o

debug: main
	valgrind --leak-check=full -s ./main_sa

run:
	./main