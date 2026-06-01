FLAGS=-Wall -Werror
LL_DIR=..

run: people_app
	./people_app

people_app: people_app.c people
	$(CC) people_app.c people.o -L$(LL_DIR) -lprog2 -o $@

people: people.c
	$(CC) $(FLAGS) -I$(LL_DIR) -c people.c -o people.o

clean:
	rm -f *.o people_app
	+$(MAKE) -C $(LL_DIR) clean
