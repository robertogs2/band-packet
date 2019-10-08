LIBNAME=lpthread
SRC=$(LIBNAME).c
OBJ=$(LIBNAME).o

LIBSTATIC=lib$(LIBNAME).a

CC=gcc
CFLAGS=-c -Wall
SL=ar rvs
DL=ld

all : $(LIBSTATIC)
	
#Static 
$(LIBSTATIC) : $(OBJ)
		$(SL) $(LIBSTATIC) $(OBJ)

#Compile source to object
$(OBJ) : $(SRC)
		$(CC) $(CFLAGS) $(SRC) -o $(OBJ)

.PHONY : clean
clean : 
		rm -rf $(OBJ)|:

