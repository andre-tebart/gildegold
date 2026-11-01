struct D3ANIM;
struct D3FLMAP;
struct SBSAMPLE;
struct SBBANK;
struct D3VECTOR;
struct D3TERRAIN;
struct D3SM;
struct XOBJ;
struct D3SURF;
/* ch.c -- characters ("ch_SetVisible(): invalid character", "ch_Create:Too many characters!"),
   original file name unknown. Starts at 0x402140 and continues past 0x4028a0. Field names unkXXX are
   byte offsets whose meaning is not known yet. Names are ours. */
#ifndef UNK_402140_H
#define UNK_402140_H
struct D3OBJECT;

#define CH_MAX 768

typedef struct CHANIM {             /* what CHARACTER.anim points to */
    char name[0x10];                /* name[9] == '-': the animation moves the character itself */
} CHANIM;

struct CHACTION;

typedef struct CHARACTER {         /* 0x244 bytes */
    int id;                         /* index in ch_List, -1 once deleted */
    int kind;                       /* 0x04, 1 person, 2 animal (| 8 horse) */
    char name[0x20];                /* 0x08, mesh name */
    int scriptId;                   /* 0x28, id of the event script the character runs; -1 = none (evt_Stop clears it) */
    int group;                      /* 0x2c, a group id, compared with evt_NoGroup and with a script's group */
    int scriptGroup;                /* 0x30, the group of the script that created / moved the character (chc.c) */
    D3OBJECT *obj;                  /* 0x34, the body */
    char pad038[0x1c];
    D3VECTOR offset;                /* 0x54, added to the body position */
    char pad060[0x4];
    D3OBJECT *rightHand;            /* 0x64, the object in the right hand (ch_SetHandObject hand 2, ch_SetVisible) */
    char pad068[0x8];
    struct D3ANIMSLOT *slot;        /* 0x70, current animation */
    char pad074[0x4];
    D3ANIM *turnAnim;               /* 0x78, the path animation a turn action created (CH_ANIM078) */
    struct D3ANIMSLOT *morph;       /* 0x7c, blend between two animations ("morph_%i") */
    struct D3ANIMDATA *next;        /* 0x80, animation to start next */
    char nextloops;                 /* 0x84 */
    char lastActionFrame;           /* 0x85, the slot flags.b[0] value a->func2 last ran for, -1: not yet */
    char pad086[0x2];
    D3SCENE *scene;                 /* 0x88 */
    union {                         /* 0x8c */
        struct {
            unsigned short dirty : 1;   /* position changed */
            unsigned short unk1 : 1;    /* 0x02 */
            unsigned short unk2 : 3;
            unsigned short hidden : 1;
            unsigned short fading : 1;
            unsigned short lowPolyAnimFailed : 1;   /* the low-poly animation could not be attached */
            unsigned short unk8 : 1;    /* 0x100 */
            unsigned short lowPoly : 1; /* 0x200, the low-poly body is shown (chc_SetLowPoly) */
            unsigned short unk10 : 2;
            unsigned short unk12 : 1;   /* 0x1000 */
            unsigned short unk13 : 3;
        };
        unsigned short all;             /* 0x8c, the same bits as a word */
        struct {
            unsigned char lo;           /* 0x8c */
            unsigned char hi;           /* 0x8d */
        } bytes;
    } flags;
    unsigned char contactSteps;     /* 0x8e, path cells the contacts table stays valid for: ch_GetNewPath
                                       sets it to 4 after a path was built around blockers, ch_NextWaypoint
                                       decrements it per cell and clears contacts when it reaches 0; > 0
                                       suppresses a new path search */
    char pad08f;
    struct CHCONTACT {              /* 0x90, characters met on the way (worker ch4) */
        struct CHARACTER *ch;
        int flags;                  /* ch_GetNewPath result */
    } contacts[16];
    char pad110[0x14];
    D3OBJECT **transport;              /* points to a CHTRANSPORT */
    union {                         /* 0x128 */
        CHANIM *anim;               /* the animation file in use */
        struct CHACTION *actions;   /* the action queue of the character */
    };
    char pad12c[0x4];
    char body[0x40];                /* 0x130, the part of the name after the last but one '_' */
    char fullname[0x30];            /* 0x170 */
    float unk1a0;
    float speedScale;               /* 0x1a4, walk animation speed scale 0.4 .. 1: a factor of
                                       CH_XSLOT(ch)->playSpeed, lowered by ch_GetNewPath the closer a blocking
                                       character is (0.5 minimum) and raised again by 0.01 per walk update */
    char pad1a8[0x80];
    void (*unk228)(struct CHARACTER *ch);
    D3OBJECT *lowPolyBody;          /* 0x22c, low-poly body */
    char lowname[0x10];             /* 0x230 */
    unsigned char pathType;         /* 0x240, the path_Find unit type (selects the terrain cost table
                                       path_Terrain[type] for this character; all three readers pass it
                                       straight to path_Find's type argument) */
    char pad241[0x3];
} CHARACTER;

typedef struct CHTRANSPORT {        /* 0x18 bytes, what CHARACTER.transport points to */
    D3OBJECT *obj;
    D3VECTOR pos;                   /* 0x04, character position at the last update */
    int unk10;
    int unk14;
} CHTRANSPORT;
#define CH_TRANSPORT(ch) ((CHTRANSPORT *)(ch)->transport)

typedef struct CHFADE {             /* 12 bytes, 16 of them in ch_Fades */
    CHARACTER *ch;
    char in;                        /* 1: fade in, 0: fade out */
    int start;                      /* 0x08, ch_Time when the fade started */
} CHFADE;

/* the scene field at 0x3d8 (inside D3SCENE.pad3d6) */
#define D3SCENE_UNK3D8(s) (*(unsigned int *)((char *)(s) + 0x3d8))

extern CHARACTER *ch_List[CH_MAX];  /* 0x1468ae0 */
extern int ch_Time;                 /* 0x631054 */
extern unsigned int d3_SceneFlags;   /* 0x63349c */

int d3t_SceneIndex(D3SCENE *scene);                                                     /* 0x411be0 */
char d3_DeflateScene(unsigned int flags);                                                  /* 0x43db30 */
char d3_InflateScene(unsigned int flags);                                                  /* 0x43deb0 */
void d3_SetObject078(D3OBJECT *obj, D3VECTOR *v);                                       /* 0x43b820 */
char d3if_Show(struct D3OBJECT *obj, char show);                                               /* 0x4471b0 */
char d3if_ShowTree(D3OBJECT *obj, char show);                                             /* 0x447300 */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);                           /* 0x458810 */
float d3_VecAngle(D3VECTOR *a, D3VECTOR *b);                                            /* 0x4539f0 */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */
struct D3SM * d3sm_Create(struct D3TERRAIN *t, int size, int mask);                          /* 0x45eeb0 */
void err_Message(char *msg);                                                            /* 0x4222b0 */
int d3t_ReleaseObjectTextures(D3OBJECT *obj);                                           /* 0x411b70 */
int sv_ReturnZero(void);                                                       /* 0x52a040, @original: takes nothing (sv.c), but ch_SetLowPoly pushes ch (0x402b6b: push esi; call); creates the low-poly body? */
extern int ch_LowPoly;              /* 0x631064, 2 - the ch_SetLowPoly mode */
struct D3OBJECT; struct D3IFSCENE; char d3if_MoveObject(struct D3OBJECT *obj, struct D3IFSCENE *from, struct D3IFSCENE *to);                           /* 0x447fd0, moves obj to another scene */
void vfs_ToSlashes(char *path);                                                         /* 0x42ed10 */
void d3anim_ApplyRootMotion(D3OBJECT *obj, D3ANIMSLOT *s, int from, int to, int flags);       /* 0x4315c0 */
/* D3ANIMFLAGS as built for d3anim_StartAnim here: field-wise, so VC6 keeps the dword at 4 in a register */
typedef struct CHANIMFLAGS {
    union {
        int all;
        struct { unsigned char b0, b1, b2, b3; } c;     /* b1: 0x10 loop, 0x08 (D3ANIMFLAGS.b[1]) */
    } u;
    unsigned int a : 1, b : 1, r : 30;                  /* 0x04 */
    unsigned char a8 : 1, b8 : 1, r8 : 6;               /* 0x08, b8 = D3ANIMSLOT.play */
} CHANIMFLAGS;
/* d3.h declares this one with another type (d3_Unk44e4a0); ch.c hides that prototype */
struct D3IOANIMDATA *d3_LoadAnimStream(char *name, char *file, char unk);                              /* 0x44e4a0, loads an animation, d3_io_t.c */

/* bits of the D3OBJECT flag bytes 0x210..0x213 (by offset; d3_engine.h's D3OBJBITS names the bits of 0x210) */
typedef struct CHOBJBITS {
    unsigned char b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1;
} CHOBJBITS;
#define D3OBJ_BITS(obj, off) ((CHOBJBITS *)((char *)(obj) + (off)))
float d3_YawTo(struct D3OBJECT *obj, float *p);                                     /* 0x448ac0, angle towards target */
#define D3OBJ_UNK048(obj) (*(int *)&(obj)->name[0x48])
#define D3OBJ_UNK200(obj) (*(int *)&(obj)->pad200[0])
#define D3MATERIAL_UNK958(m) (*(float *)((char *)(m) + 0x958))
/* D3OBJECT.info is the D3ANIMCTL of animated objects */
#define D3OBJ_ANIM(obj) ((D3ANIMCTL *)(obj)->info)

#include "ch_types.h"                /* CHMOTION */
extern CHMOTION ch_Motions[];       /* 0x14696e0 */
extern char ch_ShadowEnabled;           /* 0x5f122c */
extern int ch_CurrentGroup;         /* 0x5f1294, the group of the building the player is in (sv.c / f3init.c
                                       write it from the building record); -1 = none, ch_Reset sets both */
extern int ch_CurrentScriptGroup;   /* 0x5f1298, the script group that goes with it; -1 = none */
extern int ch_MorphTimeout;         /* 0x5f129c (-1), the D3ANIMSLOT.timeout of morphs in scene != 0 */
extern int ch_MotionTimeout;        /* 0x5f12a0 (2), the same for motions */

struct D3HMAP *ch_GetHMap(CHARACTER *ch);                                               /* 0x402140 */
struct D3HMAP *ch_GetSceneHMap(D3SCENE *scene);                                         /* 0x402220 */
void ch_UpdateFades(void);                                                              /* 0x4022e0 */
void ch_SetVisible(CHARACTER *ch, int show);                                            /* 0x402480 */
void ch_Unk402540(CHARACTER *ch);                                                       /* 0x402540 */
void ch_SetPos(CHARACTER *ch, D3VECTOR *pos, int unk);                                  /* 0x4025c0 */
void ch_PlaceAtObject(CHARACTER *ch, D3OBJECT *obj);                                    /* 0x402640 */
int ch_Count(int scene, int all);                                                       /* 0x4026c0 */
int ch_CountAt(int scene, int all, D3VECTOR *pos, float eps);                           /* 0x402720 */
void ch_Unk4027b0(void);                                                                /* 0x4027b0 */
void ch_UpdatePositions(void);                                                          /* 0x402800 */
int ch_Delete(CHARACTER *ch);                                                           /* 0x4028a0 */
CHARACTER *ch_Alloc(void);                                                              /* 0x4029c0 */
CHARACTER *ch_AllocAt(int id);                                                          /* 0x402a30 */
CHARACTER *ch_FindByName(char *name);                                                   /* 0x402a70 */
CHARACTER *ch_FindByObject(D3OBJECT *obj);                                              /* 0x402ac0 */
void ch_SetLowPoly(unsigned char mode);                                                 /* 0x402af0 */
void ch_UpdateLowPoly(CHARACTER *ch);                                                   /* 0x402ba0, ch_Updatelowpoly */
int ch_CreateMesh(CHARACTER *ch, char *name, D3VECTOR *pos);                            /* 0x403070 */
CHARACTER *ch_Create(char *name, D3VECTOR *pos);                                        /* 0x403390 */
int ch_MoveToScene(CHARACTER *ch, D3SCENE *scene);                                      /* 0x4033c0, ch_MoveCharacter2Universe */
CHTRANSPORT *ch_AttachTransport(CHARACTER *ch, D3OBJECT *obj);                          /* 0x4034e0 */
void ch_DetachTransport(CHARACTER *ch);                                                 /* 0x4035f0 */
void ch_PlaceTransport(struct CHARACTER *ch, char now);                                             /* 0x403620 */
struct D3ANIMSLOT *ch_AttachMotion(CHARACTER *ch, int motion);                          /* 0x403950 */
int ch_AnimDone(CHARACTER *ch);                                                         /* 0x403ab0 */
struct D3ANIMDATA *ch_SetNextAnim(CHARACTER *ch, char *name, char loops);              /* 0x403b00 */
void ch_StopAnim(CHARACTER *ch);                                                        /* 0x403c10 */
void ch_ReleaseMorph(CHARACTER *ch);                                                    /* 0x403d60 */
void ch_CheckAniMorph(CHARACTER *ch);                                                   /* 0x403dc0 */
void ch_PreloadAnims(struct CHARACTER *ch, int n, ...);                                        /* 0x404200 */
int ch_PlaceOnGround(CHARACTER *ch, int force);                                /* 0x404a80, puts the character on the
                                                                                  ground (force: also when the ground
                                                                                  type is 0 or 0xd); returns the ground
                                                                                  type (0: none) */
void ch_SaveGround(struct D3HMAP *hmap);                                                 /* 0x404e60 */


/* ---- 0x404360..0x405810 (worker ch2): action queue, camera presets, ground map ---- */
struct D3OBJECT;
struct SNDHANDLE;
#include "d3_anim.h"
#include "d3_sm.h"

typedef struct CHACTION CHACTION;
typedef int (*CHACTFUNC)(CHACTION *a);

/* CHACTION.u.walk.flags */
#define CH_WALK_TARGET   0x08   /* u.walk.target / u.walk.tname: follow that character */
#define CH_WALK_DEST     0x10   /* walk to u.walk.dest, then place the character there */
#define CH_WALK_RESTART  0x20   /* restart the walk animation on the next update */
#define CH_WALK_STOPANIM 0x40   /* release the walk animation before moving on */
#define CH_WALK_ARRIVED  0x80   /* the dest was reached: the action removes itself */

struct CHACTION {                   /* 0x194 bytes, 2000 of them in ch_Queue; func == 0: free */
    CHACTFUNC func;                 /* 0x00, called every update */
    CHACTFUNC func2;                /* 0x04, called when the animation reaches frame unk08 */
    unsigned char unk08;
    unsigned char type;             /* 0x09, index into ch_ActDefs; '-' (0x2d): walk */
    char pad0a[0x2];
    int count;                      /* 0x0c, updates so far */
    unsigned char unk10;            /* returned by the animation action while it runs */
    char pad11[0x3];
    CHARACTER *ch;                  /* 0x14 */
    D3VECTOR rot;                   /* 0x18, turn actions: final angles (worker ch4) */
    CHACTION *prev;                 /* 0x24, the character's queue */
    CHACTION *next;                 /* 0x28 */
    char pad2c[0x4];
    union {                         /* 0x30 */
        int args[0x30];             /* from ch_InsertAction's varargs */
        struct {
            char pad30[0x60];
            char scriptName[0x5f];  /* 0x90, the script an animation-script action runs */
        };
    };
    union {                         /* 0xf0 */
        char name[0x88];            /* animation name */
        struct {
            char started;           /* turn actions (worker ch4) */
        } turn;
        struct {
            int pathLen;            /* 0xf0, number of cells in path (2 bytes each) */
            void *path;             /* 0xf4, walk actions: allocated path */
            int cur;                /* 0xf8, next path cell (from here: worker ch4) */
            int unk0fc;
            unsigned char lastframe;    /* 0x100, walk animation frame of the last step sound (worker ch5) */
            char pad101[0x3];
            int started;            /* 0x104, walk animation started */
            float angle;            /* 0x108, turn still to do; < 0 left, > 0 right, 0 none */
            float dist;             /* 0x10c, distance to the next path cell when it was taken */
            struct CHARACTER *target;   /* 0x110, character to walk to */
            char tname[0x10];       /* 0x114, its name (CH_WALK_TARGET) */
            D3VECTOR end;           /* 0x124, world position of the last path cell */
            char pad130[0x4];
            D3VECTOR tpos;          /* 0x134, target position when the path was made */
            char pad140[0x4];
            D3VECTOR dest;          /* 0x144 */
            char pad150[0x4];
            int retryAt;            /* 0x154, ch_Time of the last failed path search + wait, -1: none */
            unsigned char retries;  /* 0x158 (worker ch5) */
            char pad159[0x3];
            void (*fail)(CHACTION *a);  /* 0x15c, called when no path is found */
            int flags;              /* 0x160, the CH_WALK_* flags below */
        } walk;
    } u;
    union {                             /* 0x178 */
        float speed;                    /* walk actions: the animation speed */
        struct {                        /* animation-script actions (chc.c) */
            int unk178;
            int group;                  /* 0x17c, EVTSCRIPT.group for the script it runs */
            int arg;                    /* 0x180, its int argument */
            char pad184[0x8];
            int flags;                    /* 0x18c, bit 0: set by chc_SitDownAtOnce */
        } scriptAction;
        struct {                        /* move actions: where to place the character */
            int unk178;
            char hasPos;                /* 0x17c, 1: pos holds the position */
            char pad17d[0x3];
            D3VECTOR pos;               /* 0x180 */
        } move;
    };
    char stop;                      /* 0x190 */
    char pad191[0x3];
};

typedef struct CHACTDEF {           /* 0x4c bytes, 64 of them in ch_ActDefs */
    CHACTFUNC func;
    char unk04;
    char name[0x43];                /* 0x05 */
    int nargs;                      /* 0x48 */
} CHACTDEF;

/* CHARACTER fields that the declaration above does not name yet */
#define CH_UNK080(ch)   (*(int *)((char *)(ch) + 0x80))
#define CH_FRAME(ch)    ((ch)->lastActionFrame)                 /* last frame func2 ran for */
#define CH_ACTIONS(ch)  ((ch)->actions)                             /* 0x128, queue head */
#define CH_MODEL(ch)    ((char *)(ch) + 0x130)                      /* model name */
#define CH_MODEL2(ch)   ((char *)(ch) + 0x230)                      /* model name of unk22c */

#define CH_QUEUE 2000

extern CHACTION *ch_Queue;                  /* 0x631050 */
#ifndef DATA_VIEWS_H
extern CHACTDEF ch_ActDefs[64];             /* 0x1469700 */
#endif
extern int ch_MotionTimeout;                /* 0x5f12a0 */
extern int ch_Ground0[];                    /* 0x146aa00, ground types of d3_Scenes[0] */
extern int ch_Ground1[];                    /* 0x14aaa20, ground types of the default scene */
extern int ch_DirtyX0;                      /* 0x5f12a8, changed ground rectangle */
extern int ch_DirtyZ0;                      /* 0x5f12ac */
extern int ch_DirtyX1;                      /* 0x63107c */
extern int ch_DirtyZ1;                      /* 0x631080 */
extern int d3t_CamMoving;                   /* 0x6310e4 */
extern struct D3ANIM *d3t_CamMove;       /* 0x6310e8, the running camera animation (d3_tools.c; ch.c / ecmd.c test it through d3t_CamMoveI) */

char d3sm_GetVertex(struct D3SM *h, int x, int z, struct D3VECTOR *out);                              /* 0x45f100 */  /* @original: the original callers read an int result (gv.h declares it int; ord.c's unk_Unk4f86a0 at 0x4f86a0 is byte-matched without the char widening) */
char map_Pick(struct D3FLMAP *map, struct D3VECTOR *pos, char lod, int *x, int *y, float *h);             /* 0x4463b0 */
void d3anim_ApplyRootMotion(D3OBJECT *obj, D3ANIMSLOT *s, int from, int to, int flags);       /* 0x4315c0 */
void d3anim_PauseAnim(D3ANIMCTL *ctl, char *name);                                          /* 0x435810 */
void d3anim_PlayAnim(D3ANIMCTL *ctl, char *name, char on);                                 /* 0x435870 */
int d3t_PlaceObjectV(struct D3OBJECT *camera, struct D3VECTOR *pos, struct D3VECTOR *dir, int unk, char flags); /* 0x411860 */
void vfs_ToSlashes(char *path);                                                         /* 0x42ed10 */
#define d3_LoadAnimFile ((int (*)(char *, char *, int))d3_LoadAnimStream)                    /* 0x44e4a0, returns int */
#define d3_AttachAnimF ((int (*)(void *, char *, D3ANIMFLAGS))d3anim_StartAnim)                   /* 0x435550 with the d3.h flags type */

void ch_LoadLowAnims(CHARACTER *ch, int n, ...);                                        /* 0x404360 */
int ch_AttachAni(CHARACTER *ch, char *name, unsigned char loop);                        /* 0x4044a0 */
int ch_ActAnim(CHACTION *a);                                                            /* 0x404640 */
CHACTION *ch_GetFreeQueueEntry(void);                                                   /* 0x404760 */
int ch_RemoveAction(CHACTION *a);                                                       /* 0x4047a0 */
int ch_ClearActions(CHARACTER *ch);                                                     /* 0x404810 */
void ch_CheckQueue(CHARACTER *ch, CHACTION *a);                                         /* 0x404860 */
CHACTION *ch_InsertAction(CHARACTER *ch, CHACTION *before, unsigned int type, ...);     /* 0x4048b0 */
int ch_UpdateAction(CHARACTER *ch);                                                     /* 0x404b90 */
void ch_PauseAnim(CHARACTER *ch, int pause);                                            /* 0x404c20 */
void ch_GetCamera(CHARACTER *ch, int view, D3VECTOR4 *pos, D3VECTOR *rot);              /* 0x404c80 */
void ch_SetCamera(CHARACTER *ch, int view);                                             /* 0x404d80 */
void ch_MoveCamera(CHARACTER *ch, int view, int time);                                  /* 0x404dd0 */
int ch_Random(unsigned short n);                                                        /* 0x404e40 */
void ch_RestoreGround(struct D3HMAP *hmap);                                                 /* 0x404ed0 */
int ch_SetGroundDiamond(int x, int z, unsigned char scene, int type, int r);                   /* 0x404fc0 */
int ch_RandomNear(D3VECTOR *pos, D3VECTOR *out);                                        /* 0x405160 */
int ch_FindNear(CHARACTER *ch, CHARACTER **out, float dist);                            /* 0x405260 */
int ch_Update(void);                                                                    /* 0x405310 */
int ch_StopActions(CHARACTER *ch);                                                      /* 0x405610 */
int ch_DeclareAction(int id, CHACTFUNC func, char unk, char *name, int nargs);          /* 0x405670 */
void ch_ActPlay(CHACTION *a);                                                           /* 0x4056e0 */
int ch_QueueAnim(CHARACTER *ch, char *name, int n);                                     /* 0x4057a0 */

#define D3MATERIAL_UNK96D(m) (*(unsigned char *)((char *)(m) + 0x96d))

extern int ch_Unk631060;                    /* 0x631060 */
extern int ch_Enabled;                      /* 0x5f12a4 */
extern D3SCENE *ch_Saved0;                  /* 0x631084, scene whose ground ch_Ground0 holds */
extern D3SCENE *ch_Saved1;                  /* 0x631088, the same for ch_Ground1 */
extern int ch_LastUpdate;                   /* 0x5f1224 */

int util_Random(unsigned short n);                                                      /* 0x42ec00, tr.c; the callers that read a 16-bit result (ch_RandomNear, anm_Update, ai_Unk4d88c0 ..) call util_RandomU, a cast in their head */
char ch_FindFreeCell(struct D3SM *h, struct D3VECTOR *from, struct D3VECTOR *to, int x, int z, int *px, int *pz, int max, int flags); /* 0x4072a0, see ch4 block */
CHACTION *ch_Unk40b270(CHARACTER *ch);                                                  /* 0x40b270 */
CHACTION *ch_Unk40b2f0(CHARACTER *ch, unsigned int type, ...);                          /* 0x40b2f0 */

/* ---- 0x405810..0x406e40 (worker ch3): sit / stand / gate / take / drop actions ---- */
/* CHACTION.u as used by the gate and object actions */
typedef struct CHACTOBJ {
    char name[0x40];                /* 0xf0, animation */
    char name2[0x40];               /* 0x130, object / gate name */
    int unk170;                     /* gate: room id; take: D3ANIMDATA of the animation */
    int unk174;
    int unk178;                     /* overlaps CHACTION.speed */
} CHACTOBJ;
#define CH_ACTOBJ(a)    ((CHACTOBJ *)&(a)->u)
#define CH_SLOT2(ch)    (*(D3ANIMSLOT **)((char *)(ch) + 0x74))    /* sitting animation */
#define CHACT_UNK18C(a) (*(unsigned char *)((char *)(a) + 0x18c))
#define D3MATERIAL_NAME104(m) (*(char **)((char *)(m) + 0x104))

void d3anim_SetFrame(D3OBJECT *obj, D3ANIMSLOT *s, int frame);                          /* 0x435950 */

CHACTION *ch_QueueAnimSpeed(CHARACTER *ch, char *name, int n, float speed);                  /* 0x405810 */
void ch_ActSitDown(CHACTION *a);                                                        /* 0x405870 */
CHACTION *ch_SitDown(CHARACTER *ch, char *name);                                             /* 0x405970 */
void ch_ReleaseSit(CHARACTER *ch);                                                      /* 0x4059c0 */
void ch_ActStandUp(CHACTION *a);                                                        /* 0x405a20 */
int ch_StandUp(CHARACTER *ch, char *name);                                              /* 0x405ad0 */
void ch_ActUsingGate(CHACTION *a);                                                      /* 0x405b30 */
CHACTION *ch_UseGate(CHARACTER *ch, char *name, char *gate, int room, int unk174, int unk178); /* 0x405cb0 */
void ch_ActTakingObject(CHACTION *a);                                                   /* 0x405d40 */
int ch_TakeObject(CHARACTER *ch, char *name, D3OBJECT *obj, int frame);                 /* 0x405e70 */
int ch_TakeObject1(CHARACTER *ch, char *name, D3OBJECT *obj, int frame);                /* 0x405f10 */
void ch_ActDroppingObject(CHACTION *a);                                                 /* 0x405fb0 */
int ch_DropObject(CHARACTER *ch, char *name, char *obj, int frame);                     /* 0x4060d0 */
int ch_DropObject1(CHARACTER *ch, char *name, char *obj, int frame);                    /* 0x406170 */
void ch_ActTurnTo(CHACTION *a);                                                         /* 0x406210 */
void ch_ActPlayOnce(CHACTION *a);                                                       /* 0x4062d0 */

typedef struct CHOBJLIST {          /* filled by ch_FindDummyCB */
    D3OBJECT *objs[32];
    int n;                          /* 0x80 */
} CHOBJLIST;
char ch_FindDummyCB(D3OBJECT *obj, CHOBJLIST *list);                                    /* 0x406340 */
D3OBJECT *ch_FindDummy(void);                                                           /* 0x406790 */
void ch_ActMove2Universe(CHACTION *a);                                                  /* 0x4067f0 */
int ch_FollowGround(CHACTION *a);                                                       /* 0x406a70 */

typedef struct CHPATHPT { unsigned char x, z; } CHPATHPT;
typedef struct CHPATH {             /* a path of map cells */
    int n;
    int unk04;
    CHPATHPT *pts;                  /* 0x08 */
} CHPATH;
typedef struct CHWALKDEF {
    char pad00[0x10];
    float speed;                    /* 0x10 */
} CHWALKDEF;
int ch_ThinPath(CHARACTER *ch, CHPATH *path, CHPATHPT *out, CHWALKDEF *w);              /* 0x406bc0 */
void ch_SetHandObject(CHARACTER *ch, int hand, char *name);                             /* 0x406c60, 1 left, 2 right, 3 head */

#define CH_UNK23A(ch)   (*(char *)((char *)(ch) + 0x23a))
#define CH_UNK038(ch)   ((int *)((char *)(ch) + 0x38))
#define CH_HAND(ch, off) ((D3OBJECT **)((char *)(ch) + (off)))                     /* 0x64 right, 0x68 left, 0x6c head */
#define CHACT_HASPOS(a) (*(char *)((char *)(a) + 0x17c))
#define CHACT_POS(a)    ((D3VECTOR *)((char *)(a) + 0x180))
char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload);                       /* 0x446f30 */
char d3_AttachToBone(D3OBJECT *obj, char *name);                                           /* 0x43d6c0 */


/* ---- 0x406e40..0x408690 (worker ch4): turning, path search helpers ---- */
typedef struct CHMAPNODE {          /* 0x14 bytes, "ch_map" */
    char pad00[0x10];
    int unk10;
} CHMAPNODE;

void ch_ActAnimOnce(CHACTION *a);                                                       /* 0x406e40 */
int ch_FindNear300(CHARACTER *ch, CHARACTER **out);                                     /* 0x407200 */
/* ch_FindFreeCell 0x4072a0 is declared above (ch2 block) */
void *ch_MapAlloc(int unk);                                                             /* 0x407690 */
void ch_MapFree(void *p);                                                               /* 0x4076b0 */
void ch_CopyVec(D3VECTOR *dst, D3VECTOR *src);                                          /* 0x4076c0 */
int ch_PosToCell(D3SM *h, D3VECTOR *pos, int *x, int *z);                               /* 0x4076e0 */
int ch_CellToCell(D3SM *h, int x, int z, D3SM *to, int *px, int *pz);                   /* 0x407740 */
typedef struct CHWAYPT {            /* 12 bytes; a list ends with flags == 0 */
    int x, z;                       /* cell in ch_PathMap */
    int flags;                      /* 1 character, 2 path cell, 8 transport, 4 inserted corner */
} CHWAYPT;
extern D3SM *ch_PathMap;            /* 0x63108c */
int ch_GetWaypoints(D3SM *h, CHARACTER *ch, CHWAYPT *out);                              /* 0x407790 */
extern PATHOUT ch_Path;              /* 0x14696e0 (overlaps ch_Motions as declared above) */
extern unsigned char ch_PathBuf[]; /* 0x14686e0 */
/* 0x406bc0 is ch3's ch_ThinPath; ch4 calls it with a PATHOUT / byte buffer / D3SM */
int ch_PathToTarget(CHACTION *a);                                                         /* 0x408490 */
typedef struct CHCONTACT CHCONTACT;
extern D3VECTOR ch_PathOffset;      /* 0x631068, added to the last path cell */
int ch_GetNewPath(CHARACTER *ch, int n, CHARACTER **others);                            /* 0x4079b0 */
typedef struct CHKEYFLAGS {         /* D3ANIMFLAGS as built for d3anim_CreatePath by ch_ActTurn */
    union {
        int all;
        struct { unsigned char b0, b1, b2, b3; } c;
        struct { unsigned char p0; unsigned char p8 : 2, f04 : 1, f08 : 1, f10 : 1, p13 : 3; } bits;
    } u;
    int mid;                        /* 0x04 */
    union {
        int all;
        struct { unsigned char a8 : 1, b8 : 1, r8 : 6; } c;
    } w;                            /* 0x08 */
} CHKEYFLAGS;
#define d3anim_CreatePathK ((D3ANIM *(*)(D3OBJECT *, CHKEYFLAGS, int, D3ANIMKEY *))d3anim_CreatePath)
#define CH_LASTFRAME(ch) (CH_SLOT(ch)->data->frames[CH_SLOT(ch)->data->nframes - 1])
void ch_ActTurn(CHACTION *a);                                                           /* 0x406e60 */

/* ---- 0x408690..0x40a900 (worker ch5): walking along a path ---- */
typedef struct CHSLOT {             /* D3ANIMSLOT as the walk code sees it */
    int frame;
    char pad04[0x48];
    D3VECTOR pos;                   /* 0x4c, where the animation moves the character to */
    int unk58;
    float speed;                    /* 0x5c, the animation speed set by ch_StartWalkAnim */
    float playSpeed;                /* 0x60, the playback speed the walk code applies from the ground
                                       type and CHARACTER.speedScale; 0.01 stops the animation, 1.0 is
                                       the plain speed */
    int unk64;
    D3ANIMDATA *data;               /* 0x68 */
    unsigned char b6c;
    unsigned char f6d_1 : 4, f6d_10 : 1, f6d_20 : 1, f6d_40 : 2;          /* 0x6d */
    unsigned char pad6e[2];
    unsigned int a70 : 1, b70 : 1, r70 : 30;                            /* 0x70 */
    unsigned char f74_1 : 1, play : 1, f74_4 : 1, f74_8 : 1, f74_10 : 4; /* 0x74 */
} CHSLOT;
#define CH_XSLOT(ch)    ((CHSLOT *)(ch)->slot)
#define CH_UNK110(ch)   (*(unsigned char *)((char *)(ch) + 0x110))
#define CH_POS038(ch)   ((D3VECTOR *)((char *)(ch) + 0x38))

extern char ch_Unk631058;           /* 0x631058 */
extern int ch_SndLib;               /* 0x63105c, sound library for step sounds */
struct SBSAMPLE * sb_GetSample(struct SBBANK *bank, char *name);                                               /* 0x426030 */
int d3snd_PlayAt(struct SNDHANDLE *snd, struct D3OBJECT *obj, int volume, float range);                    /* 0x417e30 */

int ch_InitWalk(CHACTION *a, D3SM *h);                                                  /* 0x408690 */
void ch_WalkSound(CHACTION *a, int ground);                                             /* 0x408ab0 */
int ch_StartWalkAnim(CHACTION *a, D3SM *h);                                             /* 0x408cb0 */
int ch_NextWaypoint(CHACTION *a, D3SM *h);                                              /* 0x408e60 */
void ch_WalkRotate(CHACTION *a);                                                        /* 0x4093d0 */
extern float ch_Unk5f1290;          /* 0x5f1290, 1.0 */
void ch_ActWalk(CHACTION *a);                                                           /* 0x4097d0, action 0x2d */
void ch_ActWalkCells(CHACTION *a);                                                      /* 0x409bd0, action 0x3a */

/* ---- 0x40a900..0x40b8a0 (worker ch6): walk-to helpers, fade/wait actions, open/close, action queue ---- */
/* the TU ends at 0x40b580 (script builtins follow, src/game/evtc.c) */
#define CHACT_NAME70(a) ((char *)(a) + 0x70)                         /* debug name of an action ("Walk2RndDummy") */
#define CHACT_FSTART(a) (*(float *)&(a)->args[8])                    /* 0x50, fade actions: start time */
#define CH_NOFADE       0x4000                                       /* CH_FLAGW: fade-in pending (met another character) */
extern char ch_EmptyName[];         /* 0x631090, "" (d3fl_EmptyName) */


#define ch_MapAllocF ((void *(*)(float))ch_MapAlloc)                /* 0x407690 takes a float */

CHACTION *ch_WalkToObject(CHARACTER *ch, D3OBJECT *obj);                                /* 0x40a900 */
void ch_ActWalk2RndDummy(CHACTION *a);                                                  /* 0x40a9c0 */
void ch_WalkToObjectTurn(CHARACTER *ch, D3OBJECT *obj);                                 /* 0x40aaf0 */
void ch_ActSetVisible(CHACTION *a);                                                     /* 0x40abd0 */
void ch_ActFade(CHACTION *a);                                                           /* 0x40ac00 */
void ch_ActWait(CHACTION *a);                                                           /* 0x40af50 */
void ch_Reset(void);                                                                    /* 0x40af90 */
int ch_Open(void);                                                                      /* 0x40afb0 */
void ch_Close(void);                                                                    /* 0x40b1a0 */
void ch_DeleteAll(void);                                                                /* 0x40b240 */
CHACTION *ch_NewAction(CHARACTER *ch);                                                  /* 0x40b270 */
CHACTION *ch_AddAction(CHARACTER *ch, unsigned int type, ...);                          /* 0x40b2f0 */
struct CHACTION * ch_AddActionCB(struct CHARACTER *ch, CHACTFUNC func2, unsigned char frame, unsigned int type, ...); /* 0x40b400 */
void ch_ClearSceneCallbacks(D3SCENE *scene);                                            /* 0x40b4d0 */
void ch_SetSitAnim(CHARACTER *ch, char *name);                                          /* 0x40b530 */
/* the ch2 block declares these two under their addresses */
#define ch_Unk40b270 ch_NewAction
#define ch_Unk40b2f0 ch_AddAction

#endif
