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