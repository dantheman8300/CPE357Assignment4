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
    
    
    if( !has_char('f', argv[1]) ){
        printf("you are missing \'f\' argument. \
                please include it to use myTar");
        exit(0);
    }
    if( has_char('S', argv[1]) || has_char('s', argv[1]) ){
        set_strict();
    }
    if (has_char('t', argv[1])){
        if (has_char('v', argv[1]) && (argc == 3)){
            printTable(fin);
        }
        else if (!has_char('v', argv[1]) && (argc == 3)){
            printTable_NOV(fin);
        }
        else{
            if (has_char('v', argv[1])){
                print_oneshot(fin, argv[3]);
            }
            else{
                print_oneshot_nov(fin, argv[3]);
            }
        }
    }
    if ( has_char('x', argv[1])){
      if (has_char('v', argv[1])){
          extractAllFiles(fin, 1);
        }
        else{
          extractAllFiles(fin, 0);
        }
      
    }
    if( has_char('c', argv[1])){
        if (has_char('v', argv[1])){
            printf("this case is currently not supported.\n");
        }
        else{
            printf("this case is currently not supported.\n");
        }
    }

  close(fin);
  return 0;
}



  


