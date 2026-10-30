/* vfs.c types: the virtual file system ("vfs_Tfopen", "vfs:fseek(...)" strings). Files are looked up
   in a directory tree that covers the real directories and the contents of zip archives; an open file
   reads from a plain FILE, a gzFile, a file inside a zip (unzip), or a memory buffer (raw or deflated).
   Field names unkXXX are byte offsets whose meaning is not known yet. */
#ifndef UNK_430130_H
#define UNK_430130_H

#include <stdio.h>
#include "zlib.h"
#include "unzip.h"
struct VFSFILE;

typedef struct VFSFLAGS {           /* 1 byte */
    unsigned char read : 1;         /* bit 0, opened for reading */
    unsigned char binary : 1;       /* bit 1 */
    unsigned char checkgz : 1;      /* bit 2, the first read from a zip checks for a gz header */
    unsigned char compressed : 1;   /* bit 3, gzio (file) or deflate/inflate (memory) */
    unsigned char inzip : 1;        /* bit 4, a file inside a zip archive */
    unsigned char memory : 1;       /* bit 5, a memory buffer */
} VFSFLAGS;

typedef struct VFSFILE {            /* 0x140 bytes, from vfs_FilePool */
    char name[0x100];
    union {                         /* 0x100 */
        FILE *fp;
        gzFile gz;
        unzFile zip;
        z_stream zs;                /* memory buffer: next_in/avail_in/total_in when reading,
                                       next_out/avail_out/total_out when writing */
    };
    uLong crc;                      /* 0x138, crc32 of the deflated data */
    VFSFLAGS flags;                 /* 0x13c */
} VFSFILE;

typedef struct VFSDIR VFSDIR;

typedef struct VFSENTRY {           /* 0x18 bytes, a file of a finished directory (VFSDIR.entries, sorted) */
    char *name;                     /* stored after the records in the same allocation */
    uLong date;                    /* 0x04, dos date/time; -1: written by us */
    VFSDIR *zip;                    /* 0x08, nonzero: inside a zip archive (the archive's directory) */
    uLong num;                    /* 0x0c, unzGoToFilePos num_file */
    uLong pos;                    /* 0x10, unzGoToFilePos pos_in_central_dir */
    int ext;                        /* 0x14, index into vfs_Ext */
} VFSENTRY;

typedef struct VFSNODE VFSNODE;
struct VFSNODE {                    /* 0x118 bytes, from vfs_EntryPool: a file of a directory being filled */
    char name[0x100];
    uLong date;                     /* 0x100 */
    VFSDIR *zip;                    /* 0x104 */
    uLong num;                      /* 0x108 */
    uLong pos;                      /* 0x10c */
    int ext;                        /* 0x110 */
    VFSNODE *next;                  /* 0x114 */
};

struct VFSDIR {                     /* 0x11c bytes (vfs_IGetSubDir) */
    char name[0x100];
    int nentries;                   /* 0x100 */
    VFSENTRY *entries;              /* 0x104, allocated by vfs_FinishedFiles / vfs_AddFile */
    VFSDIR *next;                   /* 0x108 */
    VFSDIR *child;                  /* 0x10c */
    VFSDIR *parent;                 /* 0x110 */
    VFSNODE *files;                 /* 0x114, until vfs_FinishedFiles */
    char sorted;                    /* 0x118, files moved to entries */
};


/* Inline-asm copy of the memory files (as D3_COPY in d3.h); all three operands are named variables. */
#define VFS_COPY_CAT(a, b) a##b
#define VFS_COPY_LABEL(n) VFS_COPY_CAT(vfs_copy_tail, n)
#define VFS_COPY(dst, src, size) {                                  \
    __asm mov edi, dst                                              \
    __asm mov esi, src                                              \
    __asm mov ebx, size                                             \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle VFS_COPY_LABEL(__LINE__)                              \
    __asm rep movsb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep movsd                                                 \
    __asm VFS_COPY_LABEL(__LINE__): add ecx, ebx                    \
    __asm rep movsb                                                 \
}

extern VFSDIR *vfs_Root;           /* 0x6333fc */
struct MPOOL;
extern struct MPOOL *vfs_EntryPool; /* 0x633400 */
extern struct MPOOL *vfs_FilePool;  /* 0x633404 */
extern char vfs_IgnoreCase;          /* 0x633408 */
extern int vfs_OpenCount;           /* 0x63340c */
extern const char vfs_Ext[][6];           /* 0x5eba40, ".BIN5", ".BIN4", ... */

void err_Message(char *msg);                                                            /* 0x4222b0 */
void * mem_Alloc(int size, char *tag);                                                   /* 0x4223c0 */
void mem_Free(void *p);                                                                 /* 0x422620 */
void *pool_Alloc(struct MPOOL **pool, unsigned int size, int count);                     /* 0x422a20, zeroed (mem.h) */
void pool_Free(struct MPOOL **pool, void *ptr);                                                  /* 0x422b30 */
void pool_Destroy(struct MPOOL **pool);                                                          /* 0x422c60 */

int vfs_strcmp(char *a, char *b);                                                       /* 0x42eca0, stricmp if vfs_IgnoreCase */
char *vfs_UpperCase(char *path);                                                        /* 0x42ecf0, strupr unless vfs_IgnoreCase */
void vfs_ToSlashes(char *path);                                                         /* 0x42ed10, '\\' -> '/' */
void vfs_ToBackslashes(char *path);                                                         /* 0x42ed40, '/' -> '\\' */
VFSDIR *vfs_IGetSubDir(char *name, VFSDIR *parent);                                     /* 0x42ed70, leaked name */
int vfs_CmpName(const char *name, const VFSENTRY *e);                                   /* 0x42ee10, strcmp with the entry's name */
int vfs_CmpNameI(const char *name, const VFSENTRY *e);                                  /* 0x42ee50, stricmp with the entry's name */
VFSENTRY *vfs_FindEntry(char *name, VFSDIR *d, VFSDIR **found, char recurse);           /* 0x42ee60 */
void vfs_AddFile(char *name, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, unsigned int ext, uLong date); /* 0x42ef10 */
void vfs_AddFilePath(char *path, VFSDIR *d, VFSDIR *zip, uLong pos, uLong num, int ext, uLong date); /* 0x42f4d0 */
int vfs_CmpEntry(const VFSENTRY *a, const VFSENTRY *b);                                 /* 0x42f5c0, by name */
int vfs_CmpEntryI(const VFSENTRY *a, const VFSENTRY *b);                                /* 0x42f600, by name, ignoring case */
void vfs_FinishedFiles(VFSDIR *d, char recurse);                                        /* 0x42f620 */
struct VFSDIR * vfs_FindDir(char *path, struct VFSDIR *d);                                             /* 0x42f7e0 */
struct VFSENTRY * vfs_FindFile(char *path, struct VFSDIR *d, struct VFSDIR **found);                         /* 0x42f8f0 */
char *vfs_EntryPath(VFSENTRY *e, VFSDIR *d, VFSDIR *stop, char *out);                   /* 0x42fa10 */
char *vfs_DirPath(VFSDIR *d, VFSDIR *stop, char *out);                                /* 0x42fbf0 */
char * vfs_GetPath(char *name, struct VFSDIR *d, struct VFSDIR *stop, char *out);                      /* 0x42fca0 */
VFSDIR *vfs_AddDir(char *path, VFSDIR *parent);                                      /* 0x42fce0 */

void vfs_AddZip(char *path, VFSDIR *d, int ext);                                       /* 0x430130 */
void vfs_FreeDir(VFSDIR *d);                                                            /* 0x430260 */
char vfs_MakeDirs(char *path, int unk);                                                 /* 0x4302e0 */
VFSFILE *vfs_Tfopen(char *name, char *mode, int unk);                                   /* 0x430470 */
int vfs_fclose(VFSFILE *f);                                                             /* 0x430aa0 */
int vfs_fread(void *buf, int size, int n, VFSFILE *f);                                  /* 0x430bd0 */
int vfs_fgetc(VFSFILE *f);                                                              /* 0x430d90 */
char *vfs_fgets(char *buf, int n, VFSFILE *f);                                          /* 0x430dc0 */
unsigned vfs_fwrite(void *buf, int size, int n, struct VFSFILE *f);                     /* 0x430e30, writes n items of size bytes to the file (the save game writer) */
int vfs_fseek(struct VFSFILE *f, long offset, int origin);                                     /* 0x430fa0 */
long vfs_ftell(VFSFILE *f);                                                             /* 0x431170 */
VFSFILE *vfs_MemOpen(unsigned char *buf, unsigned int size, char *mode);                /* 0x4311d0 */
char vfs_Init(char *root, char unk);                                                    /* 0x431490 */
void vfs_Exit(char warn);                                                               /* 0x431510 */

#endif
