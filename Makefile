CFLAGS=-std=c99 -O2 -Wall

dictionary : dictionary.o trie.o parse.o
	gcc $(CFLAGS) -o dictionary dictionary.o trie.o parse.o

dictionary.o : dictionary.c parse.h trie.h
	gcc $(CFLAGS) -c dictionary.c parse.h trie.h

parse.o : parse.c parse.h
	gcc $(CFLAGS) -c parse.c parse.h

trie.o : trie.c trie.h
	gcc $(CFLAGS) -c trie.c trie.h

debug :
	gcc $(CFLAGS) -g -o dictionary.dbg dictionary.c parse.c parse.h trie.c trie.h

clean :
	rm -f *.o
