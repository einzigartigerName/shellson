CFLAGS = -O3 -Wall -Wextra

all: clear shellson sshellson

shellson: shellson.c parser.c list.c
	$(CC) $(CFLAGS) -o $@ $^

sshellson: shellson.c pparser.c list.c
	$(CC) $(CFLAGS) -o $@ $^

clear:
	rm -f shellson shellson_neu