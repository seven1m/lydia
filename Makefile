CC=gcc
CFLAGS=-I. -Wall
ALL = src/*.c src/lib/*.c
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
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lidija

debug:
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} -g ${ALL} ${DEPS} -o bin/lidija

cloc:
	cloc --not-match-f=parser\.c src
