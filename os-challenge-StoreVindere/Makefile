CC     = gcc
CFLAGS = -O3 -Wall -Wextra
LDLIBS = -lpthread

SOURCES = server.c functions.c
HEADERS = functions.h messages.h lonesha256.h

server: $(SOURCES) $(HEADERS)
	$(CC) $(CFLAGS) -o server $(SOURCES) $(LDLIBS)

clean:
	rm -f server

.PHONY: clean
