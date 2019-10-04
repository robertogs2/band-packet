CFLAGS= -Wall -Wextra -pedantic -Werror -g 

# To debug with valgrind:
#CFLAGS:=$(CFLAGS) -DVALGRIND

# To get debugging output
#CFLAGS:=$(CFLAGS) -DLF_DEBUG

PROGRAMS=example-clone
all: $(PROGRAMS)

clean:
	$(RM) *.o $(PROGRAMS) &> /dev/null || true
	
debug: clean
	make "CC=gcc -g -Wall -pedantic -DLF_DEBUG"

$(PROGRAMS): libfiber-clone.o example.o
	$(CC) $(LDFLAGS) -DMACRO=value libfiber-clone.o example.o -o example-clone
	
libfiber-clone.o: libfiber.h
example.o: libfiber.h
