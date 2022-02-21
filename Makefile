SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -g -pedantic 
TARGET=myTar
OBJS = $(TARGET).o
LDFLAGS = 
OBJFILES = utils.o $(TARGET).o 

all: $(TARGET)

clean:
	rm -f $(TARGET).o *~ 
	rm -f utils.o *~

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

utils.o: utils.c utils.h
	gcc -c utils.c


test : $(TARGET)
	@echo Testing $(TARGET): 
	@echo testing complete...