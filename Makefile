.PHONY: test

CC=gcc
CFLAGS=-I. -Wall
DEPS = src/lidija.h
ALL = src/closure.c src/eval.c src/heap.c src/parser.c src/value.c

build: bin/lidija

test:
	$(CC) test/runner.c -Ideps -Isrc ${CFLAGS} ${ALL} deps/CuTest.c `pkg-config --cflags glib-2.0 --libs glib-2.0` -o test/runner
	test/runner

leg: clean src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

clean:
	rm -f bin/* src/*.o src/*.so test/*.o test/*.so

bin/lidija:
	$(CC) src/lidija.c -Isrc ${CFLAGS} ${ALL} `pkg-config --cflags glib-2.0 --libs glib-2.0` -o bin/lidija
