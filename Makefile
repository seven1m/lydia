CC=gcc
CFLAGS=-I. -Wall
ALL = src/closure.c src/eval.c src/heap.c src/parser.c src/value.c
DEPS = `pkg-config --cflags glib-2.0 --libs glib-2.0` -lgmp

build: bin/lidija

leg: clean-leg src/parser.c

clean-leg:
	rm src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

clean:
	rm -f bin/* src/*.o src/*.so

bin/lidija:
	${CC} src/lidija.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lidija
