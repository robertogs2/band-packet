CC = gcc -o
EXE = main
SCHEDULERS_LIB = lib_schedulers.a
LINKEDLIST_LIB = lib_linked_list.a
THREADS_LIB = lib_lpthread.a

LIB_DIR = ../lib/
INCLUDE_DIR = ../include/
BIN_DIR = ../bin/

done: $(EXE)

$(EXE) : $(EXE).c
		#gcc -o ../bin/main main.c ../lib/lib_linked_list.a -I../include/
		$(CC) $(BIN_DIR)$(EXE) $(EXE).c $(LIB_DIR)$(SCHEDULERS_LIB) $(LIB_DIR)$(LINKEDLIST_LIB) $(LIB_DIR)$(THREADS_LIB) -I$(INCLUDE_DIR) -lpthread

