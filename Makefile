CC=gcc
# Added -g flag for Valgrind line-number debugging
FLAGS=-Wall -Werror -pedantic-errors -g
VALGRIND_FLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes

all: main

run: main
	./main

# New target to check memory leaks on standard main
check: main
	valgrind $(VALGRIND_FLAGS) ./main

main: main.c transport_manager.o transport_company.o
	$(CC) $(FLAGS) main.c transport_manager.o transport_company.o -L. -lprog2 -o $@

transport_manager.o: transport_manager.c transport_manager.h transport_company.h prog2_ex3.h set.h
	$(CC) $(FLAGS) -c transport_manager.c -o transport_manager.o

transport_company.o: transport_company.c transport_company.h prog2_ex3.h set.h
	$(CC) $(FLAGS) -c transport_company.c -o transport_company.o

test: test_main
	./test_main

# New target to check memory leaks on your test suite
check_test: test_main
	valgrind $(VALGRIND_FLAGS) ./test_main

test_main: tests/test_main.c transport_manager.o transport_company.o
	$(CC) $(FLAGS) -I. tests/test_main.c transport_manager.o transport_company.o -L. -lprog2 -o $@

clean:
	rm -f *.o main test_main