APP=main
APPE=../bin/$(APP)

SRC=$(APP).c
OBJ=$(APP).o

LIBNAME=lpthread

LIBDIR=../lib
INCDIR=../include

CC=gcc
CFLAGS=-c -Wall -I $(INCDIR) 
LFLAGS= -L $(LIBDIR) -Wall

SLIBS = -l:lib$(LIBNAME).a -lm

all : $(APPE)

#Static
$(APPE) : $(OBJ)
		$(CC) $(LFLAGS) -o $(APPE) $(OBJ) $(SLIBS)

$(OBJ) : $(SRC)
		$(CC) $(CFLAGS) -o $(OBJ) $(SRC)