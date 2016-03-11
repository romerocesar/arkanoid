CC=gcc
EXTRA=-O3 -fomit-frame-pointer
#EXTRA=-g -pg

LIBS=`pkg-config --libs x11 glut`
LOADLIBES=-lm -lSDL -lpthread $(LIBS)
CFLAGS=-Wall -pipe ${EXTRA}
INC=`pkg-config --cflags glut`

objects = main.o paddle.o texto.o bola.o brick.o levels.o extra.o util.o


arkanoid: $(objects)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@

%.o: %.c
	$(COMPILE.c) $(INC) $(OUTPUT_OPTION) $<

$(objects) : util.h 

.PHONY: clean
clean:
	rm -f arkanoid *.o

