#include<stdlib.h>
#include<stdint.h>
#include<unistd.h>
#include<stdio.h>
#include<fcntl.h> 
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<stdint.h>

#define NAME_LENGTH 100
#define NAME_OFFSET 0
#define MODE_LENGTH 8
#define MODE_OFFSET 100
#define UID_LENGTH 8
#define UID_OFFSET 108
#define GID_LENGTH 8
#define GID_OFFSET 116
#define SIZE_LENGTH 12
#define SIZE_OFFSET 124
#define MTIME_LENGTH 12
#define MTIME_OFFSET 136
#define CHKSUM_LENGTH 8
#define CHKSUM_OFFSET 148
#define TYPEFLAG_LENGTH 1
#define TYPEFLAG_OFFSET 156
#define LINKNAME_LENGTH 100
#define LINKNAME_OFFSET 157
#define MAGIC_LENGTH 6
#define MAGIC_OFFSET 257
#define VERSION_LENGTH 2
#define VERSION_OFFSET 263
#define UNAME_LENGTH 32
#define UNAME_OFFSET 265
#define GNAME_LENGTH 32
#define GNAME_OFFSET 297
#define DEVMAJOR_LENGTH 8
#define DEVMAJOR_OFFSET 329
#define DEVMINOR_LENGTH 8
#define DEVMINOR_OFFSET 337
#define PREFIX_LENGTH 155
#define PREFIX_OFFSET 345

#define BLOCKSIZE 50
#define PERMISSION_WIDTH 10

#define PADCHAR '$'
#define NAME_SZ 100

typedef struct *Header header;

struct Header{
  uint8_t name[NAME_LENGTH];
  uint8_t mode[MODE_LENGTH];
  uint8_t uid[UID_LENGTH];
  uint8_t gid[GID_LENGTH];
  uint8_t size[SIZE_LENGTH];
  uint8_t mtime[MTIME_LENGTH];
  uint8_t chksum[CHKSUM_LENGTH];
  uint8_t typeflag[TYPEFLAG_LENGTH];
  uint8_t linkname[LINKNAME_LENGTH];
  uint8_t magic[MAGIC_LENGTH];
  uint8_t version[VERSION_LENGTH];
  uint8_t uname[UNAME_LENGTH];
  uint8_t gname[GNAME_LENGTH];
  uint8_t devmajor[DEVMAJOR_LENGTH];
  uint8_t devminor[DEVMINOR_LENGTH];
  uint8_t prefix[PREFIX_LENGTH];
};

void makeDataBlocks(int fin, int fout);

/* mark */ 
void make_header(int fd, char *pathname);

void write_file(int fd, struct stat sb);

char det_file_type(struct stat sb);