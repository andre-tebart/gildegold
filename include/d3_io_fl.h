struct D3VECTOR4;
struct VFSFILE;
/* d3_io_fl.h -- d3_io_fl: the fast-chunk BGF loader, mesh helpers and fast-chunk writer
   (0x450860..0x451e7c), see d3_io_fl.c.
   d3_FindBGF and d3_CalcNormals are declared in d3_io_t.h. Names are ours. */
#ifndef UNK_450860_H
#define UNK_450860_H

typedef struct D3BGFVERT { float x, y, z, nx, ny, nz; } D3BGFVERT;

void bio_rd_c(struct VFSFILE *f, char *c);                                                /* 0x401180 */
void bio_rd_v3(struct VFSFILE *f, int *v);                                                /* 0x401260 */
struct VFSFILE * d3_Unk4507a0(char *path);                                                 /* 0x4507a0, opens the fast chunk file */
void d3_TriNormal(struct D3VECTOR4 *a, struct D3VECTOR4 *b, struct D3VECTOR4 *c, struct D3VECTOR4 *n);              /* 0x454700, face normal */
void d3_CalcBounds(D3BGF *m);                                                    /* 0x450ec0 */
int d3_FLChunkSize(D3BGF *m, D3BGFDESC *d);                                       /* 0x451180 */

/* 0x451250 d3_AddFLChunk (was unk_451250.h, worker mesh) */
long vfs_ftell(struct VFSFILE *f);                                                 /* 0x431170 */
int vfs_fseek(struct VFSFILE *f, long offset, int origin);                            /* 0x430fa0 */
int vfs_fread(void *buf, int size, int n, struct VFSFILE *f);                     /* 0x430bd0 */
unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);                    /* 0x430e30 */
void bio_wr_c(struct VFSFILE *f, char c);                                        /* 0x4011f0 */
void bio_wr_v3(struct VFSFILE *f, int *v);                                       /* 0x4012d0 */

/* D3_ZERO (d3.h) with destination and size both named directly in the asm */
#define D3FL_ZERO_AT(var, size) {                                   \
    __asm mov edi, var                                              \
    __asm mov ebx, size                                             \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                               \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep stosb                                                 \
}

/* D3_COPY (d3.h) with the destination temp declared before the size temp */
#define D3FL_COPY(dst, src, size) {                                 \
    void *_cd = (dst); int _cn = (size);                            \
    __asm mov edi, _cd                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

/* the same with size, src and dst through temps, declared in this order (D3E_COPY3 in d3_engine.h) */
#define D3FL_COPY3(dst, src, size) {                                \
    int _cn = (size); void *_cs = (src); void *_cd = (dst);         \
    __asm mov edi, _cd                                              \
    __asm mov esi, _cs                                              \
    __asm mov ebx, _cn                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_COPY_LABEL(__LINE__)                               \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm D3_COPY_LABEL(__LINE__): add ecx, ebx                     \
    __asm rep movsb                                                 \
}

#endif
