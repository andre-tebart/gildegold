/* bmp.c -- Windows bitmap loader and writer ("!bmp:" strings), original file name unknown.
   Extent in GildeGold.exe (inferred): 0x401410..0x401d50. Names are ours. */
#ifndef UNK_401410_H
#define UNK_401410_H

#include "vfs.h"

/* bmp_Load flags */
#define BMP_LOAD  0x01              /* read the pixels into data */
#define BMP_PAL   0x02              /* return the palette in pal (8-bit images) */
#define BMP_RGB   0x08              /* 24-bit output (8-bit images are expanded through their palette) */
#define BMP_RGBA  0x10              /* 32-bit output (implies BMP_RGB) */
#define BMP_BGR   0x20              /* with BMP_RGBA: swap red and blue */

unsigned char bmp_GetInfo(char *path, int *w, int *h, int *bpp);                        /* 0x401410 */
unsigned char bmp_Load(char *path, unsigned char *data, unsigned char *pal, int flags); /* 0x4014d0 */
unsigned char bmp_Save24(char *path, int w, int h, unsigned char *data);               /* 0x401b90 */

#endif
