#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include <fcntl.h> 
#include<sys/types.h>
#include<sys/stat.h>

#define BLOCKSIZE 50
#define PADCHAR '$'

void makeDataBlocks(int fin, int fout);
