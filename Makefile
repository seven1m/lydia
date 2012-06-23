CC=gcc
CFLAGS=-I. -Wall
ALL = src/*.c src/lib/*.c
DEPS = `pkg-config --cflags glib-2.0 --libs glib-2.0` -lgmp -Iext/include -Iext/include/ds ext/lib/libgc.so ext/lib/libds.a

build: bin/lidija

leg: clean-leg src/parser.c

clean-leg:
	rm src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

clean:
	rm -f bin/* src/*.o src/*.so

bin/lidija: ext/lib/libgc.so ext/lib/libds.a
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} ${ALL} ${DEPS} -o bin/lidija

ext/lib/libgc.so:
	mkdir -p ext
	if [ ! -f ext/gc.tar.gz ]; then curl -o ext/gc.tar.gz http://www.hpl.hp.com/personal/Hans_Boehm/gc/gc_source/gc-7.2b.tar.gz; fi
	cd ext && tar xzf gc.tar.gz
	cd ext/gc-7.2 && ./configure --prefix=`pwd`/../ && make && make install

ext/lib/libds.a:
	if [ ! -f ext/libds.tar.gz ]; then curl -o ext/libds.tar.gz -L https://github.com/zhemao/libds/tarball/master; fi
	cd ext && tar xzf libds.tar.gz && mv zhemao-libds* libds
	cd ext/libds && make && mv libds.a ../lib
	mkdir -p ext/include/ds && cd ext/libds && cp *.h ../include/ds

debug: ext/lib/libgc.so ext/lib/libds.a
	${CC} src/bin/lidija.c -Isrc ${CFLAGS} -g ${ALL} ${DEPS} -o bin/lidija

cloc:
	cloc --not-match-f=parser\.c src
