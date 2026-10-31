struct D3SM;
struct D3OBJECT;
struct D3VECTOR;
/* Declarations of anm.c (0x4bb810..): animals (module tag "anm:animals", "DOG", "CAT", ..). Names are ours. */
#ifndef ANM_H
#define ANM_H

typedef struct ANMREC {                 /* 0x124 bytes, anm_Animals[32], an animal */
    void *obj;                          /* 0x00, its 3D object (->0x34->0x1ec, flags byte 0x96d) */
    int kind;                           /* 0x04, 0..7 */
    int nPoints;                        /* 0x08, kind 2: collected coordinates, 3 per building; -1 = not yet */
    unsigned char remove;               /* 0x0c, 1: to be removed (anm_RemoveAnimal) */
    char pad0d[3];
    float pos[3];                       /* 0x10, where it was created */
    char pad1c[4];
    unsigned int birth;                 /* 0x20, d3_Time of its anm_NewAnimal */
    char pad24[0x100];
} ANMREC;

extern char anm_KindNames[8][0x20];   /* 0x602688, "DOG", "CAT", "BIRD", "COW", "SHEEP", "CHICK", "PIG", "HORSE" */
extern int anm_NAnimals;                /* 0x6f0734, the number of animals */
extern ANMREC *anm_Animals;             /* 0x6f0738, the animal list (32 records, "anm:animals") */
extern int anm_NextAnimal;                /* 0x6f073c, the animal updated next */
extern unsigned int anm_LastSpawn;       /* 0x6f0740, d3_Time of the last spawn try */

/* views of other units (d3.h / ai.h do not compile here together) */
extern int d3_CurrentScene;             /* 0x633444 */
extern volatile unsigned long d3_Time;  /* 0x6333cc */
char d3if_SelectScene(int index, char keep);                                         /* 0x4478e0 */
extern char ai_Time[];                /* 0x770ae0, AITIME (ai.h) */
int gm_ClockSeason(void *t);                                                       /* 0x4fe350, 3: winter? */
struct CHARACTER *ch_Create(char *name, struct D3VECTOR *pos);                                         /* 0x403390, ch.c: CHARACTER *, D3VECTOR * */
struct CHACTION *ch_AddAction(struct CHARACTER *ch, unsigned int type, ...);                            /* 0x40b2f0, ch.c */
int ch_Delete(struct CHARACTER *ch);                                                         /* 0x4028a0, ch.c: CHARACTER * */
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                               /* 0x458a20, d3.h: D3 types */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);                                  /* 0x453e80, d3.h: D3VECTOR */
unsigned char * gm_BuildQuery(int n, ...);                                                  /* 0x501190, ai.h: AIBLD * */
unsigned char * gm_BuildNext(void);                                                        /* 0x501030 */
int gm_IsHomeType(unsigned char *b);                                                       /* 0x502520 */
int gm_IsType10(unsigned char *b);                                                       /* 0x5024f0 */
int ch_QueueAnim(struct CHARACTER *ch, char *name, int n);                                   /* 0x4057a0, ch.c */
char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);           /* 0x45f170, ch.h: D3SM *, D3VECTOR * */
extern struct D3SCENE *d3_DefaultScene; /* 0x5fb5b0 (as d3.h / unk_58bbd0.h) */
int util_Random(unsigned short n);                                               /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
float util_RandomF(void);                                                        /* 0x42ec20 */

void * mem_Alloc(int size, char *tag);                                            /* 0x4223c0, mem.c */
void mem_Free(void *p);                                                          /* 0x422620, mem.c */

ANMREC *anm_NewAnimal(void);
void anm_RemoveAnimal(ANMREC *a);
void anm_UpdateBird(ANMREC *a);
void anm_CreateCat(ANMREC *a);
void anm_CreateDog(ANMREC *a);
void anm_CreateSheep(ANMREC *a);
void anm_CreateCow(ANMREC *a);
void anm_CreateAnimal(ANMREC *a, int kind);
void anm_UpdateCat(ANMREC *a);
void anm_UpdateSheep(ANMREC *a);

#include "rd.h"
struct D3VECTOR;

void anm_Free(void);  /* 0x4bb840, frees the animal list */

#endif
