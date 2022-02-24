SHELL = /bin/sh
CC = gcc
CFLAGS = -Wall -g -pedantic 
TARGET=mytar
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
	./mytar tvf EZDIR_tarfile.tar basic> diff1.txt
	~pn-cs357/demos/mytar tvf EZDIR_tarfile.tar basic> diff2.txt
	diff -s diff1.txt diff2.txt
	@echo testing complete...