FILES = parse.c shell.c
CFLAGS = -O3 -w
default: main.c parse.c shell.c
	gcc main.c $(FILES) $(CFLAGS) -o shell
clean:
	rm -f shell
	rm -f testfile.txt
	rm -f testfile2.txt
