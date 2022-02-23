#include "utils.h"

/* Note */
void getHeaderName(int fin, headerPtr headerAddr){
  /* lseek(fin, NAME_OFFSET, SEEK_CUR); */
  if( read(fin, headerAddr->name, NAME_LENGTH) < NAME_LENGTH){
    printf("fixthis\n");
  } 
 
}

void getHeaderMode(int fin, headerPtr headerAddr){
    /* lseek(fin, MODE_OFFSET, SEEK_CUR); */
    char buff[9];
    char *ptr;
    lseek(fin, MODE_OFFSET, SEEK_SET);
    read(fin, &buff, MODE_LENGTH);
    headerAddr->mode = strtol(buff, &ptr, 8);
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
    char buff[12];
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


headerPtr readAndMakeHeader(int fin){
    headerPtr header = malloc(sizeof(header));

    getHeaderName(fin, header);  
    getHeaderMode(fin, header);
    getHeaderUid(fin, header);
    getHeaderGid(fin, header);
    getHeaderSize(fin, header);
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
  
    return header;
}

void printTable(int tar){
  headerPtr header;
  
  header = readAndMakeHeader(tar);
  printTableEntry(header);
  lseek(tar, numberDataBlocks(header), SEEK_CUR);

  /* Add loop */

}

/* verbose option */
void printTableEntry(headerPtr headerAddr){
  printPerms(headerAddr->mode);
  printf(" ");
  printOwners(headerAddr->uname, headerAddr->gname);
  printf(" ");
  printSize(headerAddr->size);
  printf(" ");
  printMtime(headerAddr->mtime);
  printf(" ");
  printName(headerAddr->name);
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
    /*printf("%4d-%02d-%02d %02d:%02d", localTime->tm_year + 1900,
                                    localTime->tm_mon,
                                    localTime->tm_mday,
                                    localTime->tm_hour, 
                                    localTime->tm_min
                                    ); */
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
            write_file(fd, sb, pathname);
            /* need some way to detect overflow for prefix*/
            make_header(fd, ent->d_name);
        }
        if (S_ISREG(sb.st_mode) || S_ISLNK(sb.st_mode)){ /* regular file || soft link - header to be created. */
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
    lseek(fd, 257, SEEK_SET);
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
void printPerms(mode_t mode){
    char *ret;
    int p_mask = 0400;
    char i = 9;

    ret = malloc(PERMS);

    if(S_ISDIR(mode))
        strcat(ret, "d");
    else if(S_ISLNK(mode))
        strcat(ret, "l");
    else
        strcat(ret, "-");

    for( ; i > 0 ; i-- ){
        if ( (p_mask & mode) && ((i % 3) == 0))
            strcat(ret, "r");
        else if ((p_mask & mode) && ((i % 3) == 2))
            strcat(ret, "w");
        else if ((p_mask & mode) && ((i % 3) == 1))
            strcat(ret, "x");
        else
            strcat(ret, "-");
        p_mask >>= 1;
    }
    strcat(ret, "\0");
    printf("%s", ret);
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
