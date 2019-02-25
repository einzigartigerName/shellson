CFLAGS = -03 -Wall -Wextra

all: clear shellson

shellson: shellson.c parser.c list.c
	$(CC) $(CFLAGS) -o $@ $^

clear:
	rm -f shellson