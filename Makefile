CFLAGS = -O3 -Wall -Wextra

all: clear shellson

shellson: shellson.c parser.c list.c
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f shellson
