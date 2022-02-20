#include "utils.h"
#include <stdio.h>
#include <stdint.h>

#define BLK_SZ 512


int main(int argc, char *argv[]){
    DIR *d;
    struct dirent *ent;
    struct stat sb;

    /* checking usage */
    if (argc != 4){
        printf("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
        return 0;
    }

    /* creating header */
    make_header(argv[3]);

    return 0;
}

/* comment by mark */
void make_header(char *pathname){
    DIR *d;
    struct dirent *ent;
    struct stat sb;


}