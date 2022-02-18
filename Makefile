CC = gcc

CFLAGS = -Wall -pedantic

LDFLAGS = 

TARGET = myTar

OBJFILES = utils.o $(TARGET).o 

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

utils.o: utils.c utils.h
	gcc -c utils.c
