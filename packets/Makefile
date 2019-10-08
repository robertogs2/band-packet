CC = gcc
CFLAGS = -Wall
OBJFILES = *.o
SRC_DIR=src/
LIB_DIR=lib/
OUT_DIR=bin/
H_DIR=include/
	
package: $(SRC_DIR)packageGenerator.c $(H_DIR)constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) $(SRC_DIR)packageGenerator.c -o $(OUT_DIR)packageGenerator -lm
	./$(OUT_DIR)packageGenerator

bandsim: $(SRC_DIR)bandSim.c $(H_DIR)constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) $(SRC_DIR)bandSim.c -o $(OUT_DIR)bandSim -lm
	./$(OUT_DIR)bandSim

gui: $(SRC_DIR)bandGUI.c $(H_DIR)constants.h
	echo "Compiling and running... "
	$(CC) $(CFLAGS) $(SRC_DIR)bandGUI.c -o $(OUT_DIR)bandGUI -lm
	./$(OUT_DIR)bandGUI

read: $(SRC_DIR)readConfig.c $(H_DIR)constants.h
	echo "Compiling and running..."
	$(CC) $(CFLAGS) $(SRC_DIR)readConfig.c -o $(OUT_DIR)readConfig -lm
	./$(OUT_DIR)readConfig

clean:
	echo "Cleaning workspace... \n"
	rm -f $(OUT_DIR)packageGenerator
	rm -f $(OUT_DIR)bandSim
	rm -f $(OUT_DIR)bandGUI
	rm -f $(OUT_DIR)readConfig
