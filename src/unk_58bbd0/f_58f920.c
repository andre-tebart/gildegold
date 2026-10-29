/* 0x58f920, the "vg_" predicate of ai_ShowVgObjects (86 B, a d3_ForEachObject callback): objects whose name contains
   "vg_" and whose parent is no per-vertex-lit object (parent == 0 or its kind != 1) are appended to the ctx list
   (count at +0x80); returns whether the list still has room (0x20 entries) */

#pragma pack(push, 1)
typedef struct AIOBJVIEW {         /* the object fields the vg_ / build-site callbacks read (d3.h D3OBJECT, same layout) */
    char pad0[0x1f0];
    struct AIOBJVIEW *next;        /* 0x1f0, the sibling list */
    struct AIOBJVIEW *prev;        /* 0x1f4 */
    struct AIOBJVIEW *parent;      /* 0x1f8, the parent object (the owner of a building site) */
    struct AIOBJVIEW *child;       /* 0x1fc, the first child (the "bk_" site markers live here) */
    char pad200[0x20];
    int kind;                      /* 0x220, 1 = per-vertex lit */
} AIOBJVIEW;
#pragma pack(pop)
typedef struct AIVGLIST {          /* the "vg_" object list of ai_ShowVgObjects */
    int objs[0x20];                /* 0x00 */
    int count;                     /* 0x80 */
} AIVGLIST;
typedef struct AIBUILDSITE {       /* the construction-site record lc_GetGebaeudeBauplatz fills (cm.c passes z + 0xc) */
    union {
        char name[0x40];           /* 0x00, the building name (ai_BuildingName) */
        int nameHead;              /* 0x00, cleared before the name is written */
    };
    float pos[4];                  /* 0x40, the owner's world position (d3_PointToWorld) */
    int object;                    /* 0x50, the "bk_" site object ai_FindBauplatz found */
    char siteName[0x34];           /* 0x54, the site object's name from the "bk_" prefix on */
} AIBUILDSITE;

char ai_VgFilter(int obj, AIVGLIST *ctx)
{
    AIOBJVIEW *o;

    o = (AIOBJVIEW *)obj;
    if (strstr((char *)obj, "vg_") != 0 && (o->parent == 0 || o->parent->kind != 1)) {
        ctx->objs[ctx->count] = obj;
        ctx->count = ctx->count + 1;
    }
    return ctx->count < 0x20;
}
