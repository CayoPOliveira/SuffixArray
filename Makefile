LIBOBJ = \
				lib/DC3.o\
				lib/MM90.o\
				lib/Naive.o\
				lib/LCP.o\

#INPUT = 


#################
all: main

main: main.c ${LIBOBJ}
	gcc -Wall -Werror main.c ${LIBOBJ} -lm -o main

lib/%.o(${LIBOBJ}): lib/%.c lib/%.h
	gcc -Wall -Werror $< -o $@

clean_obj:
	rm -f lib/*.o *.o

clean:
	rm -f main lib/*.o *.o

debug: main
	valgrind --leak-check=full -s ./main

run:
	./main