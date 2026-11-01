/* d3_tools -- the 3D engine's helper tools: camera / object placement and paths, rain, snow, particle emitters,
   sun rays (path leaked; start right after di_input.c, plausible).
   Open: ecmd.h still declares d3t_FindObject .. d3t_DeleteRain itself instead of including d3_tools.h.
   unit of rea 8cef7d5b (src/game/d3_tools.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\Units\\d3_tools\\d3_tools.c"
/* d3_tools.c -- original path D:\develop\Units\d3_tools\d3_tools.c
   Extent in GildeGold.exe (inferred from data/bss/call-graph, not proven): 0x411680..0x417650.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
struct D3VECTOR;
struct D3SM;
struct D3OBJECT;
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_d3d.h"
#include "d3_io_t.h"
#include "surf.h"
#include "d3_tools.h"

/* the pose a camera / zoom target stores in D3OBJECT 0x5c and 0x90 (serialized for object types 2/3;
   objectFlags bit 0 marks a key). For a light the same bytes are D3LOBJ.color and D3LOBJ.range/power/atten;
   a union view in D3OBJECT flips other units' code, so this is a local cast view (type debt). */
typedef struct D3OBJPOSE {
    char pad000[0x5c];
    D3VECTOR pos;                   /* 0x5c */
    char pad068[0x28];
    D3VECTOR rot;                   /* 0x90, the camera angles in degrees */
} D3OBJPOSE;
#define D3_POSE(o) ((D3OBJPOSE *)(o))


int d3t_CamMoving = 0;                   /* 0x6310e4 */
D3ANIM *d3t_CamMove = 0;                 /* 0x6310e8, running camera animation */
static D3OBJECT *d3t_FoundObject = 0;    /* 0x6310ec */
D3OBJECT *d3t_SunRays = 0;               /* 0x631120, reached from main.c by its address */
static int d3t_SunFade = 0;              /* 0x631124, 1 fading in, -1 fading out */
static int d3t_Unk631128 = 0;            /* 0x631128 */
int d3t_SunFadeStart = -1;  /* 0x5f345c, initialized to -1; TODO define here once .data is matched */

/* 0x411680, starts an animation that moves the object to pos and turns it to dir (angles). @deep B: read the whole 137-line asmdiff against 0x411680..0x411860: the same statements with another register allocation and scheduling - the three af flag byte updates (|= 0x14, &= ~8, |= 2) are interleaved differently, the d3_UnwrapAngles argument setup and the D3ANIMKEY copies use other registers/slots (key[0].time and the two vec copies swapped), the FPU projection/relative-vector sequence has another stack order; no different constant, callee, condition or field. Readability Q2 d3_Unk1466620/d3_Unk1466630 (globals) and d3anim_CreatePath (the animation creator) is deep work. Needs vargen/try.py */
int d3t_PlaceObject(D3OBJECT *camera, float px, float py, float pz,
                    float dx, float dy, float dz, int unk, char flags)
{
    D3ANIMKEY keys[2];
    D3VECTOR4 rel;
    D3VECTOR rot;
    D3ANIMFLAGS af;
    D3VECTOR dir;
    D3ANIM *anim;

    if (d3t_CamMoving && d3_Camera == camera)
        return 0;
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[1] &= ~8;
    af.b[8] |= 2;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    dir.y = dy;
    dir.z = dz;
    dir.x = dx;
    if (flags & 8) {
        d3_UnwrapAngles(&camera->rotation, &dir, &rot, &dir);
        d3_SetObjectRot(camera, &rot);
    }
    if (camera == d3_Camera) {
        d3_Unk1466630 = dir;
        d3_Unk1466620.x = px;
        d3_Unk1466620.y = py;
        d3_Unk1466620.z = pz;
    }
    rel.x = px - camera->pos.x;
    rel.y = py - camera->pos.y;
    rel.z = pz - camera->pos.z;
    keys[0].time = unk;
    keys[1].rot.x = (dir.x -= camera->rotation.x);
    keys[1].rot.y = (dir.y -= camera->rotation.y);
    dir.z -= camera->rotation.z;
    keys[1].pos.x = rel.x;
    keys[1].pos.y = rel.y;
    keys[1].pos.z = rel.z;
    keys[1].rot.z = dir.z;
    anim = d3anim_CreatePath(camera, af, 2, keys);
    if (camera == d3_Camera) {
        d3t_CamMoving = 1;
        d3t_CamMove = anim;
        anim->flags2d |= 8;
    }
    return 1;
}
/* 0x411860, d3t_PlaceObject with the target position and direction given as vectors */

int d3t_PlaceObjectV(D3OBJECT *camera, D3VECTOR *pos, D3VECTOR *dir, int unk, char flags)
{
    return d3t_PlaceObject(camera, pos->x, pos->y, pos->z, dir->x, dir->y, dir->z, unk, flags);
}
/* 0x4118a0, places the camera at the pose a zoom target object stores (its 0x5c position and 0x90 angles) */

int d3t_PlaceAtObject(D3OBJECT *obj, int unk)
{
    return d3t_PlaceObjectV(d3_Camera, &D3_POSE(obj)->pos, &D3_POSE(obj)->rot, unk, 8);
}
/* 0x4118d0, d3_FindObject callback: keeps the object whose name matches (case-insensitive) and stops the search */

static char d3t_FindObjectCB(D3OBJECT *obj, void *name)
{
    if (_strcmpi(obj->name, name) == 0) {
        d3t_FoundObject = obj;
        return 0;
    }
    return 1;
}
/* 0x411900, the first object of the default scene whose name matches, searched with mask 0x1e0 (flags bit 1) or 0x60 */

D3OBJECT *d3t_FindObject(char *name, char flags)
{
    d3t_FoundObject = 0;
    if (flags & 2)
        d3_ForEachObject(d3_DefaultScene, 0, 0x1e0, d3t_FindObjectCB, name);
    else
        d3_ForEachObject(d3_DefaultScene, 0, 0x60, d3t_FindObjectCB, name);
    return d3t_FoundObject;
}
/* 0x411960, starts the animation <name> on obj: an already loaded one by its base name, else the file
   <d3_AnimPath><name>.oam. */

int d3t_PlayAnim(D3OBJECT *obj, char *name, unsigned int flags)
{
    char msg[512];
    char buf[256];
    char *base;
    D3ANIMFLAGS af;
    D3ANIM *anim;

    if (!obj || !name)
        return 0;
    strcpy(buf, name);
    base = util_BaseName(buf);
    if (!d3_FindAnim(base))
        /* d3_LoadAnimStream's first parameter is the file name, the second the animation lookup name */
        d3_LoadAnimStream(name, base, 0);
    D3_ZERO(&af, sizeof(af));
    if (flags & 2)
        af.b[1] |= 0x10;
    if (flags & 1)
        af.b[1] &= ~0x10;
    if (flags & 4)
        af.b[1] |= 2;
    if (flags & 0x10)
        af.b[1] |= 8;
    if (flags & 0x20)
        af.b[1] |= 4;
    af.b[8] |= 2;
    if (obj->material && d3anim_StartAnim(obj->material->animCtl, base, af))
        return 1;
    sprintf(msg, "%s%s.oam", d3_AnimPath, name);
    anim = (D3ANIM *)d3_LoadObjectAnim(msg, obj, 1);
    if (!anim)
        return 0;
    if (flags & 2)
        anim->flags2d |= 0x10;
    if (flags & 1)
        anim->flags2d &= ~0x10;
    if (flags & 4)
        anim->flags2d |= 2;
    if (flags & 0x10)
        anim->flags2d |= 8;
    if (flags & 0x20)
        anim->flags2d |= 4;
    return 1;
}
/* 0x411b00, releases the named texture of the object's material (up to 3) and updates the object */

int d3t_ReleaseObjectTexture(D3OBJECT *obj, char *name)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (obj->material && obj->material->tex[i].name && _strcmpi(obj->material->tex[i].name, name) == 0) {
            d3_ReleaseTexture(obj->material->animCtl, name);
            d3_UpdateObject(obj, 0);
        }
    }
    return 1;
}
/* 0x411b70, releases every named texture of the object's material */

int d3t_ReleaseObjectTextures(D3OBJECT *obj)
{
    int i;
    for (i = 0; i < 3; i++) {
        if (obj->material && obj->material->tex[i].name)
            d3_ReleaseTexture(obj->material->animCtl, obj->material->tex[i].name);
    }
    return 1;
}
/* 0x411bb0, the index of a scene slot that is neither loaded nor dirty, or -1 */

int d3t_FindFreeScene(void)
{
    int i;
    for (i = 0; i < D3_MAX_SCENES; i++) {
        if (!d3_Scenes[i].loaded && !d3_Scenes[i].dirty)
            return i;
    }
    return -1;
}
/* 0x411be0, the index of scene in d3_Scenes, -1 when it is not in the array */

int d3t_SceneIndex(D3SCENE *scene)
{
    int i = ((char *)scene - (char *)d3_Scenes) / sizeof(D3SCENE);
    if (i < 0 || i > D3_MAX_SCENES - 1)
        i = -1;
    return i;
}
/* 0x411c10, per frame: ends a finished camera move and keeps the sun rays on the camera. */

void d3t_UpdateCamera(void)
{
    if (d3t_CamMove) {
        d3t_CamMoving = 1;
        if (d3t_CamMove->flags2d & 0x20) {
            d3anim_FreePath(d3_Camera);
            d3t_CamMoving = 0;
            d3t_CamMove = 0;
        }
    }
    if (d3t_SunRays && d3_DefaultScene == d3t_SunRays->scene)
        d3t_UpdateSunRays(d3t_SunRays);
}
/* 0x411c70, height range of a mesh's bounding box; returns 0 on success */

int d3t_GetHeightRange(D3OBJECT *obj, float *lo, float *hi)
{
    D3BOXVERT *v;
    int i;

    *hi = 0.0f;
    *lo = 0.0f;
    if (obj->type == 4) {
        d3_UpdateWorldMatrix(obj, d3_Camera, 0);
        v = d3_GetBoxVerts(obj, obj->info, obj->material->meshData);
        if (v) {
            *lo = v->y;
            *hi = v->y;
            for (i = 0; i < 7; i++) {
                *lo = min(*lo, v[i].y);
                *hi = max(*hi, v[i].y);
            }
            return 0;
        }
    }
    return 1;
}
/* 0x411d10, the four lower corners of the box around the mesh keys up to time t */

int d3t_GetFootprint(D3OBJECT *obj, float t, D3VECTOR *c0, D3VECTOR *c1, D3VECTOR *c2, D3VECTOR *c3)
{
    D3VECTOR4 v;
    float mn[4];
    float mx[4];
    D3MESHKEY *k;
    int i, j;

    if (obj->type != 4)
        return 1;
    d3_TransformSurf(obj);
    k = obj->info->keys;
    for (i = 0; i < obj->info->nkeys; k++, i++) {
        if (k->time <= t) {
            mn[0] = k->pos[0];
            mn[1] = k->pos[1];
            mn[2] = k->pos[2];
            mx[0] = k->pos[0];
            mx[1] = k->pos[1];
            mx[2] = k->pos[2];
            break;
        }
    }
    for (; i < obj->info->nkeys; i++, k++) {
        if (k->time <= t) {
            for (j = 0; j < 3; j++) {
                mn[j] = min(mn[j], k->pos[j]);
                mx[j] = max(mx[j], k->pos[j]);
            }
        }
    }
    v.x = mn[0];
    v.y = mn[1];
    v.z = mn[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c0);
    v.x = mx[0];
    v.y = mn[1];
    v.z = mn[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c1);
    v.x = mx[0];
    v.y = mn[1];
    v.z = mx[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c2);
    v.x = mn[0];
    v.y = mn[1];
    v.z = mx[2];
    d3_LocalToWorld(obj, (D3VECTOR *)&v, c3);
    return 0;
}

typedef struct D3MESHLIST { char pad[0x10]; D3MESHOPS *ops; } D3MESHLIST;

/* 0x411eb0, center and radius of a mesh's 8-corner box as seen from the camera; returns 1 when the
   box was read. The squared corner distance is stored through a volatile view and read back through
   a plain one: the original writes it to its slot (fstp) and compares it from memory (fcom [slot]),
   while a direct spelling keeps the sum on the FPU and compares r itself. Only the fsqrt position
   still differs (1 order hunk). */
char d3t_GetBoundingSphere(D3OBJECT *obj, D3OBJECT *camera, D3VECTOR *center, float *radius)
{
    char ok = 0;
    D3MESHLIST *list;
    D3BOXVERT *v;
    D3VECTOR4 c;
    float dx, dy, dz, r;
    union {
        volatile float sum;         /* squared distance, stored: the original keeps it in memory */
        float dist;                 /* the same slot, read by the compare (fcom [slot]) */
    } d;
    D3BOXVERT *w;
    int i;

    if (obj && (center || radius) && obj->material->ops) {
        list = (D3MESHLIST *)obj->material->animCtl;
        d3_UpdateWorldMatrix(obj, camera, 0);
        v = list->ops->GetBox(obj, list, obj->material->meshData);
        if (v) {
            c.x = v[0].x;
            c.y = v[0].y;
            c.z = v[0].z;
            for (i = 0, w = v + 1; i < 7; i++, w++) {
                c.x += w->x;
                c.y += w->y;
                c.z += w->z;
            }
            c.x *= 0.125f;
            c.y *= 0.125f;
            c.z *= 0.125f;
            r = 0.0f;
            for (i = 0; i < 8; i++) {
                dx = c.x - v[i].x;
                dy = c.y - v[i].y;
                dz = c.z - v[i].z;
                d.sum = dx * dx + dy * dy + dz * dz;
                r = max(r, d.dist);
            }
            ok = 1;
            if (radius)
                *radius = (float)sqrt(r);
            if (center) {
                center->x = c.x;
                center->y = c.y;
                center->z = c.z;
            }
        }
    }
    return ok;
}

typedef struct D3TMAP {             /* square 8-bit height map */
    int size;
    char pad04[0xc];
    unsigned char *data;            /* 0x10 */
    char pad14[0x7c];
    D3VECTOR org;                   /* 0x90, world position of cell 0 */
    char pad9c[0x4];
    float cellx;                    /* 0xa0 */
    char pada4[0x14];
    float cellz;                    /* 0xb8 */
    char padbc[0x8];
    float scale;                    /* 0xc4, height per unit */
} D3TMAP;

/* 0x412000, value range of row y between columns x0 and x1 */
int d3t_GetRowRange(D3TMAP *map, int *lo, int *hi, int x0, int x1, int y)
{
    unsigned char *p;
    int t;

    if (y >= 0 && y < map->size) {
        if (x1 < x0) {
            t = x0;
            x0 = x1;
            x1 = t;
        }
        if (x0 < map->size && x1 >= 0) {
            x0 = max(0, x0);
            x1 = min(x1, map->size - 1);
            p = map->data + map->size * y + x0;
            for (; x0 <= x1; x0++, p++) {
                *lo = min(*lo, *p);
                *hi = max(*hi, *p);
            }
            return 0;
        }
    }
    return 1;
}
/* 0x412090, height range of the map under the triangle a, b, c (x/z in world units) */

int d3t_GetTriRange(D3TMAP *map, float *lo, float *hi, D3VECTOR *a, D3VECTOR *b, D3VECTOR *c)
{
    float sx, sz;
    int x[3], z[3];
    int t, y;
    int mn, mx, ret;
    float xl, xr, dl, dr;

    sx = 1.0f / map->cellx;
    x[0] = (int)((a->x - map->org.x) * sx);
    sz = 1.0f / map->cellz;
    z[0] = (int)((a->z - map->org.z) * sz);
    x[1] = (int)((b->x - map->org.x) * sx);
    z[1] = (int)((b->z - map->org.z) * sz);
    x[2] = (int)((c->x - map->org.x) * sx);
    z[2] = (int)((c->z - map->org.z) * sz);
    if (z[0] > z[1]) {
        t = x[0]; x[0] = x[1]; x[1] = t;
        t = z[0]; z[0] = z[1]; z[1] = t;
    }
    if (z[0] > z[2]) {
        t = x[0]; x[0] = x[2]; x[2] = t;
        t = z[0]; z[0] = z[2]; z[2] = t;
    }
    if (z[1] > z[2]) {
        t = x[1]; x[1] = x[2]; x[2] = t;
        t = z[1]; z[1] = z[2]; z[2] = t;
    }
    if (z[0] < map->size && z[2] >= 0 && (x[0] >= 0 || x[1] >= 0 || x[2] >= 0)
        && (x[0] < map->size || x[1] < map->size || x[2] < map->size)) {
        mn = 255;
        mx = 0;
        if (z[0] == z[1]) {
            if (z[0] == z[2]) {
                xl = min(min(x[0], x[1]), x[2]);
                xr = max(max(x[0], x[1]), x[2]);
                ret = d3t_GetRowRange(map, &mn, &mx, xl, xr, z[0]);
            } else {
                xl = x[0];
                xr = x[1];
                ret = d3t_GetRowRange(map, &mn, &mx, xl, xr, z[0]);
                dl = ((x[2] - x[0]) * 1.0) / (z[2] - z[0]);
                dr = ((x[2] - x[1]) * 1.0) / (z[2] - z[1]);
                xl += dl;
                xr += dr;
                for (y = z[0] + 1; y <= z[2]; y++) {
                    ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                    xl += dl;
                    xr += dr;
                }
            }
        } else {
            xl = x[0];
            dl = ((x[2] - x[0]) * 1.0) / (z[2] - z[0]);
            xr = x[0];
            dr = ((x[1] - x[0]) * 1.0) / (z[1] - z[0]);
            ret = 1;
            for (y = z[0]; y <= z[1]; y++) {
                ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                xl += dl;
                xr += dr;
            }
            dr = ((x[2] - x[1]) * 1.0) / (z[2] - z[1]);
            xr = x[1] - dr;
            for (; y <= z[2]; y++) {
                ret &= d3t_GetRowRange(map, &mn, &mx, xl, xr, y);
                xl += dl;
                xr += dr;
            }
        }
        if (!ret) {
            *lo = mn * map->scale + map->org.y;
            *hi = mx * map->scale + map->org.y;
        }
        return ret;
    }
    return 1;
}
/* 0x412480, height range of the map under the quad a, b, c, d */

int d3t_GetQuadRange(D3TMAP *map, float *lo, float *hi, D3VECTOR *a, D3VECTOR *b, D3VECTOR *c, D3VECTOR *d)
{
    float lo1, hi1, lo2, hi2;
    int r1, r2;

    r1 = d3t_GetTriRange(map, &lo1, &hi1, a, b, c);
    r2 = d3t_GetTriRange(map, &lo2, &hi2, c, d, a);
    if (r1 && r2)
        return 1;
    if (!r1 && !r2) {
        *lo = min(lo1, lo2);
        *hi = max(hi1, hi2);
    } else if (!r1) {
        *lo = lo1;
        *hi = hi1;
    } else {
        *lo = lo2;
        *hi = hi2;
    }
    return 0;
}

typedef struct D3HMAP {             /* terrain height map */
    char pad00[0x4];
    float base;                     /* 0x04, height of value 0 */
    char pad08[0xc];
    float scale;                    /* 0x14, height per unit */
    char pad18[0x8];
    int size;                       /* 0x20 */
    char pad24[0x4];
    unsigned char *data;            /* 0x28 */
} D3HMAP;

char d3_GetHeightAt(struct D3SM *h, struct D3VECTOR *pos, int *px, int *pz, float *py);              /* 0x45f170 */

/* 0x412550, mean terrain height over the 8x8 cells around pos */
float d3t_GetMeanHeight(D3VECTOR *pos)
{
    D3HMAP *hm;
    int x, z, i, j;
    float h;
    float sum = 0.0f;

    if (d3_DefaultScene && (hm = d3_DefaultScene->hmap)) {
        if (d3_GetHeightAt(hm, pos, &x, &z, &h)) {
            for (i = -4; i < 4; i++) {
                for (j = -4; j < 4; j++) {
                    if (z + i < hm->size - 1 && x + j < hm->size - 1 && z + i > 0 && x + j > 0)
                        sum += hm->data[(z + i) * hm->size + x + j] * hm->scale + hm->base;
                    else
                        sum += h;
                }
            }
            sum /= 64;
        }
        return sum;
    }
    return 0.0f;
}
/* 0x412640, does the triangle's bounding box overlap the box a..b in x/z and reach below b.y? */

int d3t_InTriBox(D3VECTOR **tri, D3VECTOR *a, D3VECTOR *b)
{
    D3VECTOR4 mn, mx;

    mn.x = min(min(tri[0]->x, tri[1]->x), tri[2]->x);
    mn.y = min(min(tri[0]->y, tri[1]->y), tri[2]->y);
    mn.z = min(min(tri[0]->z, tri[1]->z), tri[2]->z);
    mx.x = max(max(tri[0]->x, tri[1]->x), tri[2]->x);
    mx.y = max(max(tri[0]->y, tri[1]->y), tri[2]->y);
    mx.z = max(max(tri[0]->z, tri[1]->z), tri[2]->z);
    if (mx.x < a->x || mn.x > b->x || mn.y > b->y || mx.z < a->z || mn.z > b->z)
        return 0;
    return 1;
}

typedef struct D3HQUERY {           /* height query over a box */
    D3OBJECT *obj;                  /* 0x00, the object being placed */
    float lo0, hi0;                 /* 0x04, its own height range */
    float t;                        /* 0x0c, footprint height */
    float lo;                       /* 0x10, lowest triangle height found */
    float hi;                       /* 0x14, highest */
    D3VECTOR4 c0, c1, c2, c3;       /* 0x18, footprint corners */
    D3VECTOR a;                     /* 0x58, box min */
    char pad64[0x4];
    D3VECTOR b;                     /* 0x68, box max */
    char pad74[0x4];
} D3HQUERY;

/* 0x412800, widens q->lo/hi by the triangles of obj and its children that lie in the box;
   returns 0 if any mesh was examined */
int d3t_QueryHeight(D3HQUERY *q, D3OBJECT *obj)
{
    D3VECTOR4 mx, mn;
    D3BOXVERT *v;
    D3TRI *tri;
    D3OBJECT *child;
    int i, r = 1;

    if (obj->type == 4) {
        if (!obj->info)
            return r;
        d3_TransformSurf(obj);
        d3_UpdateObject(obj, 1);
        v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
        mn.x = v->x;
        mn.y = v->y;
        mn.z = v->z;
        mx.x = v->x;
        mx.y = v->y;
        mx.z = v->z;
        for (i = 1, v++; i < 8; i++, v++) {
            mn.x = min(mn.x, v->x);
            mn.y = min(mn.y, v->y);
            mn.z = min(mn.z, v->z);
            mx.x = max(mx.x, v->x);
            mx.y = max(mx.y, v->y);
            mx.z = max(mx.z, v->z);
        }
        if (mn.x < q->b.x && q->a.x < mx.x && q->b.y > mn.y && q->b.z > mn.z && q->a.z < mx.z) {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                if (d3t_InTriBox(tri->v, &q->a, &q->b)) {
                    q->lo = min(q->lo, tri->v[0]->y);
                    q->lo = min(q->lo, tri->v[1]->y);
                    q->lo = min(q->lo, tri->v[2]->y);
                    q->hi = max(q->hi, tri->v[0]->y);
                    q->hi = max(q->hi, tri->v[1]->y);
                    q->hi = max(q->hi, tri->v[2]->y);
                }
            }
        }
        r = 0;
    }
    for (child = obj->child; child; child = child->next)
        r &= d3t_QueryHeight(q, child);
    return r;
}

/* inlined into d3t_DropObject; written out there, the walk keeps o in a different register */
static __inline D3OBJECT *d3t_FirstSibling(D3OBJECT *o)
{
    while (o->prev)
        o = o->prev;
    return o;
}

/* 0x412a70, sets obj on the ground (the terrain map and all other meshes under its footprint);
   top picks the highest instead of the lowest height found. Returns 0 if obj was moved. */
int d3t_DropObject(D3OBJECT *obj, int maponly, int top)
{
    D3HQUERY q;
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *parent;

    if (parent = obj->parent) {
        d3_SetParent(obj, 0);
        d3_SetObjectRotXYZ(obj, 0, obj->rotation.y, 0);
    } else
        parent = 0;
    if (!obj->info) {
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    q.obj = obj;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    if (d3t_GetHeightRange(obj, &q.lo0, &q.hi0)) {
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    q.t = (q.hi0 - q.lo0) * 0.1f + q.lo0;
    if (d3t_GetFootprint(obj, q.t, (D3VECTOR *)&q.c0, (D3VECTOR *)&q.c1, (D3VECTOR *)&q.c2, (D3VECTOR *)&q.c3)) {
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (parent)
            d3_SetParent(obj, parent);
        return 1;
    }
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    d3_SetObjectRot(d3_Camera, &rot);
    if (d3_Terrain) {
        if (d3t_GetQuadRange(d3_Terrain, &q.lo, &q.hi, (D3VECTOR *)&q.c0, (D3VECTOR *)&q.c1,
                             (D3VECTOR *)&q.c2, (D3VECTOR *)&q.c3)) {
            if (parent)
                d3_SetParent(obj, parent);
            return 1;
        }
        if (maponly) {
found:
            obj->pos.y += (top ? q.hi : q.lo) - q.lo0;
            obj->state |= 4;
            if (parent)
                d3_SetParent(obj, parent);
            return 0;
        }
    } else {
        if (maponly) {
            if (parent)
                d3_SetParent(obj, parent);
            return 1;
        }
        q.lo = 1e35f;
        q.hi = -1e35f;
    }
    q.a.x = min(min(min(q.c0.x, q.c1.x), q.c2.x), q.c3.x);
    q.a.y = q.lo0;
    q.a.z = min(min(min(q.c0.z, q.c1.z), q.c2.z), q.c3.z);
    q.b.x = max(max(max(q.c0.x, q.c1.x), q.c2.x), q.c3.x);
    q.b.y = q.hi0;
    q.b.z = max(max(max(q.c0.z, q.c1.z), q.c2.z), q.c3.z);
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    {
        D3OBJECT *o;
        int r = 1;
        o = d3t_FirstSibling(obj);
        do {
            if (o != obj)
                r &= d3t_QueryHeight(&q, o);
            o = o->next;
        } while (o);
        d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
        d3_SetObjectRot(d3_Camera, &rot);
        if (!r || d3_Terrain)
            goto found;
    }
    if (parent)
        d3_SetParent(obj, parent);
    return 1;
}
/* 0x413040, widens mn/mx by the bounding boxes of obj and its children */

void d3t_GetBounds(D3VECTOR *mn, D3VECTOR *mx, D3OBJECT *obj)
{
    D3BOXVERT *v;
    D3OBJECT *child;
    int i;

    if (!obj->info)
        return;
    d3_TransformSurf(obj);
    d3_UpdateObject(obj, 1);
    v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
    for (i = 0; i < 8; i++, v++) {
        mn->x = min(mn->x, v->x);
        mn->y = min(mn->y, v->y);
        mn->z = min(mn->z, v->z);
        mx->x = max(mx->x, v->x);
        mx->y = max(mx->y, v->y);
        mx->z = max(mx->z, v->z);
    }
    for (child = obj->child; child; child = child->next)
        d3t_GetBounds(mn, mx, child);
}
/* 0x413150, bounding box of obj and its children with the camera at the origin */

void d3t_GetObjectBounds(D3VECTOR *mn, D3VECTOR *mx, D3OBJECT *obj)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3BOXVERT *v;
    D3OBJECT *child;
    int i;

    if (!obj->info)
        return;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    d3_TransformSurf(obj);
    d3_UpdateObject(obj, 1);
    d3_UpdateWorldMatrix(obj, d3_Camera, 1);
    v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
    mn->x = v->x;
    mn->y = v->y;
    mn->z = v->z;
    mx->x = v->x;
    mx->y = v->y;
    mx->z = v->z;
    for (i = 1, v++; i < 8; i++, v++) {
        mn->x = min(mn->x, v->x);
        mn->y = min(mn->y, v->y);
        mn->z = min(mn->z, v->z);
        mx->x = max(mx->x, v->x);
        mx->y = max(mx->y, v->y);
        mx->z = max(mx->z, v->z);
    }
    for (child = obj->child; child; child = child->next)
        d3t_GetBounds(mn, mx, child);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    d3_SetObjectRot(d3_Camera, &rot);
}
/* 0x413310, bounding box of obj alone with the camera at the origin */

void d3t_GetMeshBounds(D3VECTOR *mn, D3VECTOR *mx, D3OBJECT *obj)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3BOXVERT *v;
    int i;

    if (!obj->info)
        return;
    pos.x = d3_Camera->pos.x;
    pos.y = d3_Camera->pos.y;
    pos.z = d3_Camera->pos.z;
    rot.x = d3_Camera->rotation.x;
    rot.y = d3_Camera->rotation.y;
    rot.z = d3_Camera->rotation.z;
    d3_SetObjectPosXYZ(d3_Camera, 0, 0, 0);
    d3_SetObjectRotXYZ(d3_Camera, 0, 0, 0);
    d3_TransformSurf(obj);
    d3_UpdateObject(obj, 1);
    v = (D3BOXVERT *)(obj->info->keys + obj->info->nkeys);
    mn->x = v->x;
    mn->y = v->y;
    mn->z = v->z;
    mx->x = v->x;
    mx->y = v->y;
    mx->z = v->z;
    for (i = 1, v++; i < 8; i++, v++) {
        mn->x = min(mn->x, v->x);
        mn->y = min(mn->y, v->y);
        mn->z = min(mn->z, v->z);
        mx->x = max(mx->x, v->x);
        mx->y = max(mx->y, v->y);
        mx->z = max(mx->z, v->z);
    }
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
    d3_SetObjectRot(d3_Camera, &rot);
}
/* 0x4134a0, on: sets every vertex color of obj to grey, off: lets 0x453280 restore them */

void d3t_SetGrey(D3OBJECT *obj, int on)
{
    D3TRI *tri;
    int i, j;

    if (obj && obj->info) {
        if (on) {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                for (j = 0; j < 3; j++) {
                    ((D3VERTEX *)tri->v[j])->r = 0x80;
                    ((D3VERTEX *)tri->v[j])->g = 0x80;
                    ((D3VERTEX *)tri->v[j])->b = 0x80;
                }
            }
            obj->stateFlags |= 2;
            obj->state |= 4;
        } else {
            obj->stateFlags &= ~2;
            d3_RelightObject(obj);
            obj->state |= 4;
        }
    }
}
/* 0x413550, sets every vertex color of obj to r/g/b; black lets 0x453280 restore them */

void d3t_SetColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b)
{
    D3TRI *tri;
    int i, j;

    if (obj && obj->info) {
        if (!r && !g && !b) {
            obj->stateFlags &= ~2;
            d3_RelightObject(obj);
            obj->state |= 4;
        } else {
            tri = obj->info->tris;
            for (i = 0; i < obj->info->ntris; tri++, i++) {
                for (j = 0; j < 3; j++) {
                    ((D3VERTEX *)tri->v[j])->r = r;
                    ((D3VERTEX *)tri->v[j])->g = g;
                    ((D3VERTEX *)tri->v[j])->b = b;
                }
            }
            obj->stateFlags |= 2;
            obj->state |= 4;
        }
    }
}
/* 0x413620, object walk callback: d3t_SetColor with an r/g/b triple */

static char d3t_SetColorCB(D3OBJECT *obj, void *rgb)
{
    d3t_SetColor(obj, ((unsigned char *)rgb)[0], ((unsigned char *)rgb)[1], ((unsigned char *)rgb)[2]);
    return 1;
}
/* 0x413640, d3t_SetColor on obj and its subtree; with bit 0 of state set the walk also goes on to obj->next */

void d3t_SetTreeColor(D3OBJECT *obj, unsigned char r, unsigned char g, unsigned char b)
{
    unsigned char rgb[3];
    D3OBJECT *next;

    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;
    if (obj) {
        if (!(obj->state & 1)) {
            next = obj->next;
            obj->next = 0;
            d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, d3t_SetColorCB, rgb);
            obj->next = next;
        } else
            d3_ForEachObject(d3_DefaultScene, obj, 0x1ff, d3t_SetColorCB, rgb);
    }
}
/* 0x4136d0, moves the camera along the n objects named in the varargs (an empty name is skipped)
   within time; each object marked by bit 0 of objectFlags contributes its position and angles as a key */

int d3t_MoveCameraPath(int time, int n, ...)
{
    int count = 0;
    D3OBJECT *objs[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};   /* 12 zeros: the implicit rest is stored separately */
    D3ANIMKEY keys[64];
    char name[96];
    D3ANIMFLAGS af;
    D3OBJECT *o;
    int i, k, step;
    char **args = (char **)&n;

    if (d3t_CamMoving)
        return 0;
    D3_ZERO(keys, sizeof(keys));
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[8] |= 2;
    af.b[1] |= 8;
    for (i = 0, k = 0; i < n; i++) {
        strcpy(name, args[++k]);
        if (name[0]) {
            if (!(objs[count] = d3_FindObject(0, 0x160, 0, name)))
                return 0;
            count++;
        }
    }
    step = time / count;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    keys[0].time = step;
    k = 1;
    for (i = 0; i < count; i++) {
        if (o = objs[i]) {
            if (o->objectFlags & 1) {
                keys[k].pos.x = D3_POSE(o)->pos.x;
                keys[k].pos.y = D3_POSE(o)->pos.y;
                keys[k].pos.z = D3_POSE(o)->pos.z;
                keys[k].rot.x = D3_POSE(o)->rot.x;
                keys[k].rot.y = D3_POSE(o)->rot.y;
                keys[k].rot.z = D3_POSE(o)->rot.z;
                keys[k].pos.x -= d3_Camera->pos.x;
                keys[k].pos.y -= d3_Camera->pos.y;
                keys[k].pos.z -= d3_Camera->pos.z;
                keys[k].rot.x -= d3_Camera->rotation.x;
                keys[k].rot.y -= d3_Camera->rotation.y;
                keys[k].rot.z -= d3_Camera->rotation.z;
            }
            keys[k - 1].time = step;
            k++;
        }
    }
    keys[k].time = 0;
    d3t_CamMove = d3anim_CreatePath(d3_Camera, af, count + 1, keys);
    d3t_CamMoving = 1;
    return 1;
}
/* 0x413960, moves obj along the n objects in path (null entries are skipped) within time @deep B: all 6 diffclass hunks read; the 79 code instructions are the same statements with i and k in swapped registers (ours i=ecx/k=edx, original k=ecx/i=edx; asmdiff --regs ecx=edx,edx=ecx leaves only the k store/reload placement and a 6-byte-longer tail) plus nop fill, no missing/wrong statement, constant or field */

D3ANIM *d3t_MoveObjectPath(D3OBJECT *obj, int time, int n, D3OBJECT **path)
{
    D3ANIMKEY keys[64];
    D3ANIMFLAGS af;
    D3OBJECT *o;
    int i, k, step;

    D3_ZERO(keys, sizeof(keys));
    D3_ZERO(&af, sizeof(af));
    af.b[1] |= 0x14;
    af.b[8] |= 2;
    af.b[1] |= 8;
    step = time / n;
    keys[0].rot.x = 0.0f;
    keys[0].rot.y = 0.0f;
    keys[0].rot.z = 0.0f;
    keys[0].pos.x = 0.0f;
    keys[0].pos.y = 0.0f;
    keys[0].pos.z = 0.0f;
    keys[0].time = step;
    k = 1;
    for (i = 0; i < n; i++) {
        if (o = path[i]) {
            d3_PointToWorld(o, &o->pos, (D3VECTOR *)&keys[k].pos);
            keys[k - 1].time = step;
            keys[k].rot.x -= obj->rotation.x;
            keys[k].rot.y -= obj->rotation.y;
            keys[k].rot.z -= obj->rotation.z;
            keys[k].pos.x -= obj->pos.x;
            keys[k].pos.y -= obj->pos.y;
            keys[k].pos.z -= obj->pos.z;
            k++;
        }
    }
    keys[k].time = 0;
    return d3anim_CreatePath(obj, af, n + 1, keys);
}

typedef struct D3TRAINDROP {         /* 0x28 bytes */
    D3VECTOR4 pos;                   /* w = tail length */
    float windScale;                 /* 0x10, weight of the wind velocity */
    float fallScale;                 /* 0x14, weight of the fall velocity */
    float sx1, sy1;                  /* 0x18, projected head */
    float sx2, sy2;                  /* 0x20, projected tail */
} D3TRAINDROP;

typedef struct D3TRAIN {             /* 0x60 bytes */
    int count;
    int capacity;                    /* allocated drops */
    int oldcount;                    /* count fade: oldcount -> newcount over time30..time34 */
    int newcount;
    D3TRAINDROP *drops;              /* 0x10 */
    float oldwindx, oldwindz;        /* wind fade: old -> new over time38..time3c */
    float windx, windz;              /* 0x1c */
    float newwindx, newwindz;
    int time2c;                      /* last update */
    int time30, time34, time38, time3c;
    D3VECTOR4 campos;                /* 0x40 */
    D3VECTOR4 cam04c;                /* 0x50 */
} D3TRAIN;

/* 0x413b00, inflates obj's geometry (d3_InflateObject) and then every object of its child chain
   (obj->child and the child's siblings), recursively; used after a model is loaded. */
void d3t_InflateObjectTree(D3OBJECT *obj)
{
    D3OBJECT *c;
    d3_InflateObject(obj);
    for (c = obj->child; c; c = c->next)
        d3t_InflateObjectTree(c);
}
/* 0x413b30, allocates and initialises the rain system for count drops: the D3TRAIN and its drop
   list, the counts, wind vectors and times from d3_Time, the saved camera pose, and per drop a random
   position in [-1,1], tail length 0.25..0.5 and wind / fall scales 0.25..0.3; 0 on allocation failure. */

D3TRAIN *d3t_CreateRain(int count)
{
    D3TRAIN *rain;
    D3TRAINDROP *drop;
    int i;

    rain = mem_Alloc(sizeof(D3TRAIN), "d3t:RainInfo");
    if (!rain)
        return 0;
    rain->count = count;
    rain->capacity = count;
    rain->oldcount = count;
    rain->newcount = count;
    rain->oldwindx = 1.0f;
    rain->oldwindz = 0.0f;
    rain->windx = 1.0f;
    rain->windz = 0.0f;
    rain->newwindx = 1.0f;
    rain->newwindz = 0.0f;
    rain->time2c = d3_Time;
    rain->time30 = d3_Time;
    rain->time34 = d3_Time;
    rain->time38 = d3_Time;
    rain->time3c = d3_Time;
    if (d3_Camera) {
        rain->campos.x = d3_Camera->rotation.x;
        rain->campos.y = d3_Camera->rotation.y;
        rain->campos.z = d3_Camera->rotation.z;
        rain->cam04c.x = d3_Camera->pos.x;
        rain->cam04c.y = d3_Camera->pos.y;
        rain->cam04c.z = d3_Camera->pos.z;
    }
    if (count < 1)
        count = 1;
    rain->drops = mem_Alloc(count * sizeof(D3TRAINDROP), "d3t:RainDropList");
    if (!rain->drops) {
        mem_Free(rain);
        return 0;
    }
    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        drop->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        drop->pos.w = (float)rand() / RAND_MAX * 0.25f + 0.25f;
        drop->windScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
        drop->fallScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
    }
    return rain;
}
/* 0x413d00, frees the rain system: its drop list (D3TRAIN.drops) and the D3TRAIN itself;
   null members are skipped. */

void d3t_DeleteRain(D3TRAIN *rain)
{
    if (rain) {
        if (rain->drops) {
            mem_Free(rain->drops);
            rain->drops = 0;
        }
        mem_Free(rain);
    }
}
/* 0x413d30: n commands follow: 0 count | 1 count duration | 2 windx windz duration */

void d3t_SetRain(D3TRAIN *rain, int n, ...)
{
    int *args = &n;
    int k = 0;
    int i, j, cmd, cap;
    D3TRAINDROP *drops, *drop;

    for (i = 0; i < n; i++) {
        cmd = args[++k];
        switch (cmd) {
        case 0:
        case 1:
            if (args[++k] >= rain->capacity) {
                cap = args[k] + 100;
                drops = mem_Alloc(cap * sizeof(D3TRAINDROP), "d3t:RainDropList");
                if (!drops)
                    return;
                memcpy(drops, rain->drops, rain->capacity * sizeof(D3TRAINDROP));
                mem_Free(rain->drops);
                rain->drops = drops;
                rain->capacity = cap;
                drop = &drops[rain->count];
                for (j = rain->count; j < cap; drop++, j++) {
                    drop->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    drop->pos.w = (float)rand() / RAND_MAX * 0.25f + 0.25f;
                    drop->windScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
                    drop->fallScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
                }
            }
            if (cmd == 0) {
                rain->count = args[k];
            } else {
                rain->oldcount = rain->count;
                rain->newcount = args[k];
                rain->time30 = rain->time2c;
                rain->time34 = rain->time2c + args[++k];
            }
            break;
        case 2:
            rain->oldwindx = rain->windx;
            rain->oldwindz = rain->windz;
            rain->newwindx = args[++k] * 0.001f;
            rain->newwindz = args[++k] * 0.001f;
            rain->time38 = rain->time2c;
            rain->time3c = rain->time2c + args[++k];
            break;
        }
    }
}
/* 0x413f30. The parentheses around drop->pos.w and the pos.y increment set VC6's operand ranks
   (load order and add association); the partial initializer of fall only restores the wind/fall
   slot order they disturb. See notes/vc6-matching.md. */

void d3t_UpdateRain(D3TRAIN *rain, float dt)
{
    D3VECTOR4 d, o, move, wind, fall = {0.0f, -0.75f}, vel;
    D3MATRIX m;
    float ex, ey, ez, wx, wz;
    float hw, hh, size, cx, cy, scale, f;
    D3TRAINDROP *drop;
    int i;

    d.x = rain->campos.x - d3_Camera->rotation.x;
    d.y = rain->campos.y - d3_Camera->rotation.y;
    d.z = rain->campos.z - d3_Camera->rotation.z;
    d3_MatFromAngles(&d, &m);
    rain->campos.x = d3_Camera->rotation.x;
    rain->campos.y = d3_Camera->rotation.y;
    rain->campos.z = d3_Camera->rotation.z;
    o.x = 0.0f * m._11 + 0.0f * m._21 + 2.5f * m._31;
    o.y = 0.0f * m._12 + 0.0f * m._22 + 2.5f * m._32;

    ex = rain->cam04c.x - d3_Camera->pos.x;
    ey = rain->cam04c.y - d3_Camera->pos.y;
    ez = rain->cam04c.z - d3_Camera->pos.z;
    move.x = ex * d3_Camera->matrix._11 + ey * d3_Camera->matrix._21 + ez * d3_Camera->matrix._31;
    move.y = ex * d3_Camera->matrix._12 + ey * d3_Camera->matrix._22 + ez * d3_Camera->matrix._32;
    move.z = ex * d3_Camera->matrix._13 + ey * d3_Camera->matrix._23 + ez * d3_Camera->matrix._33;
    move.x *= 0.0025f;
    move.y *= 0.0025f;
    move.z *= 0.0025f;
    rain->cam04c.x = d3_Camera->pos.x;
    rain->cam04c.y = d3_Camera->pos.y;
    rain->cam04c.z = d3_Camera->pos.z;

    wx = rain->windx;
    wz = rain->windz;
    wind.x = wx * d3_Camera->matrix._11 + 0.0f * d3_Camera->matrix._21 + wz * d3_Camera->matrix._31;
    wind.y = wx * d3_Camera->matrix._12 + 0.0f * d3_Camera->matrix._22 + wz * d3_Camera->matrix._32;
    wind.z = wx * d3_Camera->matrix._13 + 0.0f * d3_Camera->matrix._23 + wz * d3_Camera->matrix._33;
    fall.x = 0.0f * d3_Camera->matrix._11 + -0.75f * d3_Camera->matrix._21 + 0.0f * d3_Camera->matrix._31;
    fall.y = 0.0f * d3_Camera->matrix._12 + -0.75f * d3_Camera->matrix._22 + 0.0f * d3_Camera->matrix._32;
    fall.z = 0.0f * d3_Camera->matrix._13 + -0.75f * d3_Camera->matrix._23 + 0.0f * d3_Camera->matrix._33;

    hw = (d3_Viewport.right - d3_Viewport.left) >> 1;
    hh = (d3_Viewport.bottom - d3_Viewport.top) >> 1;
    size = hw > hh ? hw : hh;
    cx = d3_Viewport.left + hw;
    cy = d3_Viewport.top + hh;

    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        vel.x = wind.x * drop->windScale + fall.x * drop->fallScale;
        drop->pos.x = vel.x * dt + drop->pos.x + move.x + o.x;
        if (drop->pos.x < -1000.0 || drop->pos.x > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: X is %f", i, drop->pos.x);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.x = 0.0f;
        }
        while (drop->pos.x < -1.0)
            drop->pos.x += 2.0;
        while (drop->pos.x >= 1.0)
            drop->pos.x -= 2.0;

        vel.y = wind.y * drop->windScale + fall.y * drop->fallScale;
        drop->pos.y += (vel.y * dt + move.y + o.y);
        if (drop->pos.y < -1000.0 || drop->pos.y > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: Y is %f", i, drop->pos.y);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.y = 0.0f;
        }
        while (drop->pos.y < -1.0)
            drop->pos.y += 2.0;
        while (drop->pos.y >= 1.0)
            drop->pos.y -= 2.0;

        vel.z = wind.z * drop->windScale + fall.z * drop->fallScale;
        drop->pos.z += vel.z * dt + move.z * 2.0f;
        if (drop->pos.z < -1000.0 || drop->pos.z > 1000.0) {
            char buf[128];
            sprintf(buf, "Drop %li: Z is %f", i, drop->pos.z);
            sprintf(buf, "    Rain->WindX: %f    Rain->WindZ: %f", rain->windx, rain->windz);
            drop->pos.z = 0.0f;
        }
        while (drop->pos.z < -1.0)
            drop->pos.z += 2.0;
        while (drop->pos.z >= 1.0)
            drop->pos.z -= 2.0;

        scale = (rain->count / 2.0f + size / 3.0f) * 3.0f;
        f = scale / (drop->pos.z * 2.0f + 3.0f);
        drop->sx1 = drop->pos.x * f + cx;
        drop->sy1 = cy - drop->pos.y * f;
        vel.x *= (drop->pos.w);
        vel.y *= (drop->pos.w);
        f = scale / (((drop->pos.w) * vel.z + drop->pos.z) * 2.0f + 3.0f);
        drop->sx2 = (vel.x + drop->pos.x) * f + cx;
        drop->sy2 = cy - (vel.y + drop->pos.y) * f;
    }
}
/* 0x414590, per frame: eases count and wind toward their targets over time30..time34 / time38..time3c,
   advances the drops (d3t_UpdateRain) and draws every fully visible drop as a line from head to tail
   (D3DPT_LINELIST, at most 128 vertices per batch) tinted by a count-based fade; no-op without drops. */

void d3t_DrawRain(D3TRAIN *rain)
{
    D3DTLVERTEX VertBuff[128];
    int i, n, total, left, done;
    float dt, fade, depth;
    D3DCOLOR color;
    D3TRAINDROP *drop;

    if (!rain)
        return;
    if (rain->time30 != rain->time34) {
        if (d3_Time >= (unsigned long)rain->time34) {
            rain->count = rain->newcount;
        } else {
            total = rain->time34 - rain->time30;
            left = rain->time34 - d3_Time;
            rain->count = (total - left) * rain->newcount / total + rain->oldcount * left / total;
        }
    }
    if (rain->time38 != rain->time3c) {
        if (d3_Time >= (unsigned long)rain->time3c) {
            rain->windx = rain->newwindx;
            rain->windz = rain->newwindz;
        } else {
            total = rain->time3c - rain->time38;
            left = rain->time3c - d3_Time;
            done = total - left;
            rain->windx = done * rain->newwindx / total + left * rain->oldwindx / total;
            rain->windz = done * rain->newwindz / total + left * rain->oldwindz / total;
        }
    }
    dt = (d3_Time - rain->time2c) * 0.1f;
    rain->time2c = d3_Time;
    if (!rain->count)
        return;

    d3t_UpdateRain(rain, dt);
    d3_BeginScene(0, 0, 1);
    d3_SetBlendState(1, 0, 0);
    fade = 1.0f - rain->count * 0.0005f;
    color = RGBA_MAKE((int)(96.0f * fade), (int)(128.0f * fade), (int)(128.0f * fade), 0x80);

    n = 0;
    drop = rain->drops;
    for (i = 0; i < rain->count; drop++, i++) {
        if (drop->sx1 >= d3_Viewport.left && drop->sx1 < d3_Viewport.right &&
            drop->sx2 >= d3_Viewport.left && drop->sx2 < d3_Viewport.right &&
            drop->sy1 >= d3_Viewport.top && drop->sy1 < d3_Viewport.bottom &&
            drop->sy2 >= d3_Viewport.top && drop->sy2 < d3_Viewport.bottom) {
            depth = (1.0f - drop->pos.z) * 0.025f;
            VertBuff[n].sx = drop->sx1;
            VertBuff[n].sy = drop->sy1;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 0;
            VertBuff[n].color = color;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = drop->sx2;
            VertBuff[n].sy = drop->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 0;
            VertBuff[n].color = color;
            VertBuff[n].specular = 0;
            n++;
        }
        if (n >= 128) {
#line 2343
            D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_LINELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
            n = 0;
        }
    }
    if (n) {
#line 2348
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_LINELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
    }
    d3_EndScene();
}

typedef struct D3TSNOWFLAKE {        /* 0x28 bytes */
    D3VECTOR4 pos;                   /* w = size */
    float windScale;                 /* 0x10, weight of the wind velocity */
    float fallScale;                 /* 0x14, weight of the fall velocity */
    float sx1, sy1;                  /* 0x18, projected top left */
    float sx2, sy2;                  /* 0x20, projected bottom right */
} D3TSNOWFLAKE;

typedef struct D3TSNOW {             /* 0x94 bytes */
    int count;
    int capacity;
    int oldcount;
    int newcount;
    int unk10;
    int unk14[9];
    D3TSNOWFLAKE *flakes;            /* 0x38 */
    float oldwindx, oldwindz;
    float windx, windz;              /* 0x44 */
    float newwindx, newwindz;
    float unk54;
    int unk58;
    int time5c;                      /* last update */
    int time60, time64, time68, time6c;
    D3VECTOR4 campos;                /* 0x70 */
    D3VECTOR4 cam04c;                /* 0x80 */
    D3TEXHANDLE *Texture;            /* 0x90 */
} D3TSNOW;

/* 0x4148f0, allocates and initialises the snow system for count flakes: the D3TSNOW and its flake
   list, counts, wind vectors and times from d3_Time, the saved camera pose, per flake a random
   position in [-1,1], size 0.75..1.25 and wind / fall scales 0.025..0.035, and loads the
   "Schneeflocke" texture; 0 on allocation failure.
   @deep Q2: D3TSNOW.unk10, unk14[9], unk54 and unk58 are only ever written (CreateSnow, SetSnow command 3) and never read in src - D3TSNOW has no user outside d3_tools.c - so no role can be stated; the Q6 description is in place */
D3TSNOW *d3t_CreateSnow(int count)
{
    D3TSNOW *snow;
    D3TSNOWFLAKE *flake;
    D3TEXFLAGS flags;
    int i;

    snow = mem_Alloc(sizeof(D3TSNOW), "d3t:SnowInfo");
    if (!snow)
        return 0;
    snow->count = count;
    snow->capacity = count;
    snow->oldcount = count;
    snow->newcount = count;
    snow->unk10 = 5;
    snow->unk14[0] = 0;
    snow->unk14[1] = 0;
    snow->unk14[2] = 0;
    snow->unk14[3] = 0;
    snow->unk14[4] = 0;
    snow->unk14[5] = 0;
    snow->unk14[6] = 0;
    snow->unk14[7] = 0;
    snow->unk14[8] = 0;
    snow->unk54 = 0;
    snow->unk58 = -1;
    snow->oldwindx = 1.0f;
    snow->oldwindz = 0.0f;
    snow->windx = 1.0f;
    snow->windz = 0.0f;
    snow->newwindx = 1.0f;
    snow->newwindz = 0.0f;
    snow->time5c = d3_Time;
    snow->time60 = d3_Time;
    snow->time64 = d3_Time;
    snow->time68 = d3_Time;
    snow->time6c = d3_Time;
    if (d3_Camera) {
        snow->campos.x = d3_Camera->rotation.x;
        snow->campos.y = d3_Camera->rotation.y;
        snow->campos.z = d3_Camera->rotation.z;
        snow->cam04c.x = d3_Camera->pos.x;
        snow->cam04c.y = d3_Camera->pos.y;
        snow->cam04c.z = d3_Camera->pos.z;
    }
    if (count < 1)
        count = 1;
    snow->flakes = mem_Alloc(count * sizeof(D3TSNOWFLAKE), "d3t:SnowFlakeList");
    if (!snow->flakes) {
        mem_Free(snow);
        return 0;
    }
    flake = snow->flakes;
    for (i = 0; i < snow->count; flake++, i++) {
        flake->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
        flake->pos.w = (float)rand() / RAND_MAX * 0.5f + 0.75f;
        flake->windScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
        flake->fallScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
    }
    D3_ZERO(&flags, sizeof(flags));
    flags.b[2] |= 2;
    snow->Texture = (D3TEXHANDLE *)tex_Load("Schneeflocke", flags, 0, 0);
    if (snow->Texture)
        tex_Reload(snow->Texture, 0);
    return snow;
}
/* 0x414b50, frees the snow system: the flake list and texture, then resets the "DC_" textures'
   hicolor state (high colour when the format has no FOURCC and the file is above 8 bpp), flushes
   the texture cache and rebuilds the other scenes' tile maps; null snow is ignored.
   @deep Q2: D3TEXTURE.unk58 (0x58, TSTEXTURE.unk58) has no reader anywhere in src - every use is a zero store (the d3_fl.c 0x43faa0 @deep note records the same) - so no role can be stated; the other Q2 names are gone this session (refs/filebpp, D3SCENE.first, d3fl_ReloadTextures, d3d_UsedTexFormat.fourcc) */

void d3t_FreeSnow(D3TSNOW *snow)
{
    unsigned int i;
    int s;
    D3TEXTURE *tex;

    if (snow) {
        tex_Release(snow->Texture);
        if (snow->flakes) {
            mem_Free(snow->flakes);
            snow->flakes = 0;
        }
        tex = (D3TEXTURE *)d3_Textures;
        for (i = 0; i < d3_TexCount; tex++, i++) {
            if (tex->refs > 0 && _strnicmp("DC_", tex->name, 3) == 0 && !(tex->flags & 2)) {
                if (!tex_SetHiColor(tex, !d3d_UsedTexFormat.fourcc && tex->filebpp > 8 ? 1 : 0))
                    tex_Unload(tex);
                tex->unk58 = 0;
            }
        }
        d3fl_Flush();
        for (s = 0; s < D3_MAX_SCENES; s++) {
            if (s != d3_CurrentScene && d3_Scenes[s].first && d3_Scenes[s].map)
                d3fl_ReloadTextures(d3_Scenes[s].map);
        }
        if (d3_Terrain)
            d3fl_ReloadTextures(d3_Terrain);
        mem_Free(snow);
    }
}
/* 0x414c60: n commands follow: 0 count | 1 count duration | 2 windx windz duration | 3 unk @deep Q2: D3TSNOW.unk54 is only written (CreateSnow sets 0, SetSnow command 3 keeps max(unk54, arg*2500)) and never read in src - no role can be stated (the same gap the d3t_CreateSnow escalation records); Q6 description is already in the address comment */

void d3t_SetSnow(D3TSNOW *snow, int n, ...)
{
    int *args = &n;
    int k = 0;
    int i, j, cmd, cap;
    float f;
    D3TSNOWFLAKE *flakes, *flake;

    for (i = 0; i < n; i++) {
        cmd = args[++k];
        switch (cmd) {
        case 0:
        case 1:
            if (args[++k] >= snow->capacity) {
                cap = args[k] + 100;
                flakes = mem_Alloc(cap * sizeof(D3TSNOWFLAKE), "d3t:SnowFlakeList");
                if (!flakes)
                    return;
                memcpy(flakes, snow->flakes, snow->capacity * sizeof(D3TSNOWFLAKE));
                mem_Free(snow->flakes);
                snow->flakes = flakes;
                snow->capacity = cap;
                flake = &flakes[snow->count];
                for (j = snow->count; j < cap; flake++, j++) {
                    flake->pos.x = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.y = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.z = ((float)rand() / RAND_MAX - 0.5f) * 2.0f;
                    flake->pos.w = (float)rand() / RAND_MAX * 0.5f + 0.75f;
                    flake->windScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
                    flake->fallScale = (float)rand() / RAND_MAX * 0.01f + 0.025f;
                }
            }
            if (cmd == 0) {
                snow->count = args[k];
            } else {
                snow->oldcount = snow->count;
                snow->newcount = args[k];
                snow->time60 = snow->time5c;
                snow->time64 = snow->time5c + args[++k];
            }
            break;
        case 2:
            snow->oldwindx = snow->windx;
            snow->oldwindz = snow->windz;
            snow->newwindx = args[++k] * 0.001f;
            snow->newwindz = args[++k] * 0.001f;
            snow->time68 = snow->time5c;
            snow->time6c = snow->time5c + args[++k];
            break;
        case 3:
            j = args[++k];
            snow->unk54 = max(snow->unk54, j * 2500.0f);
            break;
        }
    }
}
/* 0x414ea0. NONMATCHING: 33 bytes, the integer loads of d3_Viewport.right/left for hw are scheduled
   two FPU instructions later than in the original; everything else is identical. The locals x and y
   keep the x update on the FPU stack and fix the sy2 operand order; (snow->windz) fixes where the
   move.z store is scheduled. Initializing fall at the top puts it below wind on the stack. */

void d3t_UpdateSnow(D3TSNOW *snow, float dt)
{
    D3VECTOR4 d, o, move, wind, fall;
    D3MATRIX m;
    float ex, ey, ez, wx, wz;
    float hw, hh, size, cx, cy, f, s, x, y;
    float gy;                        /* -1.0f: a literal -1.0f * x is folded, the original multiplies */
    D3TSNOWFLAKE *flake;
    int i;

    fall.x = 0.0f;
    fall.y = -1.0f;
    fall.z = 0.0f;
    d.x = snow->campos.x - d3_Camera->rotation.x;
    d.y = snow->campos.y - d3_Camera->rotation.y;
    d.z = snow->campos.z - d3_Camera->rotation.z;
    d3_MatFromAngles(&d, &m);
    snow->campos.x = d3_Camera->rotation.x;
    snow->campos.y = d3_Camera->rotation.y;
    snow->campos.z = d3_Camera->rotation.z;
    o.x = 0.0f * m._11 + 0.0f * m._21 + 1.9f * m._31;
    o.y = 0.0f * m._12 + 0.0f * m._22 + 1.9f * m._32;

    ex = snow->cam04c.x - d3_Camera->pos.x;
    ey = snow->cam04c.y - d3_Camera->pos.y;
    ez = snow->cam04c.z - d3_Camera->pos.z;
    move.x = ex * d3_Camera->matrix._11 + ey * d3_Camera->matrix._21 + ez * d3_Camera->matrix._31;
    move.y = ex * d3_Camera->matrix._12 + ey * d3_Camera->matrix._22 + ez * d3_Camera->matrix._32;
    move.z = ex * d3_Camera->matrix._13 + ey * d3_Camera->matrix._23 + ez * d3_Camera->matrix._33;
    move.x *= 0.0025f;
    move.y *= 0.0025f;
    move.z *= 0.0025f;
    snow->cam04c.x = d3_Camera->pos.x;
    snow->cam04c.y = d3_Camera->pos.y;
    snow->cam04c.z = d3_Camera->pos.z;

    wx = snow->windx;
    wz = (snow->windz);
    wind.x = wx * d3_Camera->matrix._11 + 0.0f * d3_Camera->matrix._21 + wz * d3_Camera->matrix._31;
    wind.y = wx * d3_Camera->matrix._12 + 0.0f * d3_Camera->matrix._22 + wz * d3_Camera->matrix._32;
    wind.z = wx * d3_Camera->matrix._13 + 0.0f * d3_Camera->matrix._23 + wz * d3_Camera->matrix._33;
    gy = -1.0f;
    fall.x = 0.0f * d3_Camera->matrix._11 + gy * d3_Camera->matrix._21 + 0.0f * d3_Camera->matrix._31;
    fall.y = 0.0f * d3_Camera->matrix._12 + gy * d3_Camera->matrix._22 + 0.0f * d3_Camera->matrix._32;
    fall.z = 0.0f * d3_Camera->matrix._13 + gy * d3_Camera->matrix._23 + 0.0f * d3_Camera->matrix._33;

    hw = (d3_Viewport.right - d3_Viewport.left) >> 1;
    hh = (d3_Viewport.bottom - d3_Viewport.top) >> 1;
    size = hw > hh ? hw : hh;
    cx = d3_Viewport.left + hw;
    cy = d3_Viewport.top + hh;

    flake = snow->flakes;
    for (i = 0; i < snow->count; flake++, i++) {
        x = flake->pos.x = (wind.x * flake->windScale + fall.x * flake->fallScale) * dt + flake->pos.x + move.x + o.x;
        while (x < -1.0)
            flake->pos.x = x += 2.0;
        while (flake->pos.x >= 1.0)
            flake->pos.x -= 2.0;

        flake->pos.y = (wind.y * flake->windScale + fall.y * flake->fallScale) * dt + flake->pos.y + move.y + o.y;
        while (flake->pos.y < -1.0)
            flake->pos.y += 2.0;
        while (flake->pos.y >= 1.0)
            flake->pos.y -= 2.0;

        flake->pos.z += (wind.z * flake->windScale + fall.z * flake->fallScale) * dt + move.z * 2.0f;
        while (flake->pos.z < -1.0f)
            flake->pos.z += 2.0f;
        while (flake->pos.z >= 1.0f)
            flake->pos.z -= 2.0f;

        f = size * 3.0f / (flake->pos.z * 2.0f + 3.0f);
        flake->sx1 = flake->pos.x * f + cx;
        flake->sy1 = y = cy - flake->pos.y * f;
        s = ((1.0f - flake->pos.z) * 13.5f + 1.0f) * flake->pos.w;
        flake->sx2 = flake->sx1 + s;
        flake->sy2 = y + s;
    }
}
/* 0x415380, per frame: eases count and wind toward their targets over time60..time64 / time68..time6c,
   advances the flakes (d3t_UpdateSnow) and draws every visible flake as a two-triangle textured quad
   (D3DPT_TRIANGLELIST, at most 192 vertices per batch) tinted 0x50646464; no-op without a snow system. */

void d3t_DrawSnow(D3TSNOW *Snow)
{
    D3DTLVERTEX VertBuff[192];
    int i, n, total, left, done;
    float dt, depth;
    D3TSNOWFLAKE *flake;

    if (!Snow)
        return;
    if (Snow->time60 != Snow->time64) {
        if (d3_Time >= (unsigned long)Snow->time64) {
            Snow->count = Snow->newcount;
        } else {
            total = Snow->time64 - Snow->time60;
            left = Snow->time64 - d3_Time;
            Snow->count = (total - left) * Snow->newcount / total + Snow->oldcount * left / total;
        }
    }
    if (Snow->time68 != Snow->time6c) {
        if (d3_Time >= (unsigned long)Snow->time6c) {
            Snow->windx = Snow->newwindx;
            Snow->windz = Snow->newwindz;
        } else {
            total = Snow->time6c - Snow->time68;
            left = Snow->time6c - d3_Time;
            done = total - left;
            Snow->windx = done * Snow->newwindx / total + left * Snow->oldwindx / total;
            Snow->windz = done * Snow->newwindz / total + left * Snow->oldwindz / total;
        }
    }
    dt = (d3_Time - Snow->time5c) * 0.1f;
    Snow->time5c = d3_Time;

    d3t_UpdateSnow(Snow, dt);
    d3_BeginScene(0, 0, 1);
    d3_SetBlendState(1, 1, 0);
#line 2942
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, Snow->Texture->surftext));

    n = 0;
    flake = Snow->flakes;
    for (i = 0; i < Snow->count; flake++, i++) {
        if (flake->sx1 >= d3_Viewport.left && flake->sx2 < d3_Viewport.right &&
            flake->sy1 >= d3_Viewport.top && flake->sy2 < d3_Viewport.bottom) {
            depth = (1.0f - flake->pos.z) * 0.025f;
            VertBuff[n].sx = (flake->sx2 + flake->sx1) * 0.5f;
            VertBuff[n].sy = flake->sy1;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0.5f;
            VertBuff[n].tv = 0;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = flake->sx2;
            VertBuff[n].sy = flake->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 1.0f;
            VertBuff[n].tv = 1.0f;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
            VertBuff[n].sx = flake->sx1;
            VertBuff[n].sy = flake->sy2;
            VertBuff[n].sz = depth;
            VertBuff[n].rhw = 1.0f;
            VertBuff[n].tu = 0;
            VertBuff[n].tv = 1.0f;
            VertBuff[n].color = 0x50646464;
            VertBuff[n].specular = 0;
            n++;
        }
        if (n >= 192) {
#line 2984
            D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
            n = 0;
        }
    }
    if (n) {
#line 2989
        D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLELIST, D3DFVF_TLVERTEX, VertBuff, n, D3DDP_DONOTLIGHT|D3DDP_DONOTCLIP|D3DDP_DONOTUPDATEEXTENTS));
    }
    d3_EndScene();
}

/* Random value in [-r, r]. The inner cast keeps the 1/RAND_MAX multiply ahead of (r + r). */
#define D3T_RAND(r) ((float)((float)rand() / RAND_MAX) * ((r) + (r)) - (r))

/* 0x4156a0, callback of d3t_CreateEmitter. The speed sum is cast to double (squares come out as
   x, z, y), and goes through a second local (root moved to st(3), dead values popped, then stored).
   z is volatile because the original keeps it in memory (fld z; fmul z, and fst z; fstp vel.z)
   where x and y follow from the parenthesized assignments. */
static char d3t_EmitterCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    D3VECTOR4 n;
    float k, dt, speed, x, y, a, d, s;
    volatile float z;
    unsigned int spawned, alive;
    int i;

    spawned = 0;
    alive = 0;
    k = ps->v10.x / (ps->v00.w * 2.0f + ps->v00.y);
    ps->flags &= ~2;
    p = ps->particles;
    for (i = 0; i < ps->count; i++, p++) {
        if ((ps->flags & 1) || (!(p->flags & 1) && spawned < ps->spawnCount)) {
            p->pos.x = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.y = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.z = (float)((float)rand() / RAND_MAX * 10.0) - 5.0f;
            p->pos.x += ps->v00.x;
            p->pos.y = (ps->v00.y) + p->pos.y;
            p->pos.z += ps->v00.z;
            p->vel.y = -(ps->v00.y - ps->v00.w);
            p->vel.x = D3T_RAND(ps->v00.w * 2.0f);
            p->vel.z = D3T_RAND(ps->v00.w * 2.0f);
            p->vel.x = k * p->vel.x;
            p->vel.y = k * p->vel.y;
            p->vel.z = k * p->vel.z;
            p->color = ps->color;
            p->start.x = 255.0f;
            p->size = ps->size;
            p->time = time;
            p->flags |= 1;
            spawned++;
        } else if (p->flags & 1) {
            dt = (time - p->time) / 3.0f;
            p->time = time;
            p->pos.x = (x = p->pos.x + dt * p->vel.x);
            p->pos.y = (y = p->pos.y + dt * p->vel.y);
            p->pos.z = (z = p->pos.z + dt * p->vel.z);
            s = (float)sqrt((double)((p->vel.x * p->vel.x) + p->vel.y * p->vel.y + p->vel.z * p->vel.z));
            speed = s;
            if ((float)sqrt(x * x + y * y + z * z) <= ps->v00.w && speed > ps->v10.y) {
                n.x = x;
                n.y = y;
                n.z = z;
                d3_Normalize(&n);
                if (n.y < 0.0f) {
                    p->flags &= ~1;
                } else {
                    p->pos.x = (ps->v00.w) * n.x;
                    p->pos.y = (ps->v00.w) * n.y;
                    p->pos.z = (ps->v00.w) * n.z;
                    d = ((n.x * p->vel.x) + n.y * p->vel.y + n.z * p->vel.z) * -2.0f;
                    p->vel.x = (x = p->vel.x + n.x * d);
                    p->vel.y = (y = p->vel.y + n.y * d);
                    p->vel.z = (z = p->vel.z + d * n.z);
                    if (speed > ps->v10.y) {
                        p->vel.x = (ps->v10.z) * x;
                        p->vel.y = (ps->v10.z) * y;
                        p->vel.z = (ps->v10.z) * z;
                    }
                }
            } else {
                p->vel.y -= dt * ps->v10.w;
            }
            a = min(1.0f, max(0.0f, p->pos.y / ps->v00.y) + 0.2f) * 255.0f;
            p->start.x = a;
            if (a > 0.0 && p->pos.y > 0.0f)
                p->a = (unsigned char)a;
            else
                p->flags &= ~1;
            alive++;
        }
    }
    ps->flags &= ~1;
    if (alive == 0)
        ps->flags |= 2;
    return 1;
}
/* 0x415ad0, creates the emitter particle system of an object's mesh: par_Create with the caller's texture, flags,
   size and count, then the world position, the mesh radius and the four effect parameters; returns the system,
   0 when the object has no mesh */

D3PARTSYS *d3t_CreateEmitter(D3OBJECT *obj, char *tex, char flag, unsigned long color, D3VECTOR *pos, int count,
                             int unused, float size, float a, float b, float c, float d)
{
    D3PARTSYS *ps = 0;
    D3TEXFLAGS flags;

    if (obj && obj->info && obj->info->mesh) {
        flags.all = 0;
        flags.b[0] = 0x7f;
        flags.b[2] = flag & 1 | 2;
        ps = par_Create(count, tex, flags, size, obj, d3t_EmitterCB);
        if (ps) {
            par_SetPos(ps, &d3_Origin);
            ps->v00.x = pos->x;
            ps->v00.y = pos->y;
            ps->v00.z = pos->z;
            ps->spawnCount = 2;
            ps->v00.y += ps->v00.w = obj->info->mesh->radius;
            ps->v10.x = a;
            ps->v10.y = b;
            ps->v10.z = c;
            ps->v10.w = d;
            ps->color = color;
            ps->v00.w *= 0.75f;
        }
    }
    return ps;
}
/* 0x415bb0, callback of d3t_CreateBlood */

static char d3t_BloodCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p, *q;
    float dt, y;
    int i, dead, spawn;

    spawn = ps->flags >> 1;
    dead = 0;
    q = ps->particles;
    for (i = 0; i < ps->count; i++) {
        if (q[i].flags & 1) {
            dt = (time - ps->particles[i].time) * 0.001f;
            ps->particles[i].pos.x = ps->particles[i].start.x + ps->particles[i].vel.x;
            ps->particles[i].pos.y = ps->particles[i].start.y + ps->particles[i].vel.y;
            ps->particles[i].pos.z = ps->particles[i].start.z + ps->particles[i].vel.z;
            ps->particles[i].size += 0.8f * dt;
            ps->particles[i].vel.y -= dt;
            if (ps->particles[i].pos.y < -10.0)
                q[i].flags &= ~1;
            q[i].start.x = q[i].pos.x;
            q[i].start.y = q[i].pos.y;
            q[i].start.z = q[i].pos.z;
            q[i].a = (unsigned char)max(0.0f, q[i].pos.y * 1.5f + 50.0f);
        }
        if (!(q[i].flags & 1))
            dead++;
    }
    if (!(ps->flags & 1) && dead >= ps->count)
        return 0;
    if ((ps->flags & 1) && dead > spawn) {
        p = ps->particles;
        for (i = 0; i < ps->count && i < spawn; i++, p++) {
            if (!(p->flags & 1)) {
                p->flags |= 1;
                p->pos.x = (float)rand() / RAND_MAX - 0.5f;
                p->pos.y = (float)rand() * (ps->v00.y + 1.0f) / RAND_MAX + ps->v00.w;
                p->pos.z = (float)rand() / RAND_MAX - 0.5f;
                p->vel.x = (float)rand() * ps->v00.y * 2.0f / RAND_MAX - ps->v00.y;
                y = (p->pos.y - 55.0f) * ps->v00.z;
                p->vel.y = y - ((float)rand() / RAND_MAX * 0.1f + 0.2f);
                p->vel.z = (float)rand() / RAND_MAX * 0.05f + ((p->pos.y - 55.0f) + p->vel.y) * ps->v00.x + 0.4f;
                p->time = time;
                p->size = ps->size;
                p->start.x = p->pos.x;
                p->start.y = p->pos.y;
                p->start.z = p->pos.z;
                p->a = (unsigned char)max(0.0f, p->pos.y * 1.5f + 50.0f);
            }
        }
        ps->flags &= ~1;
    }
    return 1;
}
/* 0x415e90, creates the blood spray: 200 "Blut" particles with the effect parameters a..d; n is the number
   spawned per frame (ps->flags = n * 2 | 1, the callback reads n from flags >> 1 and bit 0 enables spawning).
   Returns the system */

D3PARTSYS *d3t_CreateBlood(void *parent, int n, float a, float b, float c, float d)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;
    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(200, "Blut", flags, 1.0f, parent, d3t_BloodCB);
    if (ps) {
        ps->flags = n * 2 | 1;
        ps->v00.x = a;
        ps->v00.y = b;
        ps->v00.z = c;
        ps->v00.w = d;
        for (i = 0; i < ps->count; i++)
            ps->particles[i].flags &= ~1;
    }
    return ps;
}
/* 0x415f30, callback of d3t_CreateExplosion. ps->flags holds the duration. */

static char d3t_ExplosionCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    float t;
    int i;

    p = ps->particles;
    t = (float)(time - p->time) / ps->flags;
    if (t >= 0.0 && t <= 1.0) {
        t = 1.0f - ((float)cos((t + 1.0f) * 1.5707964f) + 1.0f);
        for (i = 0; i < ps->count; i++, p++) {
            p->pos.x = t * p->start.x;
            p->pos.y = (p->start.y + ps->v00.z) * t;
            p->pos.z = t * p->start.z;
            p->size = (t * 0.4f + 1.0f) * ps->size;
            p->a = (unsigned char)((1.0f - t) * 255.0f);
        }
    }
    return time < ps->particles->time + ps->flags;
}
/* 0x416040, creates the explosion: count "Explosion" particles around pos, with radius / height / z as the
   effect extents and size the particle size; duration (ms) is kept in ps->flags for the callback. Each particle
   starts on a sphere of radius radius (start.y = sin(i/count * pi/2) * height, x/z from the circle) with random
   jitter, and every count/6-th is pulled in by 0.4 / 0.2 / 0.4. Returns the system */

D3PARTSYS *d3t_CreateExplosion(void *parent, D3VECTOR *pos, int count, float radius, float height, float z,
                               float size, unsigned int duration)
{
    D3PARTSYS *ps;
    D3PARTICLE *p;
    D3TEXFLAGS flags;
    D3VECTOR4 r;
    float inv, t, ang, w;
    int i, n;

    flags.all = 0;
    flags.b[0] = 0xff;
    flags.b[2] |= 1;
    ps = par_Create(count, "Explosion", flags, size, parent, d3t_ExplosionCB);
    if (ps) {
        inv = 1.0f / ps->count;
        n = ps->count / 6;
        par_SetPos(ps, pos);
        ps->v00.x = radius;
        ps->v00.y = height;
        ps->v00.z = z;
        ps->flags = duration;
        p = ps->particles;
        for (i = 0; i < ps->count; i++, p++) {
            p->flags |= 1;
            r.x = (float)rand() / RAND_MAX;
            r.y = (float)rand() / RAND_MAX;
            r.z = (float)rand() / RAND_MAX;
            t = ps->v00.y * 0.1f;
            r.x *= t;
            r.y *= t;
            r.z *= t;
            r.x -= ps->v00.y * 0.05f;
            r.y -= ps->v00.y * 0.025f;
            r.z -= ps->v00.y * 0.05f;
            ang = (float)rand() * 6.2831855f / RAND_MAX;
            p->start.y = (float)sin(i * inv * 1.5707964f) * ps->v00.y;
            w = (float)sqrt(ps->v00.x * ps->v00.x - p->start.y * p->start.y);
            p->start.x = (float)sin(ang) * w + r.x;
            p->start.y += r.y;
            p->start.z = (float)cos(ang) * w + r.z;
            if (i % n == 0) {
                p->start.x *= 0.4f;
                p->start.y *= 0.2f;
                p->start.z *= 0.4f;
            }
        }
    }
    return ps;
}
/* 0x416240, callback of d3t_CreateSmoke. Each particle rises from its start for ps->flags ms,
   fading in until unk10.w and out after it; ps->v00.x is the maximum alpha. */

static char d3t_SmokeCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    float t;
    int i, dead;

    dead = 0;
    for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
        if (time >= p->time && time <= p->time + ps->flags) {
            t = (float)(time - p->time) / ps->flags;
            p->pos.x = t * p->start.x;
            p->pos.y = t * p->start.y;
            p->pos.z = t * p->start.z;
            if (t < p->start.w)
                p->a = (unsigned char)(p->vel.x * ps->v00.x * t);
            else
                p->a = (unsigned char)((1.0 - (t - p->start.w) * p->vel.y) * ps->v00.x);
            p->flags |= 1;
        } else {
            p->flags &= ~1;
        }
        if (!(p->flags & 1))
            dead++;
    }
    return dead < ps->count;
}
/* 0x416340. NONMATCHING: 21 bytes, scheduling in the p->time line (the original loads now after
   adding rand() & 7 and the delay, and increments i later); no operand order or type changes it.
   mask is an int local: color.a is loaded as a zero-extended dword before par_SetPos. */

D3PARTSYS *d3t_CreateSmoke(void *parent, D3VECTOR *pos, int count, float size, int unused, float height,
                           float y, float alpha, float spread, D3VECTOR *start, unsigned int duration,
                           int delay, D3COLOR color)
{
    D3PARTSYS *ps;
    D3PARTICLE *p;
    D3TEXFLAGS flags;
    D3VECTOR4 r;
    unsigned long now;
    int mask;

    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(count, "Rauch", flags, size, parent, d3t_SmokeCB);
    if (ps) {
        now = d3_Time;
        mask = color.a;
        par_SetPos(ps, pos);
        ps->v00.x = alpha;
        ps->v00.y = height;
        ps->v10.x = start->x;
        ps->v10.y = start->y;
        ps->v10.z = start->z;
        ps->flags = duration;
        for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
            p->start.x = start->x;
            p->start.y = start->y;
            p->start.z = start->z;
            p->start.y = y;
            r.x = (float)rand() / RAND_MAX;
            r.y = (float)rand() / RAND_MAX;
            r.z = (float)rand() / RAND_MAX;
            r.x *= spread;
            r.y *= spread;
            r.z *= spread;
            r.x -= spread * 0.5f;
            r.y -= spread * 0.5f;
            r.z -= spread * 0.5f;
            p->start.x += r.x;
            p->start.y += r.y;
            p->start.z += r.z;
            p->start.w = (p->start.y - ps->v00.y) / p->start.y;
            p->vel.x = 1.0f / p->start.w;
            p->a = 0;
            p->vel.y = 1.0f / (1.0f - p->start.w);
            p->r = (rand() & mask) + color.r;
            p->g = (rand() & mask) + color.g;
            p->b = (rand() & mask) + color.b;
            p->time = (rand() & 7) + i * delay + now;
        }
    }
    return ps;
}
/* 0x416550, callback of d3t_CreateEarth. NONMATCHING: 15 bytes, the original puts dead in the ps
   parameter slot and a in the slot of s (ours: the reverse), and loads ps->tex before c.a.
   Clods are thrown up once, bounce on the ground at v00.w and fade out. */

static int d3t_EarthCB(D3PARTSYS *ps, unsigned long time)
{
    D3PARTICLE *p;
    D3COLOR c;
    unsigned int frames;
    int mask, i, dead;
    float len, s, k, dt;
    volatile float a;

    if (ps->flags & 1) {
        ps->flags &= ~1;
        frames = 1;
        c = ps->color24;
        mask = c.a;
        if (ps->tex && ps->tex->frames > 0)
            frames = ps->tex->frames;
        for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
            if (!(p->flags & 1)) {
                p->r = (rand() & mask) + c.r;
                p->g = (rand() & mask) + c.g;
                p->b = (rand() & mask) + c.b;
                p->frame = i % frames;
                p->pos.x = (float)((float)rand() / RAND_MAX * 4.0) - 2.0f;
                p->pos.y = (float)((float)rand() / RAND_MAX) * 1.5f;
                p->pos.z = (float)((float)rand() / RAND_MAX * 4.0) - 2.0f;
                p->vel.x = D3T_RAND(ps->v10.x);
                p->vel.y = (float)((float)rand() / RAND_MAX) * ps->v10.z + ps->v10.y;
                p->vel.z = D3T_RAND(ps->v10.x);
                len = (float)sqrt(p->vel.x * p->vel.x + p->vel.z * p->vel.z);
                if (len > ps->v10.x || len < ps->v10.x * 0.4) {
                    s = ps->v10.x * 0.25f;
                    k = (D3T_RAND(s) + ps->v10.x) / len;
                    p->vel.x = k * p->vel.x;
                    p->vel.z = k * p->vel.z;
                }
                p->start.x = (float)((rand() & 0x3f) + 0xc0);
                p->size = D3T_RAND(ps->v00.x) + ps->size;
                p->time = time;
                p->flags |= 1;
            }
        }
    }
    dead = 0;
    for (i = 0, p = ps->particles; i < ps->count; i++, p++) {
        if (p->flags & 1) {
            dt = (time - p->time) / 3.0f;
            p->time = time;
            p->pos.x += dt * p->vel.x;
            p->pos.y += dt * p->vel.y;
            p->pos.z += dt * p->vel.z;
            if (p->pos.y < ps->v00.w) {
                p->pos.y = ps->v00.w;
                p->vel.x -= p->vel.x * 0.29f;
                p->vel.y *= -0.34f;
                p->vel.z -= p->vel.z * 0.31f;
                if (fabs(p->vel.y) < 0.2) {
                    p->vel.x = 0;
                    p->vel.y = 0;
                    p->vel.z = 0;
                }
            } else if (p->vel.y != 0.0f) {
                p->vel.y -= dt * ps->v00.y;
            }
            p->start.x = (a = p->start.x - dt * ps->v00.z);
            if (a > 0.0)
                p->a = (unsigned char)a;
            else
                p->flags &= ~1;
        }
        if (!(p->flags & 1))
            dead++;
    }
    if (dead >= ps->count && !(ps->flags & 2))
        return 0;
    return 1;
}
/* 0x4168e0, creates the earth-brocken debris: count "Erdbrocken_an0" particles with the effect parameters
   a..d in v00 and e..g in v10, the base color in the color24 union member and flags = 1; returns the system */

D3PARTSYS *d3t_CreateEarth(void *parent, D3VECTOR *pos, int count, float size, float a, float b, float c,
                           float d, float e, float f, float g, D3COLOR color)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 2;
    ps = par_Create(count, "Erdbrocken_an0", flags, size, parent, (D3PARTCB)d3t_EarthCB);
    if (ps) {
        par_SetPos(ps, pos);
        ps->v00.x = a;
        ps->v00.y = b;
        ps->v00.z = c;
        ps->v00.w = d;
        ps->v10.x = e;
        ps->v10.y = f;
        ps->v10.z = g;
        ps->flags = 1;
        ps->color24 = color;
    }
    return ps;
}

/* Software mipmap chains of the snow texture, levels 1x1..256x256 (index = log2 size), built by
   d3t_CreateSnowMipmaps and freed by d3t_FreeSnowMipmaps. */
D3RECT d3_Viewport = {0};  /* 0xfc2930 */
DDSURFACEDESC2 d3t_SnowMip8[9] = {0};       /* 0x14661c0, 8-bit palettized */
D3VECTOR d3_Unk1466620 = {0};  /* 0x1466620, camera target position */
D3VECTOR d3_Unk1466630 = {0};  /* 0x1466630, camera target angles */
DDSURFACEDESC2 d3t_SnowDither[9] = {0};     /* 0x1466640, 8-bit blurred noise */
DDSURFACEDESC2 d3t_SnowMip16[9] = {0};      /* 0x1466aa0 */
DDSURFACEDESC2 d3t_SnowMip32[9] = {0};      /* 0x1466f00 */

#define D3_MASK_RED 0xff0000u    /* red channel of the X8R8G8B8 snow surfaces below */
/* 0x416980. Level 8 of each chain is converted from the texture "snow_256_ns" (or from an empty
   256x256 image if it can't be locked), the smaller levels are scaled down from it.
   NONMATCHING: ~140 asm lines. Control flow and frame match; left: the D3_ZERO slots, height stored
   via the ddpf pointer in the mip loops, mul/store order of the 16/32-bit pitch, and in the dither
   loop the slots of n/up/row/down and n being kept in a register (the original reloads it).
   @deep B3 code hunks 231, slots 30: readability done here (Q3 D3_MASK_RED, Q4 flags.all); the reload lead for n was tried (size * size inlined in the random-fill loop) and gave no codegen change; left is allocation/scheduling only - the D3_ZERO slot forms, the ddpf height stores, the 16/32-bit pitch mul/store order and the n/up/row/down slots (control flow and frame verified equal, prior reading in the address comment); no Q failure remains */
void d3t_CreateSnowMipmaps(void)
{
    D3TEXHANDLE *snow_tex;
    DDSURFACEDESC2 snow_ddsd;
    LPDIRECTDRAWPALETTE lppal;
    PALETTEENTRY pal[1024];
    D3TEXFLAGS flags;
    float *tmp, *row, *up, *down;
    float s, lo, hi, ofs, scale, t;
    int i, j, k, x, y, size, n, cnt;

    flags.all = 0;
    snow_tex = (D3TEXHANDLE *)tex_Load("snow_256_ns", flags, 0, 0);
    lppal = 0;
    if (snow_tex) {
        tex_SetHiColor((D3TEXTURE *)snow_tex, 1);
        tex_Reload(snow_tex, 0);
        if (snow_tex->surf) {
            D3_ZERO(&snow_ddsd, sizeof(snow_ddsd));
            snow_ddsd.dwSize = sizeof(snow_ddsd);
#line 3749
            D3DCHECK(snow_tex->surf->lpVtbl->Lock(snow_tex->surf, 0, &snow_ddsd, DDLOCK_WAIT|DDLOCK_READONLY, NULL));
            if (!d3d_hr) {
                if (snow_ddsd.ddpfPixelFormat.dwFlags & DDPF_PALETTEINDEXED8) {
#line 3758
                    D3DCHECK(snow_tex->surf->lpVtbl->GetPalette(snow_tex->surf, &lppal));
                    if (d3d_hr) {
#line 3762
                        D3DCHECK(snow_tex->surf->lpVtbl->Unlock(snow_tex->surf, NULL));
                        tex_Release(snow_tex);
                        snow_tex = 0;
                    } else {
#line 3768
                        D3DCHECK(lppal->lpVtbl->GetEntries(lppal, 0, 0, 256, pal));
                    }
                }
            } else {
                tex_Release(snow_tex);
                snow_tex = 0;
            }
        } else {
            tex_Release(snow_tex);
            snow_tex = 0;
        }
    }
    if (!snow_tex) {
    snow_ddsd.dwSize = sizeof(snow_ddsd);
    snow_ddsd.dwWidth = 256;
    snow_ddsd.dwHeight = 256;
    snow_ddsd.ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    snow_ddsd.ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
    snow_ddsd.ddpfPixelFormat.dwFourCC = 0;
    snow_ddsd.ddpfPixelFormat.dwRGBBitCount = 0;
    snow_ddsd.ddpfPixelFormat.dwRBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwGBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwBBitMask = 0;
    snow_ddsd.ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    snow_ddsd.lPitch = 256;
    snow_ddsd.lpSurface = mem_Alloc(256 * 256, "d3t:snow_ddsd");
    if (!snow_ddsd.lpSurface)
        return;
    }

    d3t_SnowMip8[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip8[8].dwWidth = 256;
    d3t_SnowMip8[8].dwHeight = 256;
    d3t_SnowMip8[8].ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    d3t_SnowMip8[8].ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
    d3t_SnowMip8[8].ddpfPixelFormat.dwFourCC = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRGBBitCount = 8;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwGBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwBBitMask = 0;
    d3t_SnowMip8[8].ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    d3t_SnowMip8[8].lPitch = 256;
    d3t_SnowMip8[8].lpSurface = mem_Alloc(256 * 256, "d3t:SnowMipMap8");
    if (!d3t_SnowMip8[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip8[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip8[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip8[i].dwHeight = size;
        d3t_SnowMip8[i].dwWidth = size;
        d3t_SnowMip8[i].ddpfPixelFormat = d3t_SnowMip8[8].ddpfPixelFormat;
        d3t_SnowMip8[i].lPitch = size;
        d3t_SnowMip8[i].lpSurface = mem_Alloc(size * size, "d3t:SnowMipMap8");
        if (!d3t_SnowMip8[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip8[i], &d3t_SnowMip8[8]);
    }

    d3t_SnowMip16[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip16[8].dwWidth = 256;
    d3t_SnowMip16[8].dwHeight = 256;
    d3t_SnowMip16[8].ddpfPixelFormat = d3d_UsedTexFormat.pf2;
    d3t_SnowMip16[8].lPitch = 256 * 2;
    d3t_SnowMip16[8].lpSurface = mem_Alloc(256 * 256 * 2, "d3t:SnowMipMap16");
    if (!d3t_SnowMip16[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip16[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip16[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip16[i].dwHeight = size;
        d3t_SnowMip16[i].dwWidth = size;
        d3t_SnowMip16[i].ddpfPixelFormat = d3t_SnowMip16[8].ddpfPixelFormat;
        d3t_SnowMip16[i].lPitch = size * 2;
        d3t_SnowMip16[i].lpSurface = mem_Alloc(size * d3t_SnowMip16[i].lPitch, "d3t:SnowMipMap16");
        if (!d3t_SnowMip16[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip16[i], &d3t_SnowMip16[8]);
    }

    d3t_SnowMip32[8].dwSize = sizeof(DDSURFACEDESC2);
    d3t_SnowMip32[8].dwWidth = 256;
    d3t_SnowMip32[8].dwHeight = 256;
    d3t_SnowMip32[8].ddpfPixelFormat.dwSize = sizeof(DDPIXELFORMAT);
    d3t_SnowMip32[8].ddpfPixelFormat.dwFlags = DDPF_RGB;
    d3t_SnowMip32[8].ddpfPixelFormat.dwFourCC = 0;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRGBBitCount = 32;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRBitMask = D3_MASK_RED;
    d3t_SnowMip32[8].ddpfPixelFormat.dwGBitMask = 0xff00;
    d3t_SnowMip32[8].ddpfPixelFormat.dwBBitMask = 0xff;
    d3t_SnowMip32[8].ddpfPixelFormat.dwRGBAlphaBitMask = 0;
    d3t_SnowMip32[8].lPitch = 256 * 4;
    d3t_SnowMip32[8].lpSurface = mem_Alloc(256 * 256 * 4, "d3t:SnowMipMap32");
    if (!d3t_SnowMip32[8].lpSurface)
        return;
    surf_Blit(&d3t_SnowMip32[8], pal, &snow_ddsd, pal);
    for (i = 0; i < 8; i++) {
        size = 1 << i;
        d3t_SnowMip32[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowMip32[i].dwHeight = size;
        d3t_SnowMip32[i].dwWidth = size;
        d3t_SnowMip32[i].ddpfPixelFormat = d3t_SnowMip32[8].ddpfPixelFormat;
        d3t_SnowMip32[i].lPitch = size * 4;
        d3t_SnowMip32[i].lpSurface = mem_Alloc(size * d3t_SnowMip32[i].lPitch, "d3t:SnowMipMap32");
        if (!d3t_SnowMip32[i].lpSurface)
            return;
        surf_ScaleBlit(&d3t_SnowMip32[i], &d3t_SnowMip32[8]);
    }

    if (snow_tex) {
#line 3894
        D3DCHECK(snow_tex->surf->lpVtbl->Unlock(snow_tex->surf, NULL));
        tex_Release(snow_tex);
    } else {
        mem_Free(snow_ddsd.lpSurface);
        snow_ddsd.lpSurface = 0;
    }

    /* Dither patterns: random noise, blurred four times with the 4-neighbourhood, stretched to 0..255 */
    tmp = mem_Alloc(256 * 256 * sizeof(float), "d3t:SnowDitherTemp");
    if (!tmp)
        return;
    for (i = 0; i < 9; i++) {
        size = 1 << i;
        n = size * size;
        for (j = 0; j < n; j++)
            tmp[j] = (float)(rand() & 0xff);
        for (k = 0; k < 4; k++) {
            for (y = 0; y < size; y++) {
                up = &tmp[(y - 1) * size];
                row = &tmp[y * size];
                down = &tmp[(y + 1) * size];
                for (x = 0; x < 1 << i; x++) {
                    s = row[x];
                    cnt = 1;
                    if (x > 0) {
                        s += row[x - 1];
                        cnt++;
                    }
                    if (x < size - 1) {
                        s += row[x + 1];
                        cnt++;
                    }
                    if (y > 0) {
                        s += up[x];
                        cnt++;
                    }
                    if (y < size - 1) {
                        s += down[x];
                        cnt++;
                    }
                    row[x] = s / cnt;
                }
            }
        }
        lo = 1e35f;
        hi = -1e35f;
        for (j = 0; j < n; j++) {
            if (tmp[j] < lo)
                lo = tmp[j];
            if (tmp[j] > hi)
                hi = tmp[j];
        }
        ofs = -lo;
        scale = 255.0f / (hi - lo);
        for (j = 0; j < n; j++) {
            t = (tmp[j] + ofs) * scale;
            tmp[j] = min(max(0.0f, t), 255.0f);
        }
        d3t_SnowDither[i].dwSize = sizeof(DDSURFACEDESC2);
        d3t_SnowDither[i].dwHeight = size;
        d3t_SnowDither[i].dwWidth = size;
        d3t_SnowDither[i].ddpfPixelFormat.dwFlags = DDPF_PALETTEINDEXED8;
        d3t_SnowDither[i].ddpfPixelFormat.dwRGBBitCount = 8;
        d3t_SnowDither[i].lPitch = size;
        d3t_SnowDither[i].lpSurface = mem_Alloc(size * size, "d3t:SnowDither");
        if (!d3t_SnowDither[i].lpSurface)
            return;
        for (j = 0; j < n; j++)
            ((unsigned char *)d3t_SnowDither[i].lpSurface)[j] = (unsigned char)tmp[j];
    }
    mem_Free(tmp);
}

/* Sun rays ("sonnenstrahlen"): a camera-attached object pointing at the light "pLicht", faded in
   and out over 500 ms. */

/* 0x417150 @deep Q2 d3t_Unk631128: write-only (FadeOut stores the argument, FadeIn 0); no read anywhere in src and no caller of either fade function exists, so no role can be stated - the name needs the original's reader (match-deep). Byte otherwise */
void d3t_FadeOutSunRays(int unused, int b)
{
    d3t_SunFade = -1;
    d3t_SunFadeStart = d3_Time;
    d3t_Unk631128 = b;
}
/* 0x417170, fades the sun rays in: SunFade = 1 from now and the value d3t_FadeOutSunRays stored is cleared @deep Q2 d3t_Unk631128: the same write-only global (FadeOut stores its argument, this function 0); no read in src, no caller of either function, no role evidence - match-deep's name (Q6 fixed here: the 0x417170 comment now describes the fade-in). Byte otherwise */

void d3t_FadeInSunRays(D3OBJECT *unused)
{
    d3t_SunFade = 1;
    d3t_SunFadeStart = d3_Time;
    d3t_Unk631128 = 0;
}

void d3t_UpdateSunRays(D3OBJECT *obj);
void d3t_RemoveSunRays(D3OBJECT *obj);

#define D3T_VSET(v, a, b, c) ((v).x = (a), (v).y = (b), (v).z = (c), (D3VECTOR *)&(v))

/* 0x417190, creates the "sonnenstrahlen" object at the camera position, rotated to the light "pLicht" (yaw of
   the sun), marks it kind 5 / mask 1 / state flags 0xc and fades it in; returns it */
D3OBJECT *d3t_CreateSunRays(void)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *light;

    light = d3_FindObject(0, 8, 0, "pLicht");
    d3t_SunRays = d3_CreateObject(0, D3T_VSET(pos, d3_Camera->pos.x, d3_Camera->pos.y, d3_Camera->pos.z),
                                        D3T_VSET(rot, 0.0f, light->rotation.y, 0.0f), "sonnenstrahlen");
    d3t_SunRays->stateFlags |= 0xc;
    d3t_SunRays->objectFlags &= ~2;
    d3t_SunRays->kind = 5;
    d3t_SunRays->mask = 1;
    d3t_SunRays->flags &= ~4;
    d3t_FadeInSunRays(d3t_SunRays);
    d3t_UpdateSunRays(d3t_SunRays);
    return d3t_SunRays;
}
/* 0x417270, smooth Hermite step: t = clamp((x - a) / (b - a), 0, 1), returns (3 - 2t) * t * t (0 below a,
   1 above b) */

float d3t_SmoothStep(float a, float b, float x)
{
    float t = min(max((x - a) / (b - a), 0.0f), 1.0f);
    return (3.0f - (t + t)) * t * t;
}
/* 0x4172e0, updates the sun rays each frame: follows the camera position, turns toward the light "pLicht"
   (0.0002 rad steps while more than 0.01 off), fades by the camera's yaw / pitch angle to the sun
   (d3t_SmoothStep) and by the 500 ms fade in / out, then sets the object alpha (128 * fade * the two angles) */

void d3t_UpdateSunRays(D3OBJECT *obj)
{
    D3VECTOR4 pos;
    D3OBJECT *light;
    D3TEXFLAGS f;
    float d, a, s1, s2, fade;

    if (!obj || !obj->info || !d3_Camera)
        return;
    if (!d3_VecEqual(D3T_VSET(pos, d3_Camera->pos.x, d3_Camera->pos.y, d3_Camera->pos.z), &obj->pos, 0.0001f))
        d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    light = d3_FindObject(0, 8, 0, "pLicht");
    if (light) {
        d = light->rotation.y - obj->rotation.y;
        if (fabs(d) >= (double)0.01f) {
            D3VECTOR rot;
            d = (d > 0.0f ? 0.0002f : -0.0002f) + obj->rotation.y;
            d3_SetObjectRot(obj, D3T_VSET(rot, 0.0f, d, 0.0f));
        }
    }
    a = (float)fmod(obj->rotation.y - d3_Camera->rotation.y, 6.2831855f);
    if (a < 0.0f)
        a += 6.2831855f;
    a = (float)fmod(a, 6.2831855f);
    s1 = d3t_SmoothStep(1.0995574f, 3.1415927f, a > 3.1415927f ? 3.1415927f - (a - 3.1415927f) : a);
    a = (float)fmod(d3_Camera->rotation.x, 6.2831855f);
    a = (float)fmod(a, 6.2831855f);
    s2 = d3t_SmoothStep(-1.1780972f, 0.0f, a);
    if (d3t_SunFade == 1) {
        fade = ((float)d3_Time - d3t_SunFadeStart) * 0.002f;
        if (fade >= 1.0)
            d3t_SunFade = 0;
    } else if (d3t_SunFade == -1) {
        fade = 1.0f - ((float)d3_Time - d3t_SunFadeStart) * 0.002f;
        if (fade <= 0.0f) {
            d3t_SunFade = 0;
            d3t_RemoveSunRays(obj);
            d3t_SunRays = 0;
            return;
        }
    } else {
        fade = 1.0f;
    }
    f.all = 0;
    f.b[2] = 1;
    f.b[0] = (unsigned char)(fade * s2 * s1 * 128.0f);
    d3_SetObjectAlpha(obj, obj->info, f);
}
/* 0x417560, removes the sun rays object obj: switches to the object's scene, deletes it and clears the
   fade state, then switches back to the previous scene */

void d3t_RemoveSunRays(D3OBJECT *obj)
{
    int old = d3_CurrentScene;
    int idx = -1;
    if (obj->scene != d3_DefaultScene) {
        idx = d3t_SceneIndex(obj->scene);
        d3if_SelectScene(idx, 1);
    }
    d3if_Delete(obj);
    d3t_SunFade = 0;
    if (idx != -1)
        d3if_SelectScene(old, 1);
}
/* 0x4175c0, frees the surfaces of the four software snow mipmap chains (8/16/32-bit and the dither noise,
   levels 1x1..256x256) and clears their pointers */

void d3t_FreeSnowMipmaps(void)
{
    int i;
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip8[i].lpSurface);
        d3t_SnowMip8[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip16[i].lpSurface);
        d3t_SnowMip16[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowMip32[i].lpSurface);
        d3t_SnowMip32[i].lpSurface = 0;
    }
    for (i = 0; i < 9; i++) {
        mem_Free(d3t_SnowDither[i].lpSurface);
        d3t_SnowDither[i].lpSurface = 0;
    }
}
