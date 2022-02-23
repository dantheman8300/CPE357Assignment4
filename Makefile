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
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES) -lm

utils.o: utils.c utils.h
	gcc -c utils.c


test : $(TARGET)
	@echo Testing $(TARGET): 
	./myTar EZDIR_tarfile.tar output.txt > diff1.txt
	tar tvf EZDIR_tarfile.tar > diff2.txt
	diff -s diff1.txt diff2.txt
	@echo testing complete...