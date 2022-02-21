#include<stdlib.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>

#define BLOCKSIZE 50
#define PADCHAR '$'
#define NAME_SZ 100

void makeDataBlocks(int fin, int fout);

void make_header(int fd, char *pathname);
void write_file(int fd, struct stat sb);
