CC=gcc
CFLAGS=-Wall -g
OBJS=main.o list.o
LIBS=-lncurses

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) $(LIBS)

main.o:	main.c
	$(CC) $(CFLAGS) -c main.c
	
list.o: list.c list.h
	$(CC) $(CFLAGS) -c list.c

clean:
	rm -f $(OBJS) main
