#include "utils.h"

int main(int argc, char *argv[]){

  if (argc != 4){
    printf("usage: mytar [ctxvS]f tarfile [ path [ ... ] ]\n");
    return 0;
  }

  printf("Hello!\n");

  return 0;
}