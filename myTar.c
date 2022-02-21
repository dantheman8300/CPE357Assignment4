#include "utils.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

void make_header(int fd, char *path);
void write_file(int fd, struct stat sb);

int main(int argc, char *argv[]){
    DIR *d;
    struct dirent *ent;
    struct stat sb;
    int fd;

    /* checking usage */
    if (argc != 4){
        printf("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        return -1;
    }

    /* creating header */
    if ( (fd = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC)) == -1 ){
        perror("open");
        return -1;
    }
    make_header(fd, argv[3]);
    close(fd);

    return 0;
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
    write(fd, &sb.st_mode, 8);
    write(fd, &sb.st_uid, 8);
    write(fd, &sb.st_gid, 8);
    write(fd, &sb.st_size, 8);
    write(fd, &sb.st_mtime, 8);

    return;
}
