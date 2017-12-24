all: clean untar a3search

a3search: a3search.c stmr.h uthash.h
	gcc -lm -std=gnu99 stmr.c a3search.c -o a3search

clean:
	rm -rf index stmr.c stmr.h uthash.h *.out *.sorted *.idx

untar:
	ls *.tar | xargs -n1 tar xvf