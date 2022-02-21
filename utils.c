#include "utils.h"

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
        else{ /* regular file - header to be created. */
            write(fd, ent->d_name, 100);
            write_file(fd, sb);
        }
    }

}

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
    write(fd, &flag, 1);
    return;
}

/*  missing functionality for '\0' 
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