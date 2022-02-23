#include "utils.h"

int strict;

void set_strict(){
    strict = 1;
    return;
}

int get_strict(){
    return strict; 
}

/* Note */
int getHeaderName(int fin, headerPtr headerAddr){
  /* lseek(fin, NAME_OFFSET, SEEK_CUR); */
  read(fin, headerAddr->name, NAME_LENGTH);
 
}

void getHeaderMode(int fin, headerPtr headerAddr){
    /* lseek(fin, MODE_OFFSET, SEEK_CUR); */
    char buff[9];
    read(fin, &buff, MODE_LENGTH);
    headerAddr->mode = strtol(buff, NULL, 8);
}

void getHeaderUid(int fin, headerPtr headerAddr){
    /*lseek(fin, UID_OFFSET, SEEK_CUR);  */
    read(fin, &(headerAddr->uid), UID_LENGTH);
}

void getHeaderGid(int fin, headerPtr headerAddr){
    /*lseek(fin, GID_OFFSET, SEEK_CUR);  */
    read(fin, &(headerAddr->gid), GID_LENGTH);
}

void getHeaderSize(int fin, headerPtr headerAddr){
    /*lseek(fin, SIZE_OFFSET, SEEK_CUR);  */
    char buff[13];
    char *ptr;
    read(fin, &buff, SIZE_LENGTH);
    headerAddr->size = strtol(buff, &ptr, 8);
}

void getHeaderMtime(int fin, headerPtr headerAddr){
    /*lseek(fin, MTIME_OFFSET, SEEK_CUR);  */
    char buff[12];
    char *ptr;
    read(fin, &buff, MTIME_LENGTH);
    headerAddr->mtime = strtol(buff, &ptr, 8);
}

void getHeaderChksum(int fin, headerPtr headerAddr){
    /*lseek(fin, CHKSUM_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->chksum, CHKSUM_LENGTH);
}

void getHeaderTypeflag(int fin, headerPtr headerAddr){
    /*lseek(fin, TYPEFLAG_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->typeflag, TYPEFLAG_LENGTH);
}

void getHeaderLinkname(int fin, headerPtr headerAddr){
    /*lseek(fin, LINKNAME_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->linkname, LINKNAME_LENGTH);
}

void getHeaderMagic(int fin, headerPtr headerAddr){
    /*lseek(fin, MAGIC_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->magic, MAGIC_LENGTH);
}

void getHeaderVersion(int fin, headerPtr headerAddr){
    /*lseek(fin, VERSION_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->version, VERSION_LENGTH);
}

void getHeaderUname(int fin, headerPtr headerAddr){
    /*lseek(fin, UNAME_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->uname, UNAME_LENGTH);
}

void getHeaderGname(int fin, headerPtr headerAddr){
    /*lseek(fin, GNAME_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->gname, GNAME_LENGTH);
}

void getHeaderDevmajor(int fin, headerPtr headerAddr){
    /*lseek(fin, DEVMAJOR_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->devmajor, DEVMAJOR_LENGTH);
}

void getHeaderDevminor(int fin, headerPtr headerAddr){
    /*lseek(fin, DEVMINOR_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->devminor, DEVMINOR_LENGTH);
}

void getHeaderPrefix(int fin, headerPtr headerAddr){
    /*lseek(fin, PREFIX_OFFSET, SEEK_CUR);  */
    read(fin, headerAddr->prefix, PREFIX_LENGTH);
}


int readAndMakeHeader(int fin, headerPtr header){

  char prevName[NAME_LENGTH];

  strcpy(prevName, header->name);
  getHeaderName(fin, header);
  if(strlen(header->name) == 0 || strcmp(prevName, header->name) == 0){
    return 0;
  }
  getHeaderMode(fin, header);
  getHeaderUid(fin, header);
  getHeaderGid(fin, header);
  getHeaderSize(fin, header);
  /*if(header->size <= 0){
      return 0;
  }*/
  getHeaderMtime(fin, header);
  getHeaderChksum(fin, header);
  getHeaderTypeflag(fin, header);
  getHeaderLinkname(fin, header);
  getHeaderMagic(fin, header);
  getHeaderVersion(fin, header);
  getHeaderUname(fin, header);
  getHeaderGname(fin, header);
  getHeaderDevmajor(fin, header);
  getHeaderDevminor(fin, header);
  getHeaderPrefix(fin, header);

  return 1;
}

void clearHeader(headerPtr header){
    header = NULL;
}

void print_oneshot_nov(int tar, char *s){
    headerPtr header = malloc(sizeof(header));

    while(readAndMakeHeader(tar, header)){
        if( !strcmp(header->name, s) ){
            printTableNames(header);
            return;
        }
        lseek(tar, 12, SEEK_CUR);
        lseek(tar, numberDataBlocks(header) * 512, SEEK_CUR);
    }

    printf("tar: %s: Not found in archive\n", s);
    printf("tar: Exiting with failure status due to previous errors\n");
    exit(3);
}

void print_oneshot(int tar, char *s){
    headerPtr header = malloc(sizeof(header));

    while(readAndMakeHeader(tar, header)){
        if( !strcmp(header->name, s) ){
            printTableEntry(header);
            return;
        }
        lseek(tar, 12, SEEK_CUR);
        lseek(tar, numberDataBlocks(header) * 512, SEEK_CUR);
    }

    printf("tar: %s: Not found in archive\n", s);
    printf("tar: Exiting with failure status due to previous errors\n");
    exit(3);
}

void printTable(int tar){
  headerPtr header = malloc(sizeof(header));

/* Don't delete */
/*
  readAndMakeHeader(tar, header);

  printTableEntry(header);
*/

  while(readAndMakeHeader(tar, header)){
    printTableEntry(header);
    lseek(tar, 12, SEEK_CUR);
    lseek(tar, numberDataBlocks(header) * 512, SEEK_CUR);
  }
  
}


void printTable_NOV(int tar){
  headerPtr header = malloc(sizeof(header));

  while(readAndMakeHeader(tar, header)){
    printTableNames(header);
    lseek(tar, 12, SEEK_CUR);
    lseek(tar, numberDataBlocks(header) * 512, SEEK_CUR);
  }
  
}

void printTableNames(headerPtr headerAddr){
    printName(headerAddr->name);
    printf("\n");
}


/* verbose option */
void printTableEntry(headerPtr headerAddr){
    printPerms(headerAddr->mode, headerAddr->typeflag);
    printf(" ");
    printOwners(headerAddr->uname, headerAddr->gname);
    printf(" ");
    printSize(convertDecimalToOctal(headerAddr->size));
    printf(" ");
    printMtime(headerAddr->mtime);
    printf(" ");
    if( strlen(headerAddr->prefix)){ 
        printf("%s/", headerAddr->prefix);
    }
    printName(headerAddr->name);
    if( strlen(headerAddr->linkname) ){
        printf(" -> %s", headerAddr->linkname);
    }
    printf("\n");
}

int numberDataBlocks(headerPtr headerAddr){
    int size = headerAddr->size; 
    int numBlocks = size / BLOCKSIZE;
    if(size % BLOCKSIZE){
        numBlocks++;
    }
    return numBlocks;
}

void printOwners(char *uname, char *gname){
    printf("%s/%s", uname, gname);
}

void printSize(int size){
    printf("%d", convertOctalToDecimal(size));
}

void printMtime(time_t mtime){
    char t[17];
    struct tm *timer;
    timer = localtime(&mtime);
    strftime(t, 17, "%Y-%m-%d %R", timer);
    printf("%s", t);

}

void printName(char *name){
    printf("%s", name);
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

    NOTE: no way of writing prefix attribute in the header.
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
        if ( !strncmp(".", ent->d_name, 1) || !strncmp("..", ent->d_name, 2))
            continue;

        /* error checking stat */
        if ( (lstat(ent->d_name, &sb) == -1) ){
            perror("lstat");
            exit(EXIT_FAILURE);
        } 

        /* recurse if directory */
        if (S_ISDIR(sb.st_mode)){
            write(fd, ent->d_name, 100);
            write_file(fd, sb, pathname);
            /* need some way to detect overflow for prefix*/
            make_header(fd, ent->d_name);
        }
        /* regular file || soft link - header to be created. */
        if (S_ISREG(sb.st_mode) || S_ISLNK(sb.st_mode)){ 
            write(fd, ent->d_name, 100);
            write_file(fd, sb, pathname);
            /* need some way to detect overflow for prefix*/
        }
        else{
            printf("this filetype is not supported\n\
            supported: DIRs, Symbolic Links, Regular Files.\n\
            Filename provdied: %s", ent->d_name);
        }
    }
    closedir(d);
}

/* 
    write_file

    takes in an open file descriptor and stat structure 
    to write to the tar archive file. 
*/
void write_file(int fd, struct stat sb, char *pathname){
    char name[NAME_SZ]; /* might go on the write_file function */
    char flag;
    struct passwd user;
    struct group grp;
    dev_t maj;
    dev_t min;
    
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
    if ( S_ISLNK(sb.st_mode) && readlink(pathname, name, 100) > 0) {
        /* link name here */
        if (strlen(name) <= 99){
            if ( realloc(name, strlen(name) + 1) == NULL){
                perror("realloc");
                exit(EXIT_FAILURE);
            }
            strcat(name, "\0");
        }
        write(fd, name, 100);
    }
    /* lseek(fd, 257, SEEK_SET); */
    write(fd, "ustar\0", 6);
    write(fd, "00", 2); 
    user = *getpwuid(sb.st_uid);
    grp = *getgrgid(sb.st_gid); 
    strcat(user.pw_name, "\0");
    strcat(grp.gr_name, "\0");
    write(fd, user.pw_name, 32);
    write(fd, grp.gr_name, 32);
    maj = (0xFFFF0000 & sb.st_dev);
    min = (0x0000FFFF & sb.st_dev);
    write(fd, &maj, 8);
    write(fd, &min, 8);
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


/*  
    perms

    helper function for archive listing to create a list of permissions
    based on the mode_t mode that is passed in.

    Notes:
    testing completed successfully for permissions (rwx);
    awaiting to confirm functionality for file type.
*/
void printPerms(mode_t mode, uint8_t  *t){
    char *ret;
    int p_mask = 0400;
    char i = 9;

    if(*t == '5')
        printf("d"); /* strcat(ret, "d"); */
    else if(*t == '2')
        printf("l"); /* strcat(ret, "l"); */
    else
        printf("-"); /* strcat(ret, "-"); */

    for( ; i > 0 ; i-- ){
        if ( (p_mask & mode) && ((i % 3) == 0))
            printf("r"); /* strcat(ret, "r"); */
        else if ((p_mask & mode) && ((i % 3) == 2))
            printf("w"); /* strcat(ret, "w"); */
        else if ((p_mask & mode) && ((i % 3) == 1))
            printf("x"); /* strcat(ret, "x"); */
        else
            printf("-"); /* strcat(ret, "-"); */
        p_mask >>= 1;
    }
    return;
}

/* NICO FUNCTION IF NEEDED TO TEST */
uint32_t extract_special_int(char *where, int len){
    int32_t val = -1;
    if ( (len >= sizeof(val)) && (where[0] & 0x80) ){
        val = *(int32_t *)(where+len-sizeof(val));
        val = ntohl(val);
    }
    return val;
}

/* NICO FUNCTION IF NEEDED TO TEST */
int insert_special_character(char *where, size_t size, int32_t val){
    int err = 0;
    if( val < 0 || (size < sizeof(val))){
        err++;
    }
    else{
        memset(where, 0, size);
        *(int32_t *)(where+size-sizeof(val)) = htonl(val);
        *where |= 0x80;
    }

    return err;
}

/* helper function found online to
   help convert from base 10 to 8. */
int convertDecimalToOctal(int decimalNumber)
{
    int octalNumber = 0, i = 1;
    while (decimalNumber != 0)
    {
        octalNumber += (decimalNumber % 8) * i;
        decimalNumber /= 8;
        i *= 10;
    }
    return octalNumber;
}

/* helper function found online to
   help convert from base 8 to 10. */
int convertOctalToDecimal(int octalNumber)
{
    int decimalNumber = 0, i = 0;
    while(octalNumber != 0)
    {
        decimalNumber += (octalNumber%10) * pow(8,i);
        ++i;
        octalNumber/=10;
    }
    i = 1;
    return decimalNumber;
}

/* Needed? */
int oct2int(uint8_t *oct, int size){
  int dec = 0;
  int i;
  int currDig = 1;
  for(i = size -1; i >= 0; i--){
    printf("%d\n", oct[i] -48);
    dec += currDig * (oct[i] - 48);
    currDig *= 8;
  }
  return dec;
}

/*  
    has_char

    helper function for parsing argv[1] (under proper usage)
    to find what modes the user specifies.

    Notes:
    tested and confirmed functionality.
*/
int has_char(char c, char *str){
    char *pt;
    pt = str;
    while(*pt){
        if( c == *pt++)
            return 1;
    }
    return 0;
}