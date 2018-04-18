CC=gcc
CFLAGS=-I. -Wall
ALL = src/*.c src/lib/*.c src/ds/*.c
DEPS = -lgmp -Iext/include -Iext/include/ds ext/lib/libgc.so

build: bin/lydia

leg: clean-leg src/parser.c

clean-leg:
	rm -f src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

clean:
	rm -f bin/* src/*.o src/*.so

bin/lydia: ext/lib/libgc.so
	${CC} src/bin/lydia.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lydia

ext/lib/libgc.so:
	mkdir -p ext
	if [ ! -f ext/gc.tar.gz ]; then curl -o ext/gc.tar.gz http://www.hboehm.info/gc/gc_source/gc-7.6.4.tar.gz; fi
	cd ext && tar xzf gc.tar.gz
	cd ext/gc-7.6.4 && ./configure --prefix=`pwd`/../ --enable-redirect-malloc && make && make install

debug: ext/lib/libgc.so
	${CC} src/bin/lydia.c -Isrc ${CFLAGS} -g ${ALL} ${DEPS} -o bin/lydia

cloc:
	cloc --not-match-f=parser\.c --force-lang=C,leg src

test: run-tests

run-tests:
	bin/lydia test/all.lid
