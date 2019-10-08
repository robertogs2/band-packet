CC = gcc
CFLAGS = -Wall
OBJFILES = *.o
	
package: packageGenerator.c constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) packageGenerator.c -o packageGenerator -lm
	./packageGenerator

bandsim: bandSim.c constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) bandSim.c -o bandSim -lm
	./bandSim

gui: bandGUI.c constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) bandGUI.c -o bandGUI -lm
	./bandGUI
	
clean:
	echo "Cleaning workspace... \n"
	rm -f OBJFILES
	rm -f packageGenerator
	rm -f bandSim
	rm -f bandGUI
