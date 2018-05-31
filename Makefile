CC = gcc
CFLAGS = -Wall -Wextra -pedantic
LINKS = -lpthread

SRCS = src/comp.c
MAIN = bin/comp

$(MAIN): $(SRCS)
	$(CC) $(SRCS) -o $(MAIN) -g $(CFLAGS) $(LINKS)

prod: $(SRCS)
	$(CC) $(SRCS) -o $(MAIN) -O3 $(CFLAGS) $(LINKS)

clean:
	rm -rf *.o
