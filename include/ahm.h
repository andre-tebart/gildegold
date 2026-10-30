struct VFSFILE;
/* Declarations of ahm.c (0x4eaa50.., the alchemist's building / recipes; tag "ahm_AllocRecipePrototypes" 0x60f1c8..). Names are ours. */
#ifndef AHM_H
#define AHM_H

#pragma pack(push, 2)
typedef struct AHMCLOCK {               /* 14 bytes, AITIME (ai.h) copied as 3 ints and a short (as am.h's AITIMEX: the original copies it so) */
    int a;                              /* +0x0, the year */
    int b;                              /* +0x4, the hour */
    int c;                              /* +0x8 */
    short d;                            /* +0xc */
} AHMCLOCK;
#pragma pack(pop)

typedef struct AHMSLOT {                /* 8 bytes, a place of the record at +0x65 of the object (6 from +0xc): a good and its value */
    int id;
    float v;
} AHMSLOT;
typedef struct AHMREC {                 /* 0x3c bytes, the record the alchemist object keeps at +0x65 ("ahm_AllocAhmInfo") */
    int id;                             /* +0x0, the alchemist character id (0x4eaa50) */
    float value;                        /* +0x4, 0 .. 1, added to by an order (0x4ecdb0) */
    unsigned char share[4];             /* +0x8, 0 .. 252, the four shares / 252 (0x4eab20) */
    AHMSLOT slots[6];                   /* +0xc, the places */
} AHMREC;
#pragma pack(push, 1)
typedef struct AHMOBJ {                 /* the building object as ahm.c reads it (AIBLD, ai.h): the kind, the records and the object list */
    unsigned char kind;                 /* +0x0, index into ai_BTypes */
    int id;                             /* +0x1, the building id (AIBLD.id) */
    char pad05[0x58];
    void *objs;                         /* +0x5d, the head of the object instance list (gm_Query's list) */
    char pad61[4];
    AHMREC *rec;                        /* +0x65, its record */
} AHMOBJ;
typedef struct AHMGOOD {                /* an object of a building's object list as ahm.c reads it (AIOBJ, ai.h; what gm_Query finds) */
    unsigned short type;                /* +0x0 */
    int objId;                          /* +0x2, the object's own id (AIOBJ.objId) */
    char pad06[0x3d];
} AHMGOOD;
typedef struct AHMRESEARCH {            /* the fields ahm.c reads of a research event record (ai.h AIJOB: the kind at +0, unkb0 .. unkb8) */
    unsigned char kind;                 /* +0x0, the event type (0x95 .. 0x97) */
    char pad01[0x51];
    AHMCLOCK due;                       /* +0x52, when the event is due (0x4eb410 copies the game clock 0x770ae0 over it) */
    char pad60[0x50];
    int id;                             /* +0xb0, the recipe / share the research is about */
    int amount;                         /* +0xb4, the research amount left (the event counts it down by the elapsed minutes) */
    int total;                          /* +0xb8, the amount it started with */
    int start;                          /* +0xbc, the amount the event 0x96 / 0x97 was planned with */
} AHMRESEARCH;
typedef struct AHMCHAR {                /* the character fields ahm.c reads (AICHAR, ai.h) */
    char pad00[4];
    int id;                             /* +0x4, the character id (the 0x505df0 key) */
    char pad08[0x164];
    AHMOBJ *obj;                        /* +0x16c, the character's object */
} AHMCHAR;
typedef struct AHMORDER {               /* 0x18 bytes, the data of a type 100 message (cm.c's 0x4cd010): one of the commands below */
    int tag;                            /* +0x0, the four-letter command */
    int charId;                         /* +0x4, the id of the character it is for */
    int id;                             /* +0x8, the recipe / place id it is about */
    int unk0c;                          /* +0xc, not used */
    union { float f; int i; } value;    /* +0x10, the value ('SmSa': the bits of a float) */
    int nextObjectId;                   /* +0x14, the object id the receiver continues counting from (ahm_AddPlace 0x4ecb30 gives it to ai_NextObjectId in a network game); the senders here leave it 0 */
} AHMORDER;
#define AHM_CMD_SET_SHARE 0x61437353    /* 'SsCa', sets the share at +8 of the record (0x4eca00) */
#define AHM_CMD_ADD_PLACE 0x61616452    /* 'Rdaa', adds a place from the recipe table (0x4ecb30) */
#define AHM_CMD_SET_PLACE 0x61535263    /* 'cRSa', sets the value of a place (0x4ecd10) */
#define AHM_CMD_ADD_VALUE 0x61536d53    /* 'SmSa', adds to the value at +4 of the record (0x4ecdb0) */
#define AHM_CMD_DELETE    0x46697245    /* 'ErFi', deletes the alchemist (0x4ece60) */
typedef union AHMSEED {                 /* 0x6f0ddc, the seed of the CRT generator (its low word is saved) */
    int seed;                           /* the seed as the shuffler reads it */
    unsigned short seed16;              /* the low word written on a restart */
} AHMSEED;
#pragma pack(pop)
typedef union AHMGRID {                 /* 12 bytes, an entry of the shuffled tables 0x6f0e2c (players) / 0x6f0e34 (recipes) */
    int w[3];                           /* the three words (ahm_FillSteps copies them into a walk step) */
    struct {                            /* the player prototype (0x6f0e2c) */
        unsigned char attr[5];          /* +0x0, ATTR0 .. ATTR3 (0 .. 252) and ATTR4 (18 .. 30) */
        char pad05[3];
        int id;                         /* +0x8, ATTR5: the object id */
    } plr;
    struct {                            /* the recipe prototype (0x6f0e34) */
        int id;                         /* +0x0, RCPID: the object id */
        float a, b;                     /* +0x4 / +0x8, ATTR0 / ATTR1, 0 .. 1 */
    } rcp;
} AHMGRID;
typedef struct AHMSTEP {                /* 0x18 bytes, 0x6f0de0 (3 of them): a step of the grid walk */
    int on;                             /* +0x0, 1 while the step is in use */
    AHMGRID grid;                       /* +0x4, the table entry the step took (ahm_FillSteps copies its three words) */
    int x, y;                           /* +0x10 / +0x14, its position on the 25 x 25 grid (also the name indexes 0x144a7a8 / 0x144a80c) */
} AHMSTEP;
typedef struct AHMCREATEMSG {           /* 0x24 bytes, the data of the type 99 message (0x4c6170; the handler 0x4ccf90 calls ahm_CreateAlchemist) */
    int seed;                           /* +0x0, the generator seed; the handler 0x4ccf90 writes its own rand() when 0x1460ae0 is -1, unwritten here */
    int charId;                         /* +0x4, the character id the handler 0x4ccf90 syncs ai_NextObjectId from, unwritten here */
    int building;                       /* +0x8, the id of the alchemist building (AHMOBJ.id) */
    AHMGRID proto;                      /* +0xc, the step's prototype entry (the player view: its attributes and its place id) */
    int x, y;                           /* +0x18 / +0x1c, the name indexes of the creation (0x144a7a8 / 0x144a80c) */
    int value;                          /* +0x20, the +8 of the recipe entry of the prototype's id (the bits of its float) */
} AHMCREATEMSG;

extern char *ai_BTypes;              /* 0x7e3f30 */
extern int ai_PlayerPrototypeCount;                /* 0x6f0e28 */
extern AHMGRID *ai_PlayerPrototypes;           /* 0x6f0e2c */
extern int ai_RecipePrototypeCount;                /* 0x6f0e30 */
extern AHMGRID *ai_RecipePrototypes;           /* 0x6f0e34 */
extern int ai_WalkX, ai_WalkDX, ai_WalkY, ai_WalkDY, ai_WalkIndex;  /* 0x6f0dc8, 0x6f0dcc, 0x6f0dd0, 0x6f0dd4, 0x6f0dd8 */
extern AHMSEED ai_WalkSeed;             /* 0x6f0ddc */
extern AHMSTEP ai_WalkSteps[3];         /* 0x6f0de0 */
extern int ahm_Tab[8];                  /* 0x60f1b0 */
extern AHMCLOCK ai_Time; /* 0x770ae0, the game clock (ai.h's AITIME), @original: ahm.c copies it as 3 ints and a short (0x4eb43a lea edx,[eax+0x52]; stores 4/4/4/2), against anm.h / cm.h / f3snd.h's char[14] view */
short * gm_FindChar(int id);             /* 0x505df0 */
char * he_FindEvent(int n, ...);                   /* 0x53e400 */
char * he_NextEvent(void);                   /* 0x53e300 */
void cm_SendExSetHE(int k, char *p); /* @original: ahm.c pushes the key as an int (0x4eb43e push 0xfffffffe), against cm.h's unsigned char view */

char *ahm_OwnerChar(unsigned char *p);  /* 0x4eaa50, the character the alchemist object p belongs to */
int ahm_UsedSlots(char *c);             /* 0x4eaa90, the number of the places in use in the record of c's object */
int ahm_HasRecord(char *c);             /* 0x4eaad0, 1 when the character c has an alchemist object with a record */
float ahm_Share(char *c, int i);        /* 0x4eab20, the share i (0 .. 3) of the record of c's object, -1 when none */
AHMSLOT *ahm_SlotOf(char *c, int id);   /* 0x4eabd0, the place of c's record with the id (forwards to ahm_FindSlot) */
AHMSLOT *ahm_FindSlot(char *c, int id); /* 0x4eabe0, the place of c's record with the id, 0 when there is none */
int ahm_HasFreeSlot(char *c);           /* 0x4eac20, 1 when a place of c's record is still free (the share 2 sets the limit of six) */
void ahm_FillSteps(void);               /* 0x4eac60, fills the three walk steps from the shuffled table */
void ahm_ShufflePlayers(int init);      /* 0x4ead20, shuffles the player prototype table (init: reseed and restart the walk at random grid positions) */
void ahm_ShuffleTable(void);            /* 0x4eadc0, shuffles the player prototype table with random swaps */
AHMGRID *ahm_FindRecipe(int id);        /* 0x4eae60, the entry of the recipe prototype table with the id, 0 when there is none */
AHMGRID *ahm_RandomRecipe(char *c);     /* 0x4eaea0, a random recipe the character c has no place for yet, 0 when it may not take more */
int ahm_ResearchTime(char *c, AHMGRID *g); /* 0x4eaf50, the research time of a recipe in tenths of minutes */
int ahm_ResearchMinutes(char *c, AHMGRID *g); /* 0x4eafa0, the same in minutes */
float ahm_ResearchProgress(char *c, AHMGRID *g, int *cap);  /* 0x4eafd0, the progress made in *cap minutes (which get the minutes used) */
float ahm_ShareProgress(char *c, int i, int *cap);  /* 0x4eb0b0, the same for the share i */

float ahm_PlaceFactor(char *c, int id);   /* 0x4eb130 */
int ahm_ResearchState(char *o, int *a, float *f, int flag);  /* 0x4eb180 */
void ahm_RescheduleResearch(char *o);            /* 0x4eb410 */
int ahm_LoadPrototypes(int mode);            /* 0x4eb480 */

int ahm_LoadIni(void);                /* 0x4eb4c0 */
int ahm_LoadDfn(void);                /* 0x4eba80 */
int ahm_SaveDfn(void);                /* 0x4ebd60 */
int ahm_ObjectId(char *name);          /* 0x4ebf50 */
void ahm_FreePrototypes(void);               /* 0x4ebf60 */
AHMSTEP *ahm_WalkStep(int i);          /* 0x4ec2d0 */
void ahm_ClearStep(AHMSTEP *p);         /* 0x4ec2f0 */
extern void *main_Instance;              /* 0x75e140, the module handle */
extern struct VFSDIR *vfs_Root;         /* 0x6333fc, the VFS directory tree (vfs.h) */
void * mem_Alloc(int size, char *tag);  /* 0x4223c0 */
void mem_Free(void *p);                 /* 0x422620 */
int gm_FindById();                     /* 0x4fea50 */
char * gm_Query(void *list, int n, ...);                   /* 0x4ffe90 */
int gm_RemoveObject(int *list, unsigned item);                     /* 0x4ffff0 */
int gm_PlaceOnStage(int id, int a, int b, int *p4, int *p5);                     /* 0x4fbd10 */
int gm_ChooseHeadIcon(unsigned short *c);                     /* 0x4fb890 */
void gm_RefreshGoodsLists(unsigned p);                     /* 0x4ff190 */
int gm_AddObjekt(int id, int proto, int n);                     /* 0x500970 */
void unk_Unk503c60(int c, int mode);                     /* 0x503c60 */
void cm_SendCommand(void *p);                     /* 0x4c61b0 */
float util_RandomSq(void);              /* 0x42ec40 */
extern char ai_Chars[][0x21c];      /* 0x770b80, the characters */
extern signed char ai_Relations[768][768];  /* 0x7e5360 */
extern signed char ai_RelationMemory[768][768];  /* 0x889f80 */
extern int ai_CharCount;                /* 0x6f1bc0, the number of characters */
extern unsigned char ai_OwnPlayer;      /* 0x6f1bc5 */
extern int ai_NextObjectId;                /* 0x61a654 */
extern int ai_Unk1460ae0;               /* 0x1460ae0 */



int ahm_SaveWalk(void *f);             /* 0x4ebfb0 */
int ahm_LoadWalk(void *f);             /* 0x4ec140 */
int ahm_SendCreateAlchemist(unsigned char *o, AHMSTEP *s);  /* 0x4ec490, posts the type 99 message that creates the alchemist's owner character from the step */
int ls_Read(void *buf, int size, int count, int f);  /* 0x51c920, ls.c's read (true when the fread returned a positive count); ls.c has no header yet */
void cm_SendCreateAlchemist(void *p);                     /* 0x4c6170 */

int ahm_StepValue(char *p);             /* 0x4ec320 */
int ahm_ObjectValue(unsigned char *p);    /* 0x4ec3d0 */
unsigned short ahm_CreateAlchemist(int *p);   /* 0x4ec500 */
int ahm_SendSetShare(char *c, int id, float v);  /* 0x4ec940 */
int ahm_SetShare(int *p, char *rec);   /* 0x4eca00 */
int ahm_SendAddPlace(char *c, int id);     /* 0x4ecaa0 */
int ahm_AddPlace(int *p, char *rec, unsigned char *o);  /* 0x4ecb30 */
int ahm_RemovePlace(int *p, char *rec, unsigned char *o);  /* 0x4ecbc0 */
int ahm_SendSetPlaceValue(char *c, int id, float v);  /* 0x4ecc30 */
int ahm_SetPlaceValue(int *p, char *rec);   /* 0x4ecd10 */
int ahm_SendAddValue(char *c, int v);      /* 0x4ecd50 */
int ahm_AddValue(int *p, char *rec);   /* 0x4ecdb0 */
int ahm_SendDelete(char *c);             /* 0x4ece10 */
int ahm_WriteRecord(void *f, unsigned char *o);  /* 0x4ecf10 */
int ahm_ReadRecord(void *f, unsigned char *o);  /* 0x4ed040, reads the alchemist record of o (+0x65) from the file f */
int ahm_DeleteAlchemist(int unused, int *rec, unsigned char *o);  /* 0x4ece60 */

#endif
