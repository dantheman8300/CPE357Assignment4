CC = gcc

CFLAGS = -Wall -pedantic

LDFLAGS = 

TARGET = myTar

OBJFILES = utils.o $(TARGET).o 

all: $(TARGET)

clean:
	rm -f $(MAIN).o *~ 
	rm -f utils.o *~

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJFILES)

utils.o: utils.c utils.h
	gcc -c utils.c
