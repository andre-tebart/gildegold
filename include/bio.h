/* bio.c -- binary file i/o helpers ("bio:bio_rd_array_quick" strings), original file name unknown.
   Extent in GildeGold.exe (inferred): 0x401000..0x401410, the first code in the exe; its .data are the
   "bio:" strings at 0x5f1030..0x5f1080. Names are ours, modelled on bio_rd_array_quick. */
#ifndef UNK_401000_H
#define UNK_401000_H

struct VFSFILE;
#include "vfs.h"

extern const unsigned int bio_CrcTable[256];  /* 0x5eb380, the zlib crc32 table */

unsigned int util_Hash(unsigned char *buf, unsigned int len);                           /* 0x401000 */
char *util_BaseName(char *path);                                                         /* 0x401040 */
int file_Exists(char *path);                                                             /* 0x401090 */
char *util_ReadLine(VFSFILE *f);                                                         /* 0x4010b0 */
void bio_rd_c(VFSFILE *f, char *c);                                                     /* 0x401180 */
void bio_rd_s(struct VFSFILE *f, char *s);                                                     /* 0x4011a0 */
void bio_rd_i(struct VFSFILE *f, int *i);                                                      /* 0x4011d0 */
void bio_wr_c(VFSFILE *f, char c);                                                      /* 0x4011f0 */
void bio_wr_s(struct VFSFILE *f, char *s);                                                     /* 0x401210 */
void bio_wr_i(struct VFSFILE *f, int i);                                                       /* 0x401240 */
void bio_rd_v3(VFSFILE *f, int *v);                                                     /* 0x401260 */
void bio_rd_v4(VFSFILE *f, int *v);                                                     /* 0x401290 */
void bio_wr_v3(VFSFILE *f, int *v);                                                     /* 0x4012d0 */
void bio_wr_v4(VFSFILE *f, int *v);                                                     /* 0x401300 */
void bio_wr_array(VFSFILE *f, int size, int n, void *data);                             /* 0x401340 */
int bio_rd_array_quick(VFSFILE *f, int size, void **out);                               /* 0x401380 */

#endif
