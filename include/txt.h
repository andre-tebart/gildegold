struct VFSFILE;
/* txt.c -- text file definitions (txt_*): loads the game's text tables from a definition file.
   Original file name unknown; leaked names are the strings "txt_ParseRandomText" and "txt_OpenText".
   Starts at 0x42c3b0, after the sound output (sfx.c) and the mss32 import thunk at 0x42c3a0.
   Names are ours except those two. */
#ifndef UNK_42C3B0_H
#define UNK_42C3B0_H

extern char txt_Language[0x40];         /* 0x5f8ad0, the language name (General/Language of game.INI, default "german") */
extern int txt_GroupCount;              /* 0x6333bc, text groups parsed by the last txt_OpenText, never read */
extern char *txt_Buffer;                /* 0x6333c0, mem_Alloc'd text pool */
extern int txt_Size;                    /* 0x6333c4, bytes used in txt_Buffer */
extern char txt_BinPath[256];           /* 0xfe7c40 */
extern char txt_Dir[256];               /* 0xfe7d40 */
extern char txt_Keys[0x4800][0x50];     /* 0xfe7e40 */
extern char txt_Names[0x4800][0x50];    /* 0x12c9fa0, indexed by group - 1 */
extern int txt_GroupFirst[0x4800];      /* 0x144c1e0, first text of each group */
extern char txt_HeaderName[0x120];      /* 0x12c9e80, "#headerfile" */
extern char txt_GroupIds[0x4800][0x50]; /* 0x1161e80, the "_..." identifier of each group */
extern char *txt_Ptrs[0x4800];          /* 0x143a1c0, each text in its file's buffer */
extern unsigned char txt_GroupLevel[0x4800]; /* 0x14359c0, label column of each group */

typedef struct TXTFILE {                /* 0x74 bytes, one per included text file */
    char name[0x60];
    int first;                          /* 0x60 */
    int last;                           /* 0x64 */
    char *buf;                          /* 0x68 */
    int unk6c;
    int unk70;
} TXTFILE;

extern TXTFILE txt_Files[0x80];         /* 0x1431fc0 */

void * mem_Alloc(int size, char *tag);                                                   /* 0x4223c0 */
void mem_Free(void *p);                                                                 /* 0x422620 */
void err_Message(char *text);                                                           /* 0x4222b0 */

char *txt_Field(char *p, int n, char type);                                             /* 0x42c3b0 */
char *txt_SkipSpaces(char *s);                                                          /* 0x42c430 */
void txt_TrimRight(char *s);                                                            /* 0x42c480 */
int txt_ParseLine(char *line, int group);                                               /* 0x42c4b0 */
void txt_ParseRandomText(char *p);                                                      /* 0x42c950 */
int txt_LoadIncludes(char *file);                                                       /* 0x42c9c0 */
int txt_OpenText(char *file, int mode);                                                 /* 0x42cbc0 */
void txt_FreeFiles(void);                                                               /* 0x42e2a0 */
int txt_FindFile(char *name);                                                           /* 0x42e2e0 */
void txt_FreeFile(char *name);                                                          /* 0x42e320 */
int txt_ReloadTextFile(char *name);                                                     /* 0x42e360, leaked name */
int txt_LoadTextFile(char *name);                                                       /* 0x42e530, leaked name */
int txt_FindKey(char *key);                                                             /* 0x42e770 */

/* The virtual file system (vfs.c); the file handle is a VFSFILE * there. */
extern struct VFSDIR *vfs_Root;                  /* 0x6333fc */
struct VFSFILE * vfs_Tfopen(char *name, char *mode, int unk);                                    /* 0x430470 */
int vfs_fclose(struct VFSFILE *f);                                                                /* 0x430aa0 */
int vfs_fread(void *buf, int size, int n, struct VFSFILE *f);                                     /* 0x430bd0 */
int vfs_fseek(struct VFSFILE *f, long offset, int origin);                                        /* 0x430fa0 */

#endif
