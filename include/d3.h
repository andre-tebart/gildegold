struct XOBJ;
/* d3 engine types and functions as far as reconstructed.
   Field names unkXXX are byte offsets whose meaning is not known yet; pads keep the offsets exact.
   Function names are ours unless noted; the address is the function's location in GildeGold.exe. */
#ifndef D3_H
#define D3_H

struct D3IOPATH;                   /* d3_io_t.h: the return of d3_LoadObjectAnim */
struct D3IOOBJ;

typedef struct D3VECTOR {
    float x, y, z;
} D3VECTOR;

typedef struct D3VECTOR4 {         /* the engine pads most vectors to 16 bytes */
    float x, y, z, w;
} D3VECTOR4;

typedef struct D3MATRIX {
    float _11, _12, _13, _14;
    float _21, _22, _23, _24;
    float _31, _32, _33, _34;
    float _41, _42, _43, _44;
} D3MATRIX;

typedef struct D3RECT {
    int left, top, right, bottom;
} D3RECT;

typedef struct D3TEXSLOT {          /* 0x7c bytes */
    char *name;
    char pad04[0x78];
} D3TEXSLOT;

struct D3OBJECT;
typedef struct D3MESHOPS {          /* function table of a mesh driver */
    char pad000[0x1f4];
    struct D3BOXVERT *(*GetBox)(struct D3OBJECT *obj, void *list, void *unk);   /* 0x1f4 */
} D3MESHOPS;

typedef struct D3MATERIAL {
    char pad000[0x8];
    char meshData[0xec];            /* 0x08..0xf4, the D3MESH block (d3_engine.h): worldMatrix 0x08 (a D3MATRIX,
                                       passed to d3_GetBoxVerts / GetBox), cameraMatrix 0x48, zMin/zMax, frameStamp */
    char animCtl[0x10];             /* 0xf4, the animation control (a D3ANIMCTL at 0xf4) passed to
                                       d3anim_StartAnim / d3_ReleaseTexture as their "list" */
    D3MESHOPS *ops;                 /* 0x104 */
    char pad108[0x70];
    D3TEXSLOT tex[3];               /* 0x178 */
} D3MATERIAL;

struct D3OBJECT;

typedef struct D3SCENE {            /* 0x3e0 bytes, 64 of them in d3_Scenes */
    char pad000[0x80];
    struct D3OBJECT *first;         /* 0x80, first top-level object (D3SCENELISTS.first, d3_engine.h) */
    char pad084[0x28];
    void *map;                      /* 0x0ac, the scene's terrain map (D3FLMAP, d3_Terrain; d3fl_ReloadTextures) */
    struct D3HMAP *hmap;            /* 0x0b0, terrain height map */
    union {                             /* the same bytes, read as the event table by d3_engine.c */
        char pad0b4[0x320];
        struct {
            char pad0b4a[0x314];
            void *events;               /* 0x3c8, the scene's D3EVENT table (D3E_NEVENTS entries,
                                           allocated by d3e_RegisterSceneEvent, freed in d3_Shutdown) */
            char pad3cc[0x8];
        };
    };
    char loaded;                    /* 0x3d4, set after a scene file was loaded into the slot; a scene slot
                                       with loaded or dirty set is not free (d3t_FindFreeScene) */
    char dirty;                     /* 0x3d5, a reserved scene whose file is not loaded yet: the next use
                                       reloads it and clears the flag (f3init.c "flagged dirty") */
    char pad3d6[0xa];
} D3SCENE;

typedef struct D3OBJECT D3OBJECT;
struct D3OBJECT {
    char name[0x4c];                /* compared with _strcmpi by name lookups; real length unknown */
    D3VECTOR pos;
    char pad058[0x4];
    D3VECTOR unk05c;
    char pad068[0x10];
    D3VECTOR posOffset;               /* 0x78 */
    D3VECTOR rotation;                /* 0x84, rotation angles in degrees; d3_SetObjectRot writes them */
    D3VECTOR unk090;
    char pad09c[0xf0];
    D3MATRIX matrix;                /* 0x18c */
    struct D3OBJINFO *info;         /* 0x1cc, the mesh data holder (D3OBJINFO / D3SMMESH / D3ANIMCTL views) */
    char pad1d0[0x4];
    float radius;                   /* 0x1d4, the mesh driver's radius (D3LOPS::radius, d3_light.h); read as obj->info->mesh->radius */
    char pad1d8[0x14];
    D3MATERIAL *material;           /* 0x1ec */
    D3OBJECT *next;                 /* 0x1f0, sibling list */
    D3OBJECT *prev;                 /* 0x1f4 */
    D3OBJECT *parent;               /* 0x1f8, set with d3_SetParent */
    D3OBJECT *child;                /* 0x1fc */
    char pad200[0x8];
    D3SCENE *scene;                 /* 0x208 */
    struct D3SOUND *sound;          /* 0x20c, set by d3snd_Attach */
    unsigned char state;            /* 0x210, object state bits: 1 = starts an object group, 2 = linked, 4 = dirty,
                                       8 = mirrored, 0x20 = vertex normals, 0x80 = no camera transform */
    unsigned char objectFlags;      /* 0x211, object flags, read and written bit by bit: 1 = has a saved position
                                       and angle (applied on load; d3t_RecordKeys keys it), 2 = saved with the
                                       scene, 4 = shadow caster (d3s_LightCB), 8 = simple shadow, 0x10 = light
                                       keeps its stored direction (type 7), 0x20 = shadow fake object (no box,
                                       tracks the z range), 0x40 = mirrored (d3_mir.c), 0x80 = alpha alias applied
                                       (d3_SetObjectAlpha) */
    unsigned char stateFlags;       /* 0x212, object flags, read and written bit by bit: 2 = vertex colors set
                                       (d3t_SetColor), 0xc = shadow mask tested against D3SM.mask, 0x10 = cast
                                       shadows despite the mask, 0x80 = world matrix current (d3_UpdateWorldMatrix) */
    unsigned char flags;            /* 0x213 */
    char pad214[0x4];
    int type;                       /* 0x218, 4 = mesh */
    int type0;                      /* 0x21c, the object's real type: d3if_Show saves it there while it hides
                                       the object as type 1, and the spawn sets it (D3OBJX.type0, d3_engine.h) */
    int kind;                       /* 0x220, object kind from the file: 1 = per-vertex lit (d3_LightApply), 2 = character, 5 = hand object / sun rays; d3_ReloadMeshes selects one kind */
    int mask;                       /* 0x224, the object's mask bits, tested against the pick mask
                                       (d3if_PickCB: p->mask && !(obj->mask & p->mask)); 1 for the sun rays,
                                       -1 / 0 from the loader */
};

typedef struct D3BOXVERT {         /* 0x50 bytes, 8 of them: the transformed bounding box */
    float x, y, z;
    char pad0c[0x44];
} D3BOXVERT;

typedef struct D3VERTEX {          /* 0x50 bytes, what D3TRI::v points to */
    float x, y, z;
    char pad0c[0x38];
    unsigned char b, g, r, a;       /* 0x44, diffuse color */
    char pad48[0x8];
} D3VERTEX;

void d3_UpdateWorldMatrix(D3OBJECT *obj, D3OBJECT *camera, char force);                         /* 0x439870, d3_engine.c */
D3BOXVERT *d3_GetBoxVerts(D3OBJECT *obj, struct D3OBJINFO *info, void *unk);           /* 0x459ab0 */

typedef struct D3MESHKEY {          /* 0x50 bytes */
    int unk00;
    float time;
    char pad08[0x40];
    float *pos;                     /* 0x48 */
    char pad4c[0x4];
} D3MESHKEY;

typedef struct D3TRI {              /* 0x28 bytes */
    D3VECTOR *v[3];
    char pad0c[0x1c];
} D3TRI;

typedef struct D3OBJINFO {
    D3MESHKEY *keys;                /* followed by the 8 bounding box corners */
    D3TRI *tris;                    /* 0x04 */
    int nkeys;                      /* 0x08 */
    int ntris;                      /* 0x0c */
    D3OBJECT *mesh;                 /* 0x10, the mesh driver (D3OPS, d3_engine.h; starts with the mesh name) */
} D3OBJINFO;

void d3_TransformSurf(D3OBJECT *obj);                                                       /* 0x43d7c0 */
char d3_RelightObject(struct D3OBJECT *obj);                                                       /* 0x453280 */
void d3_LocalToWorld(struct XOBJ *obj, float *in, float *out);                             /* 0x458c10 */
void d3_PointToWorld(struct D3OBJECT *obj_, struct D3VECTOR *in, struct D3VECTOR *out);                          /* 0x458a20, writes out and the angles after it */

#define D3_MAX_SCENES 64

typedef struct D3TEXTURE {          /* 0x80 bytes, d3_TexCount of them at d3_Textures */
    char name[0x40];
    int refs;                       /* 0x40, 0 = free slot (TSTEXTURE.refs, ts_texture.h) */
    char pad44[0x14];
    int unk58;
    char pad5c[0xc];
    unsigned char flags;            /* 0x68, bit 0 wrap, bit 3 compressed (size / 8); bit 4: no culling for
                                       the triangles using the texture (d3_SetVertexColors copies it) */
    char pad69[0x14];               /* 0x6c: the tex_Load flags (D3_TEXVIEW in d3_light.h) */
    unsigned char filebpp;          /* 0x7d, bit depth of the file (TSTEXTURE.filebpp) */
    char pad7e[0x2];
} D3TEXTURE;

typedef struct D3TEXHANDLE {        /* returned by tex_Load */
    char pad00[0x60];
    struct IDirectDrawSurface4 *surf;   /* 0x60 */
    union {                         /* 0x64, the original calls it surftext (D3DCHECK texts) */
        struct IDirect3DTexture2 *d3dtex;
        struct IDirect3DTexture2 *surftext;
    };
    char pad68[0x8];
    unsigned char frames;           /* 0x70, animation frames in the texture */
} D3TEXHANDLE;

typedef struct D3TEXFLAGS {         /* passed by value to tex_Load */
    union {
        unsigned char b[4];
        unsigned int all;           /* the four flag bytes as one value (cleared at once) */
    };
} D3TEXFLAGS;

/* Inline-asm clear used all over the engine (284 copies): byte stores up to dword alignment,
   then dwords, then the remaining bytes. */
#define D3_ZERO_CAT(a, b) a##b
#define D3_ZERO_LABEL(n) D3_ZERO_CAT(d3_zero_tail, n)   /* unique per line, so D3_ZERO can be used twice in one function */
#define D3_ZERO(dst, size) {                                        \
    int _zn = (size); void *_zp = (dst);                            \
    __asm mov edi, _zp                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                                        \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                            \
    __asm rep stosb                                                 \
}

/* Same with the destination named directly in the asm (a plain variable, e.g. a global): no pointer temp. */
#define D3_ZERO_AT(var, size) {                                     \
    int _zn = (size);                                               \
    __asm mov edi, var                                              \
    __asm mov ebx, _zn                                              \
    __asm xor eax, eax                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle D3_ZERO_LABEL(__LINE__)                                        \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm D3_ZERO_LABEL(__LINE__): add ecx, ebx                            \
    __asm rep stosb                                                 \
}

extern D3SCENE d3_Scenes[D3_MAX_SCENES];   /* 0xfd7ba0 */
extern D3SCENE *d3_DefaultScene;            /* 0x5fb5b0 */
extern int d3_CurrentScene;                 /* 0x633444 */
extern D3OBJECT *d3_Camera;                 /* 0xfc2dec */
extern D3RECT d3_Viewport;                  /* 0xfc2930 */
extern volatile unsigned long d3_Time;      /* 0x6333cc, milliseconds; re-read at every use */
extern struct IDirect3DDevice3 *d3d_Device; /* 0x633418 */
extern long d3d_hr;                         /* 0x14661a4, result of the last checked D3D call */

/* Checked D3D call; the original passes __FILE__, __LINE__ and the call text. Callers pin __LINE__
   with #line so the pushed line number matches the original. */
#define D3DCHECK(call) if ((d3d_hr = (call)) != 0) d3d_Error(__FILE__, __LINE__, #call)

typedef char (*D3OBJECTCB)(D3OBJECT *obj, void *ctx);

char d3_ForEachObject(struct D3SCENE *scene, struct D3OBJECT *obj, unsigned int mask, D3OBJECTCB cb, void *ctx); /* 0x438d40 */
void d3_ReleaseTexture(void *list, char *name);                                         /* 0x435740 */
void d3_UpdateObject(D3OBJECT *obj, unsigned char full);                                           /* 0x43b6b0 */
void d3_InflateObject(D3OBJECT *obj);                                                       /* 0x43dbc0 */
void d3_MatFromAngles(D3VECTOR4 *v, D3MATRIX *m);                                           /* 0x4545c0 */
char d3if_SelectScene(int index, char keep);                                                /* 0x4478e0 */
typedef struct D3ANIM {             /* returned by d3anim_CreatePath */
    char pad00[0x2d];
    unsigned char flags2d;          /* 0x20 = finished */
} D3ANIM;

typedef struct D3ANIMFLAGS {        /* passed by value to d3anim_CreatePath */
    unsigned char b[12];
} D3ANIMFLAGS;

typedef struct D3ANIMKEY {          /* 0x58 bytes */
    int time;
    D3VECTOR4 pos;                  /* 0x04, relative to the object's position */
    D3VECTOR4 rot;                  /* 0x14, relative to the object's angles */
    char pad24[0x34];
} D3ANIMKEY;

D3ANIM *d3anim_CreatePath(D3OBJECT *obj, D3ANIMFLAGS f, int nkeys, D3ANIMKEY *keys);          /* 0x433d70 */
void d3_UnwrapAngles(float *a, float *b, float *ra, float *rb);                            /* 0x453d60, angles mod 2pi */
int d3_FindAnim(char *name);                                                           /* 0x433f40, anim loaded? */
void d3_Unk44e4a0(char *file, char *name, int unk);                                     /* 0x44e4a0, @original: d3_io_t.c defines D3IOANIMDATA * (char *, char *, char); this spelling keeps the /O2 operand order of the byte-matched d3_LoadScene44be60 (d3_int_io.c, 0x27f) */
int d3anim_StartAnim(void *list, char *name, D3ANIMFLAGS f);                                /* 0x435550, @original: this typedef spelling (not struct D3ANIMFLAGS) keeps the /O2 operand order of the byte-matched d3_LoadScene44be60 (d3_int_io.c, 0x27f) */
struct D3IOPATH *d3_LoadObjectAnim(char *file, struct D3IOOBJ *obj, char reload);                                 /* 0x44e2e0 */
extern char *d3_AnimPath;                   /* 0xfba940 */
char *util_BaseName(char *path);            /* 0x401040, strips the extension in place, returns the file name */
extern D3VECTOR d3_Unk1466620;             /* 0x1466620, camera target position */
extern D3VECTOR d3_Unk1466630;              /* 0x1466630, camera target angles */
void d3anim_FreePath(D3OBJECT *obj);                                                       /* 0x433b10 */
char d3if_Delete(D3OBJECT *obj);                                                       /* 0x447190, d3_interface.h */
struct D3OBJECT * d3_FindObject(int a, int flags, int c, char *name);                     /* 0x449690 */
D3OBJECT *d3_CreateObject(int a, D3VECTOR *pos, D3VECTOR *rot, char *name);       /* 0x446e10 */
int d3_VecEqual(struct D3VECTOR *a, struct D3VECTOR *b, float eps);                                   /* 0x453e80 */
void d3_SetObjectPos(struct D3OBJECT *obj, struct D3VECTOR *pos);                                     /* 0x43b790 */
void d3_SetObjectRot(D3OBJECT *obj, D3VECTOR *rot);                                     /* 0x43b860 */
void d3_SetObjectPosXYZ(D3OBJECT *obj, float x, float y, float z);                      /* 0x43b7f0 */
void d3_SetObjectRotXYZ(D3OBJECT *obj, float x, float y, float z);                      /* 0x43b920 */
char d3_SetParent(struct D3OBJECT *obj, struct D3OBJECT *parent);                                     /* 0x449960 */

void d3d_Error(char *file, int line, char *call);                                      /* 0x418e90 */
void d3_BeginScene(unsigned char fog, DWORD fogcol, unsigned char zbuffer);                                                 /* 0x4385a0 */
void d3_SetBlendState(unsigned char balpha, unsigned char glow, unsigned char colorkey);   /* 0x4387b0, d3_d3d.c */
void d3_EndScene(void);                                                                /* 0x438780 */

extern D3TEXTURE *d3_Textures;              /* 0xfb9074 */
extern unsigned int d3_TexCount;            /* 0xfb9084 */
extern void *d3_Terrain;                  /* 0x633738 */

D3TEXHANDLE *tex_Load(char *name, D3TEXFLAGS flags, int a, int b);                          /* 0x4605c0, @original: ts_texture.c defines TSTEXTURE * (char *, TSFLAGS, unsigned char, unsigned char); the d3 callers were compiled against this view (D3TEXFLAGS is the same 4-byte union, TSTEXTURE / D3TEXHANDLE the same memory) */                          /* 0x4605c0 */
void d3_SetObjectAlpha(D3OBJECT *obj, struct D3OBJINFO *info, D3TEXFLAGS f);           /* 0x43d430, f.b[0] = alpha */
struct TSTEXTURE; void tex_Reload(struct TSTEXTURE *tex, char keep);                                               /* 0x460e50 */  /* @original: ts_texture.c defines (TSTEXTURE *, char); the d3 callers were compiled against this */
struct TSTEXTURE; void tex_Release(struct TSTEXTURE *tex);                                                          /* 0x45fbc0 */
struct TSTEXTURE; char tex_SetHiColor(struct TSTEXTURE *tex, unsigned char hicolor);                                            /* 0x4611b0 */
struct TSTEXTURE; char tex_Unload(struct TSTEXTURE *tex);                                                     /* 0x461140 */
void d3fl_Flush(void);                                                                /* 0x43f990 */
struct D3FLMAP;
void d3fl_ReloadTextures(struct D3FLMAP *map);                                          /* 0x442280 */

typedef struct D3COLOR {
    unsigned char b, g, r, a;
} D3COLOR;

/* Particle systems (d3_par, "d3_par:ParticleSystem"). The engine allocates the particles and calls
   the system's callback each frame; the callbacks in d3_tools.c animate them. */
typedef struct D3PARTICLE {         /* 0x54 bytes */
    D3VECTOR4 vel;                  /* meaning depends on the effect */
    D3VECTOR4 start;                /* start position of the effect; .x is the brightness in the emitter */
    char pad20[0x10];
    unsigned long time;             /* 0x30, d3_Time of the last update */
    int pad34;
    D3VECTOR4 pos;                  /* 0x38 */
    float size;                     /* 0x48 */
    union {
        unsigned long color;        /* 0x4c, initialized to 0xffffffff */
        struct { unsigned char b, g, r, a; };
    };
    unsigned char frame;            /* 0x50, texture animation frame (tex_GetFrame index) */
    unsigned char flags;            /* 0x51, bit 0 = alive */
    char pad52[0x2];
} D3PARTICLE;

typedef struct D3PARTSYS D3PARTSYS;
typedef char (*D3PARTCB)(D3PARTSYS *ps, unsigned long time);

struct D3PARTSYS {                  /* 0x31c bytes */
    D3VECTOR4 v00;                  /* 0x00..0x1c: effect parameters, meaning depends on the effect */
    D3VECTOR4 v10;
    unsigned int flags;             /* 0x20 */
    union {
        unsigned int spawnCount;    /* particles spawned per frame (d3t_CreateEmitter sets 2) */
        D3COLOR color24;            /* base color + random mask in a (Erdbrocken) */
    };
    D3PARTICLE *particles;          /* 0x28 */
    char pad2c[0x9c];
    unsigned long color;            /* 0xc8 */
    char padcc[0x4];
    int count;                      /* 0xd0 */
    D3TEXHANDLE *tex;               /* 0xd4 */
    int texindex;                   /* 0xd8, index into d3_Textures */
    void *points;                   /* 0xdc, 4 D3PVERT per particle ("d3_par:ParticlePoints") */
    void *polys;                    /* 0xe0, 2 D3PTRI per particle ("d3_par:ParticlePolys") */
    float size;                     /* 0xe4, initial particle size */
    char obj[0x1f8];                /* 0xe8, embedded object */
    void *parent;                   /* 0x2e0 */
    char pad2e4[0xc];
    D3SCENE *scene;                 /* 0x2f0 */
    char pad2f4[0x1c];
    D3PARTCB callback;              /* 0x310 */
    D3PARTSYS *next;                /* 0x314 */
    D3PARTSYS *prev;                /* 0x318 */
};

D3PARTSYS *par_Create(int count, char *tex, D3TEXFLAGS flags, float size, void *parent, D3PARTCB cb); /* 0x455830 */
void par_SetPos(D3PARTSYS *ps, D3VECTOR *pos);                                          /* 0x455a20 */
extern D3VECTOR d3_Origin;                 /* 0x5ebba4, (0, 0, 0) */
void d3_Normalize(D3VECTOR4 *v);                                                        /* 0x454560 */

void *mem_Alloc(int size, char *tag);                                                  /* 0x4223c0 */
void mem_Free(void *p);                                                                 /* 0x422620 */

/* Inline-asm copy, the counterpart of D3_ZERO (dd_vesa.c): size and destination go through temps (which
   VC6 puts into dead parameter slots), the source is named directly in the asm. */
#ifndef D3_COPY
#define D3_COPY_LABEL(n) D3_ZERO_CAT(d3_copy_tail, n)
#define D3_COPY(dst, src, size) {                                   \
    int _cn = (size); void *_cd = (dst);                            \
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
#endif

#endif
