struct D3ANIMFRAME;
struct D3VECTOR4;
struct D3SURF;
struct XOBJ;
/* d3_light.c -- dynamic lighting of mesh objects ("d3_light:cache", "d3_light:rlight_object"), original file
   name unknown: 0x451e80..0x4538a0. One view of the objects for the whole TU: D3OBJECT with D3_L() for the
   fields d3.h does not name (rounds 11 mesh / d3x had two, unified in round 12 by worker geo). Names are ours. */
#ifndef UNK_451E80_H
#define UNK_451E80_H

typedef struct D3LVERT {            /* 0x50 bytes, a mesh vertex (D3PVERT, D3OBJINFO::keys) as the light code sees it */
    float x, y, z;                  /* 0x00, camera space */
    char pad0c[0x14];
    float nx, ny, nz;               /* 0x20, normal */
    char pad2c[0x4];
    union {
        D3VECTOR light;             /* 0x30, sum of the lights that reach the vertex */
        struct {
            float r, g, b;
        };
    };
    char pad3c[0x4];
    union {
        unsigned long color;        /* 0x40 */
        D3COLOR lit;                /* current colour: base plus the dynamic lights */
    };
    union {
        unsigned long specular;     /* 0x44 */
        D3COLOR base;               /* colour from the static lights */
    };
    D3VECTOR *src;                  /* 0x48, source point */
    signed char flags;              /* 0x4c, bit 7: reached by the light being processed */
    char pad4d[0x3];
} D3LVERT;

typedef struct D3LTRI {             /* 0x28 bytes (D3TRI) */
    D3LVERT *v[3];
    char pad0c[0x1c];
} D3LTRI;

typedef struct D3LOPS {             /* the mesh driver (D3OBJINFO::unk10) as the light code uses it */
    char pad000[0x6c];
    float originY;                  /* 0x06c, y of the mesh origin in its vertex data (d3_LightApply subtracts it before the directional-light normals) */
    char pad070[0x164];
    float radius;                   /* 0x1d4 */
    char pad1d8[0x14];
    void (*fn1ec)(D3OBJECT *obj, D3OBJINFO *info);          /* 0x1ec, transforms the normals of a mesh without key frames */
    char pad1f0[0x8];
    float (*Radius)(D3OBJECT *obj, D3OBJINFO *info);        /* 0x1f8 */
} D3LOPS;

typedef struct D3LINFO {            /* D3OBJINFO */
    D3LVERT *keys;                  /* 0x00 */
    D3LTRI *tris;                   /* 0x04 */
    int nkeys;                      /* 0x08 */
    int ntris;                      /* 0x0c */
    D3LOPS *ops;                    /* 0x10 */
} D3LINFO;

typedef struct D3LHIT {             /* 8 bytes: a lit vertex and the light's contribution (only c.r in grey mode) */
    D3LVERT *v;
    D3COLOR c;
} D3LHIT;

typedef struct D3LREC D3LREC;
struct D3LREC {                     /* "d3_light:rlight_object": the vertices one light reaches in one object */
    int n;                          /* 0x00 */
    D3OBJECT *obj;                  /* 0x04, the lit object */
    D3LREC *next;                   /* 0x08 */
    D3LHIT v[1];                    /* 0x0c, n of them */
};

typedef struct D3LKEY {             /* 0x38 bytes */
    char pad00[0x24];
    float val;                      /* 0x24 */
    char pad28[0x10];
} D3LKEY;

typedef struct D3LDATA {            /* D3_L(o)->light, D3LIGHTINFO in d3_engine.h */
    D3LKEY keys[7];                 /* 0x000 */
    D3VECTOR dir;                   /* 0x188, direction in camera space (type 7) */
    unsigned long time;             /* 0x194, last flicker step (d3_Time) */
    D3LREC *list;                   /* 0x198 */
    float flicker;                  /* 0x19c */
    unsigned int interval;          /* 0x1a0 */
    int unk1a4;
    unsigned char on;               /* 0x1a8, the light reaches the object being lit */
    char pad1a9[0x3];
} D3LDATA;

typedef struct D3LOBJ {             /* D3OBJECT fields the light code uses that d3.h does not name */
    char pad000[0x44];
    unsigned long time;             /* 0x044, d3_Time of the last lighting */
    char pad048[0x10];
    float flickerEnd;               /* 0x058, flicker factor the current interval runs to */
    D3VECTOR color;                 /* 0x05c */
    float flickerStart;             /* 0x068, flicker factor the current interval started from */
    char pad06c[0x24];
    float range;                    /* 0x090 */
    float power;                    /* 0x094, 0: off */
    float atten;                    /* 0x098 */
    char pad09c[0x130];
    D3LINFO *info;                  /* 0x1cc */
    char pad1d0[0x8];
    D3VECTOR cam;                   /* 0x1d8, position in camera space */
    float radius2;                  /* 0x1e4, squared radius */
    D3LDATA *light;                 /* 0x1e8 */
    struct D3MESH *mesh;            /* 0x1ec, D3MESH (d3_engine.h) */
} D3LOBJ;
#define D3_L(o) ((D3LOBJ *)(o))

typedef struct D3LCTX {             /* the lights that reach an object (d3_LightCollectCB collects, d3_LightApply adds) */
    D3OBJECT *obj;                  /* 0x00, the lit object */
    unsigned int count;             /* 0x04 */
    D3OBJECT **list;                /* 0x08 */
    unsigned char normals;          /* 0x0c, use the vertex normals (D3OBJECT::state bit 5) */
} D3LCTX;

typedef struct D3LCTX2 {            /* context of d3_LightAddObject: the light and whether it needs the vertex normals */
    D3OBJECT *light;
    unsigned char normals;
} D3LCTX2;

/* Views of the bytes d3_SetVertexColors reads bit by bit. A union member in D3PTRI / D3TEXTURE (d3_engine.h,
   d3.h) would be the typed-field form, but adding one flips VC6's instruction order in d3_CloneObject
   (0x43d180) and d3_MirDrawObject (0x4550e0), which are byte; the casts are byte-identical to the original's
   bitfield accesses. Type debt (see the lead comment of src/d3_light.c). */
typedef struct D3BITS {             /* a byte as bit fields */
    unsigned char b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1;
} D3BITS;
typedef struct D3TEXVIEW {          /* D3TEXTURE as d3_SetVertexColors reads it */
    char pad00[0x6c];
    D3TEXFLAGS flags;               /* 0x6c, the flags tex_Load was called with (TSTEXTURE.flags, ts_texture.h) */
} D3TEXVIEW;
typedef union D3TEXPACK {           /* D3TEXFLAGS as the dword d3_RelightObject zeroes at once */
    int i;
    D3TEXFLAGS f;
} D3TEXPACK;
typedef struct D3LFRAME {           /* the animation frame d3_RelightObjectFrame is called with (D3ANIMFRAME) */
    char pad00[0x38];
    float radius;                   /* 0x38, the frame's mesh radius (D3ANIMFRAME.radius, d3_anim.h) */
} D3LFRAME;

extern volatile unsigned long d3_Time;      /* 0x6333cc */
extern D3OBJECT *d3_Camera;                 /* 0xfc2dec */
extern float d3_LightRadiusMargin;          /* 0x633b18, added to the mesh radius for the light reach:
                                               the matrix z offset (m->_43) or 0 */
extern D3VECTOR d3_AmbientColor;               /* 0x5fcc78, ambient light colour */
extern D3VECTOR d3_ZAxis;                   /* 0x5ebb74, (0, 0, 1) */

/* float to int with fistp (current rounding mode), as d3_FtoI in d3_engine.h but float first */
__inline void d3l_FtoI2(float f, int *dst)
{
    __asm fld f
    __asm mov edx, dst
    __asm fistp dword ptr [edx]
}

void d3_LightResetVertexColors(D3OBJECT *obj);           /* 0x451e80, callback */
void d3_LightFlicker(D3OBJECT *obj);           /* 0x451f10, callback: light flicker */
char d3_LightApply(D3OBJECT *obj, D3LCTX *set);  /* 0x4522b0, adds the lights in set to the vertex colours of obj */
char d3_LightAddObject(D3OBJECT *obj, D3LCTX2 *ctx); /* 0x452940 */
char d3_LightAttach(D3OBJECT *light, D3OBJECT *obj);  /* 0x452d90 */
void d3_LightUpdate(void);                      /* 0x452e10 */
char d3_LightRemoveRecords(D3OBJECT *obj, void *src); /* 0x452e40, drops the light records of obj (all, or src's) */
void d3_SetVertexColors(D3OBJECT *obj, struct D3SURF *s, char on, D3TEXFLAGS fl); /* 0x452fa0 */
char d3_RelightObject(D3OBJECT *obj);           /* 0x453280 */
void d3_RelightObjects(D3OBJECT *obj);          /* 0x453580, recomputes the vertex colours of the selected objects */
void d3_RelightObjectFrame(D3OBJECT *obj, struct D3ANIMFRAME *f); /* 0x4535b0 */
void d3_FrameNormals(D3OBJECT *obj, D3LINFO *info, struct D3ANIMFRAME *f);              /* 0x459980, f: key frame (d3_anim.h) */
char d3_ForEachObject2(D3SCENE *scene, D3OBJECT *obj, unsigned int mask, void (*cb)(D3OBJECT *obj)); /* 0x438e60 */
struct D3FLMAP;
void d3fl_MakeLight(struct D3FLMAP *map);   /* 0x441860 (d3_fl.c; d3_light.c passes d3_Terrain, a void *) */
void d3_LightInvalidate(unsigned char on);  /* 0x4538a0 (d3_engine.h: d3_LightInvalidate(int)) */

#endif
