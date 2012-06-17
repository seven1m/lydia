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
	rm -f bin/* src/*.o src/*.so ext/*.so

bin/lidija:
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lidija

ext/libgc.so:
	mkdir -p ext
	if [ ! -f ext/gc.tar.gz ]; then curl -o ext/gc.tar.gz http://www.hpl.hp.com/personal/Hans_Boehm/gc/gc_source/gc-7.2b.tar.gz; fi
	cd ext && tar xzf gc.tar.gz
	cd ext/gc-7.2 && ./configure --prefix=`pwd`/../ && make && make install

debug:
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} -g ${ALL} ${DEPS} -o bin/lidija

cloc:
	cloc --not-match-f=parser\.c src
