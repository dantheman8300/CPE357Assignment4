#include "utils.h"

void getHeaderName(int fin, header headerAddr){
  lseek(fin, NAME_OFFSET, SEEK_CUR);  
  read(fin, headerAddr->name, NAME_LENGTH);
}

void getHeaderMode(int fin, header headerAddr){
  lseek(fin, MODE_OFFSET, SEEK_CUR);  
  read(fin, headerAddr->mode, MODE_LENGTH);
}

void getHeaderUid(int fin, header headerAddr){
  lseek(fin, UID_OFFSET, SEEK_CUR);  
  read(fin, headerAddr->uid, UID_LENGTH);
}

void getHeaderGid(int fin, header headerAddr){
  lseek(fin, GID_OFFSET, SEEK_CUR);  
  read(fin, headerAddr->gid, GID_LENGTH);
}

/*
  makeDataBlocks

  1. Read input file in increments of BLOCKSIZE bytes
  2. If the last set doesn't have BLOCKSIZE bytes, pad with PADCHAR
*/
void makeDataBlocks(int fin, int fout){
  char buf[BLOCKSIZE + 1];
  int num;

  while((num = read(fin, &buf, BLOCKSIZE)) > 0){
    for( ; num <= BLOCKSIZE; num++){
      buf[num] = PADCHAR;
    }
    write(fout, &buf, BLOCKSIZE);
  }
}

/*
    make_header

    takes an open fd and user selected path and reads
    all the files (or the singular file) of the path provided

    calls upon a helper function to write to the archive file
*/
void make_header(int fd, char *pathname){
    DIR *d;
    struct dirent *ent;
    struct stat sb;
    
    /* open directory */
    if ( (d = opendir(pathname)) == NULL ){
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    /* READ ALL DIRECTORY ENTIRES */
    while ( (ent = readdir(d)) ){
        /* dc about directory points to itself and parent */
        if ( !strncmp(".", ent->d_name, 1) || !strncmp("..", ent->d_name, 2) )
            continue;

        /* error checking stat */
        if ( (lstat(ent->d_name, &sb) == -1) ){
            perror("lstat");
            exit(EXIT_FAILURE);
        } 

        /* recurse if directory */
        if (S_ISDIR(sb.st_mode)){
            write(fd, ent->d_name, 100);
            write_file(fd, sb);
            make_header(fd, ent->d_name);
        }
        else{ /* regular file || soft link - header to be created. */
            write(fd, ent->d_name, 100);
            write_file(fd, sb);
        }
    }
    closedir(d);
}

/* 
    write_file

    takes in an open file descriptor and stat structure 
    to write to the tar archive file. 
*/
void write_file(int fd, struct stat sb){
    char name[NAME_SZ]; /* might go on the write_file function */
    char flag;
    write(fd, &sb.st_mode, 8);
    write(fd, &sb.st_uid, 8);
    write(fd, &sb.st_gid, 8);
    write(fd, &sb.st_size, 8);
    write(fd, &sb.st_mtime, 8);
    write(fd, &sb.st_blocks, 8);
    flag = det_file_type(sb);
    if ( ((flag == -1)) || (write(fd, &flag, 1) == -1)){
        perror("flag or write");
        exit(EXIT_FAILURE);
    }
    /* link name here */
    write(fd, "ustar\0", 6);
    write(fd, "00", 2); 
    return;
}

/*  
    det_file_type

    helper function for write_file that takes in stat structure
    returns a char flag to be written into the archive file.

    Notes:
    missing functionality for '\0' 
    also not sure if '5' is ever printed out.
*/
char det_file_type(struct stat sb){
    if (S_ISREG(sb.st_mode)){
        return '0';
    }
    if (S_ISLNK(sb.st_mode)){
        return '2';
    }
    if (S_ISDIR(sb.st_mode)){
        return '5';
    }
    return -1; /* lame error-checking here */
}