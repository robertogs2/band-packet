CC = gcc -o
EXE = main2
SCHEDULERS_LIB = lib_schedulers.a
LINKEDLIST_LIB = lib_linked_list.a
THREADS_LIB = lib_lpthread.a
PACKAGE_LIB = lib_package.a

LIBS = $(LIB_DIR)$(SCHEDULERS_LIB) $(LIB_DIR)$(LINKEDLIST_LIB) $(LIB_DIR)$(THREADS_LIB) $(LIB_DIR)$(PACKAGE_LIB)

LIB_DIR = ../lib/
INCLUDE_DIR = ../include/
BIN_DIR = ../bin/

done: $(EXE)

$(EXE) : $(EXE).c
		$(CC) $(BIN_DIR)$(EXE) $(EXE).c $(LIBS) -I$(INCLUDE_DIR) -lm #-lpthread

