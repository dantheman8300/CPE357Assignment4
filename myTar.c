#include "utils.h"

int main(int argc, char *argv[]){
    DIR *d;
    struct dirent *ent;
    struct stat sb;
    int fin, fout;
    headerPtr header;

    /* Check arguments */
    if(argc < 3){
        printf("Not even arguments");
        exit(0);
    }

    /* Input file open */
    if((fin = open(argv[1], O_RDONLY)) < 0){
        perror(argv[1]);
        exit(0);
    }

    /* Output file open */
    if((fout = open(argv[2], O_CREAT | O_WRONLY, 0711)) < 0){
        perror(argv[2]);
        exit(0);
    }
    
    header = readAndMakeHeader(fin);

    printf("%d\n", convertOctalToDecimal(611));

    return 0;
}



  


