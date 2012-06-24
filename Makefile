CC=gcc
CFLAGS=-I. -Wall
ALL = src/*.c src/lib/*.c src/ds/*.c
DEPS = -lgmp -Iext/include -Iext/include/ds ext/lib/libgc.so

build: bin/lidija

leg: clean-leg src/parser.c

clean-leg:
	rm -f src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

clean:
	rm -f bin/* src/*.o src/*.so

bin/lidija: ext/lib/libgc.so
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lidija

ext/lib/libgc.so:
	mkdir -p ext
	if [ ! -f ext/gc.tar.gz ]; then curl -o ext/gc.tar.gz http://www.hpl.hp.com/personal/Hans_Boehm/gc/gc_source/gc-7.2b.tar.gz; fi
	cd ext && tar xzf gc.tar.gz
	cd ext/gc-7.2 && ./configure --prefix=`pwd`/../ --enable-redirect-malloc && make && make install

debug: ext/lib/libgc.so
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} -g ${ALL} ${DEPS} -o bin/lidija

cloc:
	cloc --not-match-f=parser\.c src

test: run-tests

run-tests:
	bin/lidija test/all.lid
