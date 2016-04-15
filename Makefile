all: test.c storage.c
	gcc -o test test.c storage.c -I.

clean:
	rm -f test
