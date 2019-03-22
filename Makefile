CFLAGS = -O3 -Wall -Wextra
SOURCE = shellson.c parser.c list.c

all: clean shellson

shellson: 
	$(CC) $(CFLAGS) -o shellson $(SOURCE)

clean:
	rm -f shellson

install: clean shellson
	mv shellson /usr/bin/

uninstall:
	rm -f /usr/bin/shellson