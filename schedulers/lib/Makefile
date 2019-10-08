LIBS = lib_linked_list.a
SRC = linked_list.c
OBJ = linked_list.o
AR = ar rvs

INCLUDE_DIR = ../include/
CC = gcc -c -I$(INCLUDE_DIR)

clean: $(LIBS)
		rm $(OBJ)

$(LIBS) : $(OBJ)
		$(AR) $(LIBS) $(OBJ)

$(OBJ) : $(SRC)
		$(CC) $(SRC)

