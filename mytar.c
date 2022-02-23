#include "utils.h"

int main(int argc, char *argv[]){
    int fin;
    /*DIR *d;
    int fout;
    struct stat sb;
    struct dirent *ent;
    headerPtr header;*/

    /* Check arguments */
    /* this is wrong... should be 4? */
    if(argc < 3){
        printf("usage: ./myTar [ctxvs]f tarfile [ path [ ... ] ]");
        exit(0);
    }

    /* Input file open - should be 2 later...*/
    if((fin = open(argv[2], O_RDONLY)) < 0){
        perror(argv[2]);
        exit(0);
    }

    /* Output file open - should be 3 later? / not needed? */
    /*
    if((fout = open(argv[2], O_CREAT | O_WRONLY, 0711)) < 0){
        perror(argv[2]);
        exit(0);
    }
    */

    /* argument checking - to test functionality later */
    /***************************************************
    NOTES: 
        f:      implemented
        t + v:  should be easy to implement
                will read into header and return
                either just name or it v, more detailed header
        x:      relies on same codebase as t
                unsure of how 'v' helps here
        c:      have not really started to look at this 
                or even how to implement 'v'
    ***************************************************/
    
  

  close(fin);
  return 0;
}



  


