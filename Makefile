FLAGS=-Wall -Werror

run: main
	./main

main: main.c transport_manager.o
	$(CC) $(FLAGS) main.c transport_manager.o -L. -lprog2 -o $@

transport_manager.o: transport_manager.c
	$(CC) $(FLAGS) -c transport_manager.c -o transport_manager.o

clean:
	rm -f *.o main