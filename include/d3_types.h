/* The scene record of d3's table d3_Scenes, for the units that declare the table without the rest of d3.h:
   types only. d3.h includes it. */
#ifndef D3_TYPES_H
#define D3_TYPES_H

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

#endif
