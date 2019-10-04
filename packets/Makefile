CC = gcc
CFLAGS = -Wall
OBJFILES = *.o
	
package: packageGenerator.c constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) packageGenerator.c -o packageGenerator -lm
	./packageGenerator
	
clean:
	echo "Cleaning workspace... \n"
	rm -f OBJFILES
	rm -f packageGenerator
