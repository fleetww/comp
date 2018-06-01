CC = gcc
CFLAGS = -Wall

INCL = -I./include

SRCS = src/comp.c src/pq.c
MAIN = bin/comp

$(MAIN): $(SRCS)
	$(CC) $(SRCS) -o $(MAIN) -g $(CFLAGS) $(INCL)

prod: $(SRCS)
	$(CC) $(SRCS) -o $(MAIN) -O3 $(CFLAGS) $(INCL)

clean:
	rm -rf *.o
