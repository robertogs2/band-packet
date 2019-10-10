CC = gcc -o
EXE = main
LIBS = lib_schedulers.a
LIBS2 = lib_linked_list.a

LIB_DIR = ../lib/
INCLUDE_DIR = ../include/
BIN_DIR = ../bin/

done: $(EXE)

$(EXE) : $(EXE).c
		#gcc -o ../bin/main main.c ../lib/lib_linked_list.a -I../include/
		$(CC) $(BIN_DIR)$(EXE) $(EXE).c $(LIB_DIR)$(LIBS) $(LIB_DIR)$(LIBS2) -I$(INCLUDE_DIR) -lpthread

