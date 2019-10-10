SRC = linked_list.c packageGenerator.c
OBJ = *.o
AR = ar rvs

PG = packageGenerator
LL = linked_list

INCLUDE_DIR = ../include/
CC = gcc -c -I$(INCLUDE_DIR)

all: $(SRC)
	make ll
	make pg
	make clean

clean:
	echo "Cleaning workspace..."
	rm $(OBJ)

ll: $(LL).c
	$(CC) $(LL).c
	$(AR) lib_$(LL).a $(LL).o

pg: $(PG).c
	$(CC) $(PG).c -lm
	$(AR) lib_$(PG).a $(PG).o
