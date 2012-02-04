.PHONY: test src/parser.c

CC=gcc
CFLAGS=-I. -Wall
DEPS = src/airball.h

test: src/airball.o
	$(CC) test/runner.c -Ideps -Isrc ${CFLAGS} src/airball.o deps/CuTest.c `pkg-config --cflags glib-2.0 --libs glib-2.0` -o test/runner
	test/runner

leg: clean src/parser.c

src/parser.c:
	leg src/parser.leg -o src/parser.c

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS) `pkg-config --cflags glib-2.0`

clean:
	rm -f src/*.o src/*.so test/*.o test/*.so
