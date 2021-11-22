CC=gcc -Wall

PROGS=prod_cons

all: $(PROGS)

clean:
	rm -f $(PROGS)

prod_cons: main.c semaforo.c semaforo.h
	$(CC) main.c semaforo.c -o main -lpthread
