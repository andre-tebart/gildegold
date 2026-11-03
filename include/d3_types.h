/* d3 records other units need without the rest of d3.h: the scene record of the table d3_Scenes and
   the texture flags tex_Load takes by value (ts_texture.c's TSFLAGS), the sort entry of d3_engine's
   d3_SortBuf (d3_sm fills it), the particle colour D3COLOR (d3_tools' d3t_CreateSmoke / CreateEarth take it
   by value), the vectors D3VECTOR / D3VECTOR4. Types only; d3.h and d3_engine.h include it. */
#ifndef D3_TYPES_H
#define D3_TYPES_H

typedef struct D3VECTOR {
    float x, y, z;
} D3VECTOR;

typedef struct D3VECTOR4 {         /* the engine pads most vectors to 16 bytes */
    float x, y, z, w;
} D3VECTOR4;

typedef struct D3TEXFLAGS {         /* passed by value to tex_Load (TSFLAGS in ts_texture.h) */
    union {
        unsigned char b[4];
        unsigned int all;           /* the four flag bytes as one value (cleared at once) */
    };
} D3TEXFLAGS;

/* event tables ("d3e:"): an object's (D3OBJECT.events) and a scene's (D3SCENE.events) */
#define D3E_NEVENTS 7
typedef int (*D3EVHANDLER)(void *obj, int id, char *name, int arg);   /* obj: the object or scene, id: the event
                                       (sv fires 2 at an object's camera focus, 3 at its release, 6 at a scene),
                                       name: the handler's D3EVENT.name, arg: 0 at every known call */
typedef struct D3EVENT {            /* 0x84 bytes, D3E_NEVENTS of them in D3OBJX::unk1d4 / D3_SCENE3C8 */
    D3EVHANDLER fn;
    char name[0x80];                /* 0x04, name of the handler */
} D3EVENT;

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
            D3EVENT *events;            /* 0x3c8, the scene's D3EVENT table (D3E_NEVENTS entries,
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

typedef struct D3SORTENT {          /* 8 bytes, sorted by key with a radix sort (d3_RadixSort) */
    unsigned long key;
    void *item;                     /* a D3PTRI (d3_engine), a D3SMTRI (d3_sm) */
} D3SORTENT;

typedef struct D3COLOR {            /* a B, G, R, A colour (passed by value) */
    unsigned char b, g, r, a;
} D3COLOR;

#endif
