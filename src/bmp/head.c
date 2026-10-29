/* bmp.c -- original file name unknown: Windows bitmap loader and writer ("!bmp:" strings at
   0x5f1084..0x5f10f8). Extent in GildeGold.exe (inferred): 0x401410..0x401d50.
   Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/bmp.c); boundaries in decl/text.dcl, notes in src/bmp/open.md */
#include <stdlib.h>
#include <windows.h>
#include "bmp.h"
#include "qnt.h"

/* the engine's inline-asm clear (D3_ZERO in d3.h, which needs the DirectX headers) */
#define BMP_CAT(a, b) a##b
#define BMP_LABEL(name, n) BMP_CAT(name, n)
#define BMP_ZERO(dst, size) {                                       \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_zero, __LINE__)                         \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm BMP_LABEL(bmp_zero, __LINE__): add ecx, ebx               \
    __asm rep stosb                                                 \
}

/* byte fill (D3E_FILL in d3_engine.h): value and size through temps; the destination is assigned to the
   caller's pointer variable dvar inside the block (a block-local temp never shared the x slot: VC6's code
   needs this form, match 2026-10-23) */
#define BMP_FILL(dvar, dst, val, size) {                            \
    int _fn = (size); int _fv = (val); dvar = (dst);                \
    __asm mov edi, dvar                                             \
    __asm mov eax, _fv                                              \
    __asm mov ebx, _fn                                              \
    __asm mov ah, al                                                \
    __asm shl eax, 16                                               \
    __asm mov ecx, eax                                              \
    __asm shr ecx, 16                                               \
    __asm add eax, ecx                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_fill, __LINE__)                         \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm BMP_LABEL(bmp_fill, __LINE__): add ecx, ebx               \
    __asm rep stosb                                                 \
}

/* copy (D3_COPY in d3.h); all three named directly in the asm */
#define BMP_COPY(dst, src, size) {                                  \
    __asm mov edi, dst                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, size                                             \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle BMP_LABEL(bmp_copy, __LINE__)                         \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm BMP_LABEL(bmp_copy, __LINE__): add ecx, ebx               \
    __asm rep movsb                                                 \
}

