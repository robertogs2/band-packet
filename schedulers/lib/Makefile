LIBS = lib_linked_list.a
SRC = linked_list.c
OBJ = linked_list.o
AR = ar rvs

LIBS2 = lib_schedulers.a
SRC2 = schedulers.c
OBJ2 = schedulers.o

INCLUDE_DIR = ../include/
LIB_DIR=../lib/
CC = gcc -c -I$(INCLUDE_DIR)

all: $(LIBS) $(LIBS2)


$(LIBS2) : $(OBJ2)
		$(AR) $(LIBS2) $(OBJ2)

$(OBJ2) : $(SRC2)
		$(CC) $(SRC2) $(LIB_DIR)$(LIBS)

$(LIBS) : $(OBJ)
		$(AR) $(LIBS) $(OBJ)

$(OBJ) : $(SRC)
		$(CC) $(SRC)


clean: $(LIBS)
		rm $(OBJ)