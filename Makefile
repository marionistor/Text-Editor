CC = gcc
CFLAGS = -Wall -g
DEPS = include/list.h include/utils.h
OBJS = main.o list.o utils.o
LIBS = -lncurses

main: $(OBJS)
	$(CC) $(CFLAGS) -o main $(OBJS) $(LIBS)

main.o:	main.c $(DEPS)
	$(CC) $(CFLAGS) -c main.c
	
list.o: src/list.c include/list.h
	$(CC) $(CFLAGS) -c src/list.c

utils.o: src/utils.c $(DEPS)
	$(CC) $(CFLAGS) -c src/utils.c

clean:
	rm -f $(OBJS) main
