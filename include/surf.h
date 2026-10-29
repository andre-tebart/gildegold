/* surf.c -- surface scaling and pixel format conversion on locked DDSURFACEDESC2s, original file
   name unknown. The TU probably starts at 0x41ebc0 (the 8/16/24-bit box filters called by
   surf_ScaleBlit lie below 0x41f000) and ends at 0x4206d0. surf_ScaleBlit / surf_Blit are declared
   here. Needs <ddraw.h>. Names are ours. */
#ifndef UNK_41EBC0_H
#define UNK_41EBC0_H

/* Inline-asm copy in the style of D3_ZERO: byte moves up to dword alignment of dst, dwords, bytes.
   n must be a plain variable. */
#define SURF_COPY(dst, src, n) {                                    \
    void *_cs = (src); void *_cd = (dst);                           \
    __asm mov edi, _cd                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, n                                                \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

void surf_Scale8(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                              /* 0x41ebc0 */
void surf_Shrink16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                             /* 0x41ec50 */
void surf_Shrink24(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                             /* 0x41ee70 */
void surf_Down32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                             /* 0x41f000, box filter */
void surf_Up8(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                                /* 0x41f220, nearest */
void surf_Up16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                               /* 0x41f2b0, bilinear */
void surf_Up24(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                               /* 0x41f800, bilinear */
void surf_Up32(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                               /* 0x41fc00, bilinear */
void surf_ScaleBlit(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                       /* 0x420140, scales src down into dst */
void surf_Pal8To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src, PALETTEENTRY *pal);        /* 0x4202d0 */
void surf_24To16(DDSURFACEDESC2 *dst, DDSURFACEDESC2 *src);                             /* 0x420450 */
void surf_Blit(DDSURFACEDESC2 *dst, PALETTEENTRY *dstpal, DDSURFACEDESC2 *src, PALETTEENTRY *srcpal); /* 0x4205c0, copies a same-size src surface into dst, converting the pixel format */

#endif
