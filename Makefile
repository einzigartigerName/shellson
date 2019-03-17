CFLAGS = -O3 -Wall -Wextra

all: clean shellson

shellson: shellson.c parser.c list.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f shellson
