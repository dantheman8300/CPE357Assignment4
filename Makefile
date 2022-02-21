SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -g -pedantic 
MAIN=myTar
OBJS = $(MAIN).o
LDFLAGS = 
OBJFILES = utils.o $(TARGET).o 

all: $(TARGET)

clean:
	rm -f $(MAIN).o *~ 
	rm -f utils.o *~

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

utils.o: utils.c utils.h
	gcc -c utils.c


test : $(MAIN)
	@echo Testing $(MAIN): 
	@echo testing complete...