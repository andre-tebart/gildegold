/* d3_par ("d3_par:" allocation tags, 0x455630..0x457c30): particle system list, drawing and emitters.
   Original file name unknown. Names are ours. */
#ifndef D3_PAR_H
#define D3_PAR_H
#include "d3.h"                       /* the D3 records and typedefs this header names */

/* The list of particle systems is bracketed by two fake nodes whose link fields are plain globals:
   the end marker d3_ParEnd (0xfb9280), whose prev field (+0x318) is the tail pointer at 0xfb9598, and the head
   node (0xfb95e0), whose next field (+0x314) is the head pointer at 0xfb98f4. */
#define D3PAR_HEADNODE ((D3PARTSYS *)d3if_ParList)
extern char d3if_ParList[];         /* 0xfb95e0, d3_interface.c */
#ifndef DATA_VIEWS_H
extern D3PARTSYS *d3_ParHead;       /* 0xfb98f4, head of the default scene's list */
#endif
#ifndef DATA_VIEWS_H
extern D3PARTSYS *d3_ParTail;       /* 0xfb9598, tail of the default scene's list */
#endif

/* the scene's own particle list head / tail (D3SCENE +0xa4 / +0xa8), saved while another scene is active */
typedef struct D3SCENEPAR {
    char pad000[0xa4];
    D3PARTSYS *head;                /* 0xa4 */
    D3PARTSYS *tail;                /* 0xa8 */
} D3SCENEPAR;
#define D3PAR_SCENE_PAR(s) ((D3SCENEPAR *)(s))

/* the system's callback time marker at 0x24 (a D3PARTSYS view; the union there holds effect parameters
   otherwise: a per-call spawn cap in d3_tools.c's d3t_EmitterCB, color24 for Erdbrocken (d3_mir.c)) */
typedef struct D3PARSTIME {
    char pad000[0x24];
    unsigned long lasttime;         /* 0x24, d3_Time of the last callback; a repeated time is skipped */
} D3PARSTIME;
#define D3PAR_TIME(ps) ((D3PARSTIME *)(ps))

void par_Init(void);                                    /* 0x455630 */
void par_DeleteAll(void);                               /* 0x4556b0 */
void par_Unlink(D3PARTSYS *ps);                         /* 0x4556e0 */
void par_Delete(D3PARTSYS *ps);                         /* 0x455760, also in d3_engine.h */

extern float d3_ParUV0[6];          /* 0xfb95a0, (0,0) (1,1) (0,1) */
extern float d3_ParUV1[6];          /* 0xfb95c0, (0,0) (1,0) (1,1) */

/* views of the D3PVERT.flags / D3PTRI.flags bytes: bitfields reached through a typed pointer */
typedef struct PARVFLAGS {
    unsigned char l : 1, r : 1, t : 1, b : 1, b4 : 1, b5 : 1, b6 : 1, vis : 1;
} PARVFLAGS;
#define PAR_VF(v) ((PARVFLAGS *)&(v)->flags)
typedef struct PARTFLAGS {
    unsigned char low : 7, vis : 1;
} PARTFLAGS;
#define PAR_TF(t) ((PARTFLAGS *)&(t)->flags)

/* the stamp of a texture slot (0x54, see D3FLTEX in d3_fl.h): the frame of its last use */
typedef struct D3PARSTAMP {
    char pad000[0x54];
    unsigned long stamp;
} D3PARSTAMP;
#define D3PAR_TEXSTAMP(t) (((D3PARSTAMP *)(t))->stamp)

/* a D3COLOR that can also be set as a whole; the original stores the four bytes as a long */
typedef union PARCOL {
    unsigned long color;
    D3COLOR c;
} PARCOL;

/* each particle is a quad of 4 projected vertices (ps->points) and 2 triangles (ps->polys) */
#define PAR_VERTS(ps) ((D3PVERT *)(ps)->points)
#define PAR_TRIS(ps) ((D3PTRI *)(ps)->polys)
#define PAR_OBJ(ps) ((D3OBJECT *)(ps)->obj)
#define PAR_EMITTER(ps) ((D3EMITTER *)(ps))
/* D3FLTEX.flags.b[2] (0x6e): bit 0 = alpha modulates the colour */
#define PAR_TEXBITS(t) (((unsigned char *)(t))[0x6e])

/* a particle as the emitter callbacks use it (D3PARTICLE, 0x54 bytes) */
typedef struct PARPART {
    D3VECTOR4 vel;                  /* w: phase of the alpha wave */
    float phase[4];                 /* 0x10, phases of the x/y/z/size waves */
    float base[3];                  /* 0x20, position without the waves */
    float pad2c;
    unsigned long time;             /* 0x30, last update */
    unsigned long birth;            /* 0x34 */
    D3VECTOR4 pos;                  /* 0x38, w: alpha factor */
    float size;                     /* 0x48 */
    unsigned char b, g, r, a;       /* 0x4c */
    unsigned char frame;            /* 0x50 */
    unsigned char flags;            /* 0x51, bit 0 = alive */
    char pad52[2];
} PARPART;

/* the emitter parameters copied from D3EMITTERCFG into the particle system at 0x2c (D3EMITTER view) */
#define PAR_E(ps) ((D3EMITTER *)(ps))

/* cached reciprocals of the emitter parameters, computed at the start of the emitter callbacks; they live in
   the copy of the emitter config (D3EMITTERCFG at ps + 0x2c), overwriting its pad04 tail, f34, f38 and f3c */
typedef struct D3PARINV {
    char pad000[0x30];
    float inv_alpha0;               /* 0x30, 1 / alpha[0] */
    float inv_alpha_gap;            /* 0x34, 1 / (alpha[2] - alpha[1]) */
    float inv_size;                 /* 0x38, 1 / |size[1] - size[0]| (type 0), 1 / size[2] (type 1) */
    float size_ratio;               /* 0x3c, size[1] / size[0] */
} D3PARINV;
#define D3PAR_INV(ps) ((D3PARINV *)(ps))

#define PAR_RND() ((float)((float)rand() / RAND_MAX))

struct TSTEXTURE * tex_GetFrame(int index, unsigned char frame);                                         /* 0x460b00 */
void par_SetDir(D3PARTSYS *ps, D3VECTOR *dir, float roll);                              /* 0x455a40 */
void par_Draw(D3PARTSYS *ps);                                                       /* 0x455a90, draw */
char par_EmitterCB0(D3PARTSYS *ps, unsigned long time);                                 /* 0x456350 */
char par_EmitterCB1(D3PARTSYS *ps, unsigned long time);                                 /* 0x456b60 */
char par_EmitterCB2(D3PARTSYS *ps, unsigned long time);                                 /* 0x457460 */

#endif
