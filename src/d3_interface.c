/* d3_interface -- 0x446e10..0x44aa10, the old d3_obj.c joined (its .data, d3o_LightSet / d3o_FogSet 0x5fbcfc, heads
   the block of the d3_interface literals); ends before d3_int_io.c (codegen).
   unit of rea 8cef7d5b (src/game/d3_interface.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\Units\\d3\\d3_interface.c"
/* d3_interface.c -- "D:\develop\Units\d3\d3_interface.c" (string at 0x5fbd94): the engine's object API
   (creating, deleting, showing objects) and the scene switch. Starts at 0x446e10 (after the d3_fl TU,
   whose last function is d3fl_SetShift at 0x446dc0) and ends at 0x44aa10.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <d3d.h>
#define d3_SetParent d3_SetParent_void      /* d3.h declares it void; it returns char */
#include "d3.h"
#undef d3_SetParent
#include "d3_vec.h"
#include "d3_engine.h"
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_d3d.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "bio.h"
#include "d3_light.h"
#include "data_views.h"
#include "f3init.h"
#include "ch.h"
#include "d3_int_io.h"
#include "d3_io_t.h"
#include "d3_par.h"
#include "d3_sky.h"
#include "d3_sm.h"
#include "d3_tools.h"
#include "d3s.h"
#include "dd_vesa.h"
#include "err.h"
#include "f3gm.h"
#include "mem.h"
#include "ts_texture.h"
#include "d3_pose.h"
#define dd_ClearB ((void (*)(D3RECT *, unsigned char, unsigned char, unsigned char))dd_Clear)  /* 0x41e100 (dd_vesa.h: RECT *, int x3): @original this unit passes the fog colour as bytes, its D3RECT viewport */
#define d3_PointToRefD ((void (*)(D3OBJECT *, D3OBJECT *, D3VECTOR *, D3VECTOR4 *))d3_PointToRef)  /* 0x458b20 (d3_pose.h: XOBJ *, XOBJ *, float *, float *): this unit's D3OBJECT / vector view */
#define d3_WorldMatrixD ((void (*)(D3OBJECT *, D3OBJECT *, int, void *))d3_WorldMatrix)  /* 0x458da0 (d3_pose.h: XOBJ *, XOBJ *, char, D3MATRIX *): this unit's D3OBJECT view of the object */

void d3fl_FreeMap(D3FLMAP *map);  /* 0x43ff90, d3_fl.c; declared here because a d3_fl.h prototype perturbs d3_tools.c's d3t_UpdateSunRays */
int d3fl_Prerender(D3FLMAP *map);  /* 0x446180, d3_fl.c; declared here for the same reason (a d3_fl.h prototype perturbs d3_tools.c) */
/* 0x446e10, creates a mesh object named `name` (loading its mesh data), optionally as a child of `a`,
   at pos / rot; returns the object or 0 when the mesh could not be loaded */
D3OBJECT *d3_CreateObject(int a, D3VECTOR *pos, D3VECTOR *rot, char *name)
{
    D3OBJECT *obj;
    unsigned int i, n;
    D3TEXHANDLE **tex;

    obj = d3_SpawnObject(4, name);
    if (obj) {
        if (a)
            d3_AddChild((D3OBJECT *)a, obj);
        if (!d3io_FindStock(name))
            d3io_LoadObject(name, name);
        d3_AttachMesh(obj, 0, name);
        if (D3_MESH(obj) && D3_MESH(obj)->surf[0].info.mesh) {
            for (i = 0; i < D3_MESH(obj)->nsurf; i++) {
                if (D3_MESH(obj)->surf[i].info.mesh) {
                    n = D3_OPS(&D3_MESH(obj)->surf[i].info)->count;
                    tex = (D3TEXHANDLE **)D3_MESH(obj)->surf[i].texHandles;
                    for (; n > 0; n--, tex++)
                        if (*tex)
                            tex_Reload(*tex, 0);
                }
            }
            d3_SetObjectPos(obj, pos);
            d3_SetObjectRot(obj, rot);
            if (!a)
                d3_LinkTop(obj);
            return obj;
        }
        d3_DisposeObject(obj);
    }
    return 0;
}
/* 0x446f30, switches the texture set of a mesh part: loads the textures named in the part's table for
   `set` in place of the current ones (reloading them when `reload`) */

char d3if_SelectTextureSet(D3OBJECT *obj, D3SURF *surf, unsigned char set, char reload)
{
    char ok = 1;
    int i;
    D3FLTEX *old;
    D3TEXHANDLE *tex;
    char *name;
    int opts, fmt;
    char buf1[256], buf2[256];
    int j, replaced;
    D3OPS *ops;

    if (!obj || !D3_MESH(obj) || !surf)
        return 0;
    if (set == surf->texSet)
        return 1;
    obj->state |= 4;
    D3_MESH(obj)->mirrorSource = 0;
    if (D3_MESH(obj)->mirrorPoints) {
        mem_Free(D3_MESH(obj)->mirrorPoints);
        D3_MESH(obj)->mirrorPoints = 0;
    }
    if (D3_MESH(obj)->mirrorPolys) {
        mem_Free(D3_MESH(obj)->mirrorPolys);
        D3_MESH(obj)->mirrorPolys = 0;
    }
    ops = D3_OPS(&surf->info);
    if (set < ops->nsets) {
        for (i = 0; i < ops->count; i++) {
            surf->texSet = set;
            name = ops->texnames + (ops->count * set + i) * 0x40;
            if (*name && (old = ((D3FLTEX **)surf->texHandles)[i]) != 0) {
                tex_GetParams(old, &opts, &fmt);
                tex = (D3TEXHANDLE *)tex_Load(name, old->flags, opts, fmt);
                if (tex) {
                    D3PTRI *t;

                    replaced = 0;
                    if (reload)
                        tex_Reload(tex, 0);
                    ((D3TEXHANDLE **)surf->texHandles)[i] = tex;
                    t = (D3PTRI *)surf->info.tris;
                    for (j = 0; j < surf->info.ntris; j++, t++)
                        if (((D3SRCPOLY *)t->src)->slot == i) {
                            t->tex = (D3TEXTURE *)tex;
                            replaced++;
                        }
                    if (!replaced)
                        sprintf(buf2, "SelectTextureSet: no polys w/ %s found to replace w/ %s in %s", old->name, name, obj->name);
                    tex_Release((D3TEXHANDLE *)old);
                } else {
                    sprintf(buf1, "SelectTextureSet: cannot replace %s with %s (new not found) in %s", old->name, name, obj->name);
                    ok = 0;
                }
            }
        }
    } else
        ok = 0;
    return ok;
}
/* 0x447190, unlinks and disposes an object */

char d3if_Delete(D3OBJECT *obj)
{
    if (obj) {
        d3_Unlink(obj);
        d3_DisposeObject(obj);
        return 1;
    }
    return 0;
}
/* 0x4471b0, hides (show = 0: type saved in +0x21c and set to 1) or shows an object again */

char d3if_Show(D3OBJECT *obj, char show)
{
    char buf[64];
    char *p, *q;

    if (obj) {
        if (show) {
            if (obj->type == 1) {
                if (obj->name[0] == '!') {
                    p = obj->name + 1;
                    q = buf;
                    while ((*q++ = *p++))
                        ;
                    p = buf;
                    q = obj->name;
                    while ((*q++ = *p++))
                        ;
                }
                if (obj->type0 == 5 || obj->type0 == 6)
                    d3_LightInvalidate(0);
                obj->type = obj->type0;
                return 1;
            }
        } else if (obj->type != 1) {
            obj->type0 = obj->type;
            obj->type = 1;
        }
        return 1;
    }
    return 0;
}
/* 0x447260, d3_ForEachObject callback of d3if_ShowTree (the same as d3if_Show, without the type 6 test) */

char d3if_ShowCB(D3OBJECT *obj, char show)
{
    char buf[64];
    char *p, *q;

    if (show) {
        if (obj->type == 1) {
            if (obj->name[0] == '!') {
                p = obj->name + 1;
                q = buf;
                while ((*q++ = *p++))
                    ;
                p = buf;
                q = obj->name;
                while ((*q++ = *p++))
                    ;
            }
            if (obj->type0 == 5)
                d3_LightInvalidate(0);
            obj->type = obj->type0;
            return 1;
        }
    } else if (obj->type != 1) {
        obj->type0 = obj->type;
        obj->type = 1;
    }
    return 1;
}
/* 0x447300, d3if_Show for an object and everything below it */

char d3if_ShowTree(D3OBJECT *obj, char show)
{
    if (obj) {
        d3_ForEachObject(d3_DefaultScene, obj, 0x3ff, (D3OBJECTCB)d3if_ShowCB, (void *)(unsigned char)show);
        return 1;
    }
    return 0;
}
/* 0x447340, reloads the mesh data of an object from the file `name` */

char d3if_Reload(D3OBJECT *obj, char *name)
{
    D3OBJECT *next, *g;

    if (obj) {
        if (obj->child) {
            d3_DisposeObject(obj->child);
            obj->child = 0;
        }
        next = obj->next;
        if (next && !(next->state & 1)) {
            g = d3_NextGroup(obj);
            d3_DisposeObject(next);
            obj->next = g;
        }
        d3_FreeMesh(obj);
        d3io_LoadObject(name, name);
        d3_AttachMesh(obj, 0, name);
        tex_ReloadAll(0, 0);
        if (D3_MESH(obj) && D3_MESH(obj)->surf[0].info.mesh) {
            d3_RelightObject(obj);
            return 1;
        }
    }
    return 0;
}
/* 0x4473f0, empties the current scene: all objects, particle systems, the map, the sky, and resets
   the lighting globals */

void d3if_Reset(void)
{
    int i;
    D3LIGHTSET *l;

    while (d3_TopFirst != &d3_ObjEnd)
        d3_DisposeObject(d3_TopFirst);
    while ((D3PARTSYS *)d3_ParHead != &d3_ParEnd)
        par_Delete((D3PARTSYS *)d3_ParHead);
    if (d3_Terrain) {
        d3fl_FreeMap(d3_Terrain);
        d3_Terrain = 0;
    }
    if (d3_Sky) {
        d3sky_Destroy(d3_Sky);
        d3_Sky = 0;
    }
    if (((D3IFSCENE *)d3_DefaultScene)->events) {
        mem_Free(((D3IFSCENE *)d3_DefaultScene)->events);
        ((D3IFSCENE *)d3_DefaultScene)->events = 0;
    }
    d3_TopFirst = &d3_ObjEnd;
    d3_TopLast = &d3_TopHead;
    d3_SunColor.x = 0;
    d3_SunColor.y = 0;
    d3_SunColor.z = 0;
    d3if_Gray = 200.0f;
    d3_AmbientColor.x = 200.0f;
    d3_AmbientColor.y = 200.0f;
    d3_AmbientColor.z = 200.0f;
    for (i = 0; i < 6; i++) {
        d3if_Fog[i].col.col = 0;
        d3if_Fog[i].end = 0;
        d3if_Fog[i].start = 0;
    }
    for (i = 0; i < 7; i++) {
        d3if_Levels[i].amb.x = d3_AmbientColor.x;
        d3if_Levels[i].amb.y = d3_AmbientColor.y;
        d3if_Levels[i].amb.z = d3_AmbientColor.z;
        d3if_Levels[i].sun.x = d3_SunColor.x;
        d3if_Levels[i].sun.y = d3_SunColor.y;
        d3if_Levels[i].sun.z = d3_SunColor.z;
        D3_COPY(d3if_Levels[i].fog, offset d3if_Fog, sizeof(d3if_Levels[i].fog));
    }
    d3if_MegaCam = 0;
    d3if_CamSeite = 0;
    d3if_CamVorne = 0;
    d3if_CamOben = 0;
    d3if_CreateCameras();
    d3_SceneFlags = 0;
    d3if_SceneState0 = 4;
    d3if_SceneState1 = 0xf;
    D3IF_SCENE(d3_CurrentScene).last = 0;
    D3IF_SCENE(d3_CurrentScene).first = 0;
    D3IF_SCENE(d3_CurrentScene).dirty = 0;
    D3IF_SCENE(d3_CurrentScene).loaded = 0;
    d3_Scenes[d3_CurrentScene].name[0] = 0;
}
/* 0x4475f0, initializes an empty scene: its first object is a camera ("MegaCam") */

char d3if_InitScene(D3IFSCENE *s)
{
    D3OBJECT *cam;
    int i;

    if (s && !s->first && !s->par1) {
        cam = d3_SpawnObject(3, "MegaCam");
        s->campos.x = 0;
        s->campos.y = 0;
        s->campos.z = -256.0f;
        d3_SetObjectPos(cam, &s->campos);
        s->camrot.y = 0;
        s->camrot.x = 0;
        s->camrot.z = 0;
        d3_SetObjectRot(cam, &s->camrot);
        cam->scene = (D3SCENE *)s;
        cam->next = &d3_ObjEnd;
        cam->prev = &d3_TopHead;
        cam->state |= 3;
        s->first = s->last = s->cam = cam;
        s->par1 = &d3_ParEnd;
        s->par2 = d3if_ParList;
        s->hardware = d3_Hardware;
        s->sceneFlags = 0;
        s->amb.x = 200.0f;
        s->amb.y = 200.0f;
        s->amb.z = 200.0f;
        s->map = 0;
        s->sky = 0;
        s->mapSize = 0;
        s->hmap = 0;
        s->sun.x = 0;
        s->sun.y = 0;
        s->sun.z = 0;
        s->fog.col.col = 0;
        s->fog.end = 0;
        s->fog.start = 0;
        for (i = 0; i < 6; i++) {
            s->fogs[i].col.col = s->fog.col.col;
            s->fogs[i].start = s->fog.start;
            s->fogs[i].end = s->fog.end;
        }
        for (i = 0; i < 7; i++) {
            s->levels[i].amb.x = s->amb.x;
            s->levels[i].amb.y = s->amb.y;
            s->levels[i].amb.z = s->amb.z;
            s->levels[i].sun.x = s->sun.x;
            s->levels[i].sun.y = s->sun.y;
            s->levels[i].sun.z = s->sun.z;
            D3_COPY3(s->levels[i].fog, s->fogs, sizeof(s->levels[i].fog));
        }
        s->state0 = 4;
        s->state1 = 0xf;
        return 1;
    }
    return 0;
}

typedef struct D3IFTEX {          /* view of a texture slot: D3TEXTURE with the flags as unsigned char */
    char pad00[0x64];
    void *d3dtex;                   /* 0x64 */
    unsigned char state;            /* 0x68, bit 7 = precached */
} D3IFTEX;

/* 0x447800, d3_ForEachObject2 callback: precaches the textures of the object's mesh */
void d3if_PrecacheCB(D3OBJECT *obj)
{
    D3OBJINFO *info;
    D3IFTEX **tex;
    int i, n;

    info = obj->info;
    if (info && info->mesh) {
        n = D3_OPS(info)->count;
        tex = (D3IFTEX **)((int *)info)[5];
        for (i = 0; i < n; i++)
            if (tex[i] && tex[i]->d3dtex && !(tex[i]->state & 0x80)) {
                d3d_PrecacheTexture(tex[i]->d3dtex);
                tex[i]->state |= 0x80;
            }
    }
}
/* 0x447860, precaches the textures of all objects (inside Begin/EndScene) and prerenders the map */

void d3if_Prerender(char map)
{
    unsigned int i;

    for (i = 0; i < d3_TexCount; i++)
        ((D3TEXTURE *)d3_Textures)[i].flags &= 0x7f;
    d3d_Device->lpVtbl->BeginScene(d3d_Device);
    d3_ForEachObject2(d3_DefaultScene, 0, 0x40, d3if_PrecacheCB);
    if (d3_Terrain && map)
        d3fl_Prerender(d3_Terrain);
    d3d_Device->lpVtbl->EndScene(d3d_Device);
}
/* 0x4478e0, makes scene `index` the current one: saves the engine's global state into the old scene and
   loads the new scene's; with `keep` the objects, sky and map of the old state stay as they are
   @deep read the whole function against the original: 353 vs 353 instructions, 1400 vs 1400 bytes, the
   same call set. Q2 settled: the two write-only scene dwords are d3if_SceneState0 / d3if_SceneState1
   (D3IFSCENE.state0 / state1, a reasoned guess documented on both), d3if_SceneSwitches counts the scene
   switches and d3if_LastMegaCam keeps the camera of the scene being left. What is left is the order in
   which the scene-to-global restore block is emitted: ours stores TopLast = scene.last and state1 =
   scene.state1 two statements earlier than the original, which emits ParHead = scene.par1, TopLast, Sky,
   TopFirst, SceneFlags, Terrain, state1, ...; the instruction pair at 0x447bd6 and 0x447c2e moves by one
   slot and the four fpu hunks are the 0.3 / 0.59 / 0.11 constants (the original pools them at 0x5ebb30 /
   0x5eb99c / 0x5ebb34, ours are literals of our own). Tried: reordering the restore statements into the
   original's emitted order (worse: 8 -> 28 differing), the two-loop-variable and negation spellings; the
   source order is not the driver, the scheduler is. (references: M8 at +0x296: ours refers to
   d3_Scenes+0x84 (0xfd7c24), the original's field to 0xfd7c44 (d3_Scenes+0xa4) (and 15 more), explained:
   the 16 ref lines are the same scene-to-global pairs emitted in another order: every one of them is the
   same assignment on both sides (TopLast = scene+0x84, ParHead = scene+0xa4, Sky = scene+0xb8, TopFirst =
   scene+0x80, SceneFlags = scene+0x3d8, Terrain = scene+0xac, state0 = scene+0x3cc, state1 = scene+0x3d0,
   ParTail = scene+0xa8, AmbientColor.x/y/z = scene+0xbc/0xc0/0xc4, SunColor.x = scene+0xc8), the aligner
   pairs each store with the next because our two leading stores move up two slots; the prover's first
   difference is exactly that pair (ours store4 [0xfc28f4] = scene+0x84 against the original's store4
   [0xfb98f4] = scene+0xa4, both present on both sides later)) */

char d3if_SelectScene(int index, char keep)  /* @original: the definition returns char and takes a char keep (byte-matched); the headers declare void (int, int) */
{
    D3OBJECT *cam;

    if (index >= 0x40)
        return 0;
    if (index != d3_CurrentScene) {
        if (!keep && d3_Mirror.obj && d3_Mirror.planes) {
            mem_Free(d3_Mirror.planes);
            d3_Mirror.planes = 0;
            d3_Mirror.obj = 0;
            d3_Mirror.info = 0;
        }
        D3IF_SCENE(d3_CurrentScene).first = d3_TopFirst;
        D3IF_SCENE(d3_CurrentScene).last = d3_TopLast;
        if (D3IF_SCENE(d3_CurrentScene).first == &d3_ObjEnd && D3IF_SCENE(d3_CurrentScene).last == &d3_ObjEnd) {
            D3IF_SCENE(d3_CurrentScene).last = 0;
            D3IF_SCENE(d3_CurrentScene).first = 0;
        }
        D3IF_SCENE(d3_CurrentScene).par1 = d3_ParHead;
        D3IF_SCENE(d3_CurrentScene).par2 = d3_ParTail;
        if (D3IF_SCENE(d3_CurrentScene).par1 == &d3_ParEnd && D3IF_SCENE(d3_CurrentScene).par2 == &d3_ParEnd) {
            D3IF_SCENE(d3_CurrentScene).par2 = 0;
            D3IF_SCENE(d3_CurrentScene).par1 = 0;
        }
        D3IF_SCENE(d3_CurrentScene).map = d3_Terrain;
        D3IF_SCENE(d3_CurrentScene).sky = d3_Sky;
        D3IF_SCENE(d3_CurrentScene).sceneFlags = d3_SceneFlags;
        D3IF_SCENE(d3_CurrentScene).amb.x = d3_AmbientColor.x;
        D3IF_SCENE(d3_CurrentScene).amb.y = d3_AmbientColor.y;
        D3IF_SCENE(d3_CurrentScene).amb.z = d3_AmbientColor.z;
        D3IF_SCENE(d3_CurrentScene).sun.x = d3_SunColor.x;
        D3IF_SCENE(d3_CurrentScene).sun.y = d3_SunColor.y;
        D3IF_SCENE(d3_CurrentScene).sun.z = d3_SunColor.z;
        D3_COPY(D3IF_SCENE(d3_CurrentScene).levels, offset d3if_Levels, sizeof(D3IF_SCENE(d3_CurrentScene).levels));
        D3IF_SCENE(d3_CurrentScene).fog.col.col = d3_fog_color.col;
        D3IF_SCENE(d3_CurrentScene).fog.start = d3_FogStart1;
        D3IF_SCENE(d3_CurrentScene).fog.end = d3_FogEnd2;
        D3_COPY(D3IF_SCENE(d3_CurrentScene).fogs, offset d3if_Fog, sizeof(D3IF_SCENE(d3_CurrentScene).fogs));
        D3IF_SCENE(d3_CurrentScene).state0 = d3if_SceneState0;
        D3IF_SCENE(d3_CurrentScene).state1 = d3if_SceneState1;
        D3IF_SCENE(d3_CurrentScene).hardware = d3_Hardware;
        cam = (D3OBJECT *)d3if_MegaCam;
        D3IF_SCENE(d3_CurrentScene).cam = cam;
        if (cam) {
            D3IF_SCENE(d3_CurrentScene).campos.x = cam->pos.x;
            D3IF_SCENE(d3_CurrentScene).campos.y = cam->pos.y;
            D3IF_SCENE(d3_CurrentScene).campos.z = cam->pos.z;
            D3IF_SCENE(d3_CurrentScene).camrot.x = cam->rotation.x;
            D3IF_SCENE(d3_CurrentScene).camrot.y = cam->rotation.y;
            D3IF_SCENE(d3_CurrentScene).camrot.z = cam->rotation.z;
        }
        if (!D3IF_SCENE(index).first)
            d3if_InitScene(&D3IF_SCENE(index));
        d3_ParHead = D3IF_SCENE(index).par1;
        d3_Sky = D3IF_SCENE(index).sky;
        d3_TopLast = D3IF_SCENE(index).last;
        d3_SceneFlags = D3IF_SCENE(index).sceneFlags;
        d3_Terrain = D3IF_SCENE(index).map;
        d3if_SceneState1 = D3IF_SCENE(index).state1;
        d3_TopFirst = D3IF_SCENE(index).first;
        d3_AmbientColor.y = D3IF_SCENE(index).amb.y;
        d3_ParTail = D3IF_SCENE(index).par2;
        d3_AmbientColor.x = D3IF_SCENE(index).amb.x;
        d3if_SceneState0 = D3IF_SCENE(index).state0;
        d3_AmbientColor.z = D3IF_SCENE(index).amb.z;
        d3_SunColor.x = D3IF_SCENE(index).sun.x;
        d3_Camera = 0;
        d3_SunColor.y = D3IF_SCENE(index).sun.y;
        d3_SunColor.z = D3IF_SCENE(index).sun.z;
        d3if_Gray = d3_AmbientColor.x * 0.3f + d3_AmbientColor.y * 0.59f + d3_AmbientColor.z * 0.11f;
        D3_COPY_TO(offset d3if_Fog, D3IF_SCENE(index).fogs, sizeof(D3IF_SCENE(index).fogs));
        D3_COPY_TO(offset d3if_Levels, D3IF_SCENE(index).levels, sizeof(D3IF_SCENE(index).levels));
        d3_CurrentScene = index;
        d3_DefaultScene = (D3SCENE *)&D3IF_SCENE(index);
        if (d3if_LastMegaCam == d3if_MegaCam)
            d3if_LastMegaCam = (int)D3IF_SCENE(index).cam;
        d3if_MegaCam = (int)D3IF_SCENE(index).cam;
        if (d3if_MegaCam) {
            d3_SetObjectPos((D3OBJECT *)d3if_MegaCam, &D3IF_SCENE(index).campos);
            d3_SetObjectRot((D3OBJECT *)d3if_MegaCam, &D3IF_SCENE(index).camrot);
            d3_SetCamera((D3OBJECT *)d3if_MegaCam);
        }
        if (!keep) {
            if (!d3_Terrain) {
                if (d3sm_Shadow)
                    d3sm_Build(d3sm_Shadow, 0);
                else {
                    d3sm_Shadow = d3sm_Create(0, -d3s_ShadowMapSize, 1);
                    d3_ForEachObject2(d3_DefaultScene, 0, 0xc0, d3s_ResetShadows);
                }
            } else if (d3sm_Shadow) {
                d3sm_Destroy(d3sm_Shadow);
                d3sm_Shadow = 0;
            }
            d3_SortPtr = d3_SortBuf;
            d3_SortCount = 0;
            d3_SetFog(D3IF_SCENE(index).fog.start, D3IF_SCENE(index).fog.end, D3IF_SCENE(index).fog.col.col);
        }
    }
    if (!keep) {
        d3if_TexturesEvicted = 0;
        if (d3_Hardware && d3d_D3D && d3_Busy <= 0 && (d3if_SceneSwitches++ & 0xf) == 0 &&
        (d3d_hr = d3d_D3D->lpVtbl->EvictManagedTextures(d3d_D3D)) != 0)
            d3d_Error(__FILE__, 682, "d3d_D3D->lpVtbl->EvictManagedTextures(d3d_D3D)");
        d3if_TexturesEvicted = 1;
    }
    return 1;
}
/* 0x447e60, empties scene `index`: the current one with d3if_Reset, any other by switching to it (keeping the
   engine state) and freeing its objects, particle systems, map and sky */

char d3if_ClearScene(int index)
{
    int cur;

    char ret = 0;
    if (index >= 0 && index < 0x40) {
    if (index != d3_CurrentScene) {
    if (D3IF_SCENE(index).first) {
        cur = d3_CurrentScene;
        d3if_SelectScene(index, 1);
        while (d3_TopFirst != &d3_ObjEnd)
            d3_DisposeObject(d3_TopFirst);
        while ((D3PARTSYS *)d3_ParHead != &d3_ParEnd)
            par_Delete((D3PARTSYS *)d3_ParHead);
        if (d3_Terrain) {
            d3fl_FreeMap(d3_Terrain);
            d3_Terrain = 0;
        }
        if (d3_Sky) {
            d3sky_Destroy(d3_Sky);
            d3_Sky = 0;
        }
        if (D3IF_SCENE(index).events) {
            mem_Free(D3IF_SCENE(index).events);
            D3IF_SCENE(index).events = 0;
        }
        d3_SceneFlags = 0;
        d3if_MegaCam = 0;
        if (cur != d3_CurrentScene)
            d3if_SelectScene(cur, 1);
    }
    } else
        d3if_Reset();
    ret = 1;
    }
    return ret;
}
/* 0x447f70, d3_ForEachObject callback of d3if_MoveObject: moves one object of the tree to the new scene and
   deletes the particle systems attached to it */

char d3if_MoveCB(D3OBJECT *obj, D3IFMOVE *m)
{
    D3PARTSYS *p, *next;

    d3_ForEachObject((D3SCENE *)m->from, 0, 7, (D3OBJECTCB)d3_LightRemoveRecords, obj);
    obj->scene = (D3SCENE *)m->to;
    for (p = m->from->par1; p != &d3_ParEnd; p = next) {
        next = p->next;
        if (p->parent == obj)
            par_Delete(p);
    }
    return 1;
}
/* 0x447fd0, moves the top-level object `obj` (with its children) from scene `from` to the end of scene `to` */

char d3if_MoveObject(D3OBJECT *obj, D3IFSCENE *from, D3IFSCENE *to)
{
    D3IFMOVE m;
    D3OBJECT *o;

    if (!obj || !from || !to || obj->parent)
        return 0;
    if (from == to)
        return 1;
    if (from == (D3IFSCENE *)d3_DefaultScene) {
        from->first = d3_TopFirst;
        from->last = d3_TopLast;
        from->par1 = d3_ParHead;
        from->par2 = d3_ParTail;
    }
    for (o = from->first; o != &d3_ObjEnd; o = o->next)
        if (o == obj)
            break;
    if (o != obj)
        return 0;
    if (obj->prev == &d3_TopHead)
        from->first = obj->next;
    else
        obj->prev->next = obj->next;
    if (obj->next == &d3_ObjEnd)
        from->last = obj->prev;
    else
        obj->next->prev = obj->prev;
    if (from == (D3IFSCENE *)d3_DefaultScene) {
        d3_TopFirst = from->first;
        d3_TopLast = from->last;
        d3_ParHead = from->par1;
        d3_ParTail = from->par2;
    }
    m.from = from;
    m.to = to;
    d3_ForEachObject((D3SCENE *)from, obj, 0x3ff, (D3OBJECTCB)d3if_MoveCB, &m);
    if (to == (D3IFSCENE *)d3_DefaultScene) {
        to->first = d3_TopFirst;
        to->last = d3_TopLast;
        to->par1 = d3_ParHead;
        to->par2 = d3_ParTail;
    } else if (!to->first)
        d3if_InitScene(to);
    obj->next = &d3_ObjEnd;
    o = to->last;
    o->next = obj;
    obj->prev = o;
    to->last = obj;
    d3_InflateObject(obj);
    if (to == (D3IFSCENE *)d3_DefaultScene) {
        d3_TopFirst = to->first;
        d3_TopLast = to->last;
        d3_ParHead = to->par1;
        d3_ParTail = to->par2;
    }
    return 1;
}
/* 0x4481e0, releases a sprite texture */

void d3if_FreeSprite(D3TEXHANDLE *t)
{
    if (t)
        tex_Release(t);
}
/* 0x448200, loads a sprite texture (alpha 0x7f) */

D3TEXHANDLE *d3if_LoadSprite(char *name, char glow)
{
    D3IFTEXFLAGS f;
    D3TEXHANDLE *t;

    memset(&f, 0, sizeof f);
    f.bits.alpha = 0x7f;
    f.bits.glow = glow;
    if (d3d_Device && (d3d_UsedCard.info.alphablend)) {
        t = (D3TEXHANDLE *)tex_Load(name, f.raw, 0, 0);
        if (t)
            tex_Reload(t, 0);
        return t;
    }
    return 0;
}

#define D3D_VERTEX_STRUCT D3DFVF_TLVERTEX

/* 0x448270, draws sprite `t` centred at screen position x, y, rotated by `angle`, scaled by sx, sy, with
   alpha 0..1, over the whole screen */
char d3if_DrawSprite(D3IFSPRITE *t, int x, int y, float angle, float sx, float sy, float alpha)
{
    unsigned char a;
    float w, h, vx, vy;
    float size, s, c;
    D3DTLVERTEX TurboBuffer[4];
    float corner[4][4];
    unsigned long color;
    int i;

    a = alpha * 255.0;
    if (!t || t->frames <= 0 || !d3d_Device || !d3_Hardware || d3_Busy > 0)
        return 0;
    d3_Busy++;
    w = d3d_ViewW;
    h = d3d_ViewH;
    vx = d3d_ViewX;
    vy = d3d_ViewY;
    d3d_SetViewport((float)dd_Width, (float)dd_Height, 0, 0, d3d_Near, d3d_Far);
    d3_BeginScene(0, 0, 0);
    D3_ZERO(TurboBuffer, sizeof TurboBuffer);
#line 973
    D3DCHECK(d3d_Device->lpVtbl->SetTexture(d3d_Device, 0, t->surftext));
    d3_SetBlendState(t->colorkey < 0xff, t->flags & 1, (t->flags >> 1) & 1);
    sx *= 0.5f;
    sy *= 0.5f;
    size = (float)t->size;
    corner[0][0] = size * sx;
    corner[0][1] = size * sy;
    corner[0][2] = 1.0f;
    corner[0][3] = 1.0f;
    corner[1][0] = -(size * sx);
    corner[1][1] = size * sy;
    corner[1][2] = 0.0f;
    corner[1][3] = 1.0f;
    corner[2][0] = -(size * sx);
    corner[2][1] = -(size * sy);
    corner[2][2] = 0.0f;
    corner[2][3] = 0.0f;
    corner[3][0] = size * sx;
    corner[3][1] = -(size * sy);
    corner[3][2] = 1.0f;
    corner[3][3] = 0.0f;
    color = (a << 24) | D3IF_RGB_MASK;
    s = (float)sin(angle);
    c = (float)cos(angle);
    for (i = 0; i < 4; i++) {
        TurboBuffer[i].sx = c * corner[i][0] - s * corner[i][1] + x;
        TurboBuffer[i].sy = s * corner[i][0] + c * corner[i][1] + y;
        TurboBuffer[i].sz = 0;
        TurboBuffer[i].rhw = 1.0f;
        TurboBuffer[i].color = color;
        TurboBuffer[i].tu = corner[i][2];
        TurboBuffer[i].tv = corner[i][3];
    }
#line 1008
    D3DCHECK(d3d_Device->lpVtbl->DrawPrimitive(d3d_Device, D3DPT_TRIANGLEFAN, D3D_VERTEX_STRUCT, (void*) &TurboBuffer, 4, D3DDP_WAIT));
    d3_EndScene();
    d3d_SetViewport(w, h, vx, vy, d3d_Near, d3d_Far);
    d3_Busy--;
    return 1;
}
/* 0x448540, d3_ForEachObject callback of d3if_Pick: keeps, per object type, the object whose projected
   bounding sphere contains the point and whose centre is nearest to it */

char d3if_PickCB(D3OBJECT *obj, D3IFPICK *p)
{
    D3VECTOR4 pos;
    float r, f, x, y, d;
    D3IFHIT *hit;

    if (p->mask && !(obj->mask & p->mask))
        return 1;
    if (D3_MESH(obj) && (D3_MESH(obj)->flags & 0x40) && D3_MESH(obj)->camera == p->cam)
        return 1;
    if (obj->type == 4 && !D3_MESH(obj))
        return 1;
    if (!d3_GetBounds(obj, p->cam, (D3VECTOR *)&pos, &r))
        return 1;
    f = 1.0f / pos.z;
    r = d3_ScaleX * f * r;
    x = d3_ScaleX * pos.x * f + p->dx;
    y = d3_ScaleY * pos.y * f + p->dy;
    hit = &p->hits[obj->type];
    d = x * x + y * y;
    if (d < hit->dist && (float)sqrt(d) < r) {
        hit->dist = d;
        hit->obj = obj;
    }
    return 1;
}
/* 0x448630, returns the object under screen position x, y (objects of a lower type win), 0 if none */

D3OBJECT *d3if_Pick(float x, float y, int flags, int mask)
{
    D3IFPICK p;
    D3OBJECT *obj;
    float best;
    unsigned int i;

    obj = 0;
    best = 1e10f;
    for (i = 0; i < 9; i++) {
        p.hits[i].dist = 1e10f;
        p.hits[i].obj = 0;
    }
    p.dx = d3_CenterX - x;
    p.cam = d3_Camera;
    p.mask = mask;
    p.dy = d3_CenterY - y;
    d3_ForEachObject(d3_DefaultScene, 0, flags, (D3OBJECTCB)d3if_PickCB, &p);
    for (i = 0; i < 9; i++)
        if (p.hits[i].obj && p.hits[i].dist < best) {
            obj = p.hits[i].obj;
            best = p.hits[i].dist;
        }
    return obj;
}
/* 0x4486e0, the screen rectangle covered by an object: a square around the centre for type 0, the projected
   bounding box (clipped to the viewport) for meshes */

char d3if_ScreenBox(D3OBJECT *obj, D3RECT *r)
{
    D3VECTOR4 v;
    D3BOXVERT *b;
    D3OBJINFO *info;
    float f, x, y, s;
    int i, ix, iy;

    if (r && obj) {
        if (obj->type == 0) {
            d3_PointToRefD(obj, d3_Camera, &obj->pos, &v);
            f = 1.0f / v.z;
            x = d3_ScaleX * v.x * f + d3_CenterX;
            y = d3_ScaleY * v.y * f + d3_CenterY;
            s = d3_ScaleX * D3IF_OBJ068(obj) * f * 0.5f;
            r->left = (int)(x - s);
            r->right = (int)(x + s);
            r->top = (int)(y - s);
            r->bottom = (int)(y + s);
            return 1;
        }
        if (D3_MESH(obj) && !(D3_MESH(obj)->flags & 0x40) && (info = obj->info) && info->mesh) {
            d3_UpdateWorldMatrix(obj, (obj->state & 0x80) ? 0 : d3_Camera, 0);
            b = D3_OPS(info)->GetBox(obj, info, D3_MESH(obj)->cameraMatrix);
            r->top = 0x10000;
            r->left = 0x10000;
            r->bottom = -0x10000;
            r->right = -0x10000;
            for (i = 0; i < 8; i++) {
                f = 1.0f / b[i].z;
                ix = (int)(d3_ScaleX * b[i].x * f + d3_CenterX);
                r->left = min(r->left, ix);
                r->right = max(r->right, ix);
                iy = (int)(d3_ScaleY * b[i].y * f + d3_CenterY);
                r->top = min(r->top, iy);
                r->bottom = max(r->bottom, iy);
            }
            r->left = max(r->left, d3_Viewport.left);
            r->right = min(r->right, d3_Viewport.right - 1);
            r->top = max(r->top, d3_Viewport.top);
            r->bottom = min(r->bottom, d3_Viewport.bottom - 1);
            return 1;
        }
    }
    return 0;
}
/* 0x448910, creates the cameras of a new scene: "MegaCam" at (0, 0, -256) and, when d3_EditorMode is set, the
   three orthographic view cameras "Oben" (above), "Vorne" (front) and "Seite" (side) */

void d3if_CreateCameras(void)
{
    d3if_MegaCam = (int)d3_SpawnObject(3, "MegaCam");
    d3_SetObjectPosXYZ((D3OBJECT *)d3if_MegaCam, 0, 0, -256.0f);
    d3_SetObjectRotXYZ((D3OBJECT *)d3if_MegaCam, 0, 0, 0);
    d3_LinkTop((D3OBJECT *)d3if_MegaCam);
    if (d3_EditorMode) {
        d3if_CamOben = (int)d3_SpawnObject(3, "Oben");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamOben, 0, 256.0f, 0);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamOben, -1.57079637f, 0, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamOben);
        ((D3OBJECT *)d3if_CamOben)->objectFlags &= 0xfd;
        d3if_CamVorne = (int)d3_SpawnObject(3, "Vorne");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamVorne, 0, 0, -256.0f);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamVorne, 0, 0, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamVorne);
        ((D3OBJECT *)d3if_CamVorne)->objectFlags &= 0xfd;
        d3if_CamSeite = (int)d3_SpawnObject(3, "Seite");
        d3_SetObjectPosXYZ((D3OBJECT *)d3if_CamSeite, 256.0f, 0, 0);
        d3_SetObjectRotXYZ((D3OBJECT *)d3if_CamSeite, 0, 1.57079637f, 0);
        d3_LinkTop((D3OBJECT *)d3if_CamSeite);
        ((D3OBJECT *)d3if_CamSeite)->objectFlags &= 0xfd;
    }
}
/* 0x448a50, clears the viewport (and refreshes the screen) when the scene wants it: with the fog colour
   unless the back buffer was just cleared */

void d3if_ClearView(void)
{
    if (d3_EngineReady && d3_Busy <= 0) {
        if (d3_Hardware) {
            d3d_Clear(&d3_Viewport);
        } else {
            if (!dd_Cleared)
                dd_ClearB(&d3_Viewport, GetBValue(d3_fog_color.col), GetGValue(d3_fog_color.col), GetRValue(d3_fog_color.col));
        }
        d3_Frame(0x40, 1, 1);
    }
}

/* 0x448ac0..0x44aa10: d3_obj.c until layout 2026-10-22 (object helpers, hierarchy, light / fog sets, scene
   writer wr_object); joined: its .data (d3o_LightSet / d3o_FogSet = -1 at 0x5fbcfc, then "Unknown Object-Type in
   wr_object()!" 0x5fbe8c) frames d3_interface.c's literals 0x5fbd08..0x5fbe80, one block. */

/* 0x448ac0, the signed horizontal angle between the facing of `obj` (its z axis) and the direction to the
   point `p`, around the vertical axis */
float d3_YawTo(D3OBJECT *obj, float *p)
{
    D3VECTOR4 a, b, pos;
    float d, e, ang, t;
    float sn, cs, u;

    d3_LocalToWorld(obj, &d3_ZAxis, (D3VECTOR *)&a);
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    a.x = a.x - pos.x;
    a.y = 0;
    b.y = 0;
    a.z = a.z - pos.z;
    b.x = p[0] - pos.x;
    b.z = p[2] - pos.z;
    d3_Normalize(&b);
    d3_Normalize(&a);
    d = a.x * b.x + a.y * b.y + a.z * b.z;
    e = min(max(-1.0f, d), 1.0f);
    ang = (float)acos(e);
    cs = cos(1.5707964f);
    sn = sin(1.5707964f);
    t = cs * a.x + sn * a.z;
    u = cs * a.z - sn * a.x;
    if (t * b.x + b.z * u + a.y * b.y > 0.0f)
        ang = -ang;
    return ang;
}
/* 0x448c20, callback of d3_Unk448e60: of all polygons textured with ctx->tex it remembers the vertex nearest
   to each of the four screen corners (by the sum / difference of x and y), with its uv
   @deep B: read all 3 hunks (asmdiff 448cba code 26, 448d27 slots 10, 448d7b code 75): hunk 1 is only the commutative operand order of the pt[0] sum (edx/ecx loaded x/y in the other order; tried swapping the source terms of c->pt[0].x + c->pt[0].y - identical code); slots hunk is x/y swapped in the frame (tried swapping the declarations i,n,np,y,x - identical code); hunk 3 is the same pt[3] comparison with the two sides evaluated R-first and dd_Width reloaded per side in the original vs interleaved with one CSE'd load in ours (tried R > L - evaluation order matches now but the CSE remains, unitdiff unchanged 167/560); no missing/wrong constant, field or condition; readable already */

char d3_CornerCB(D3OBJECT *obj, D3CORNERCTX *c)
{
    D3SURF *s = (D3SURF *)obj->info;
    D3CORNERTRI *t;
    void **tex;
    void *want;
    char fl;
    int i, n, np, x, y;

    if (s) {
        n = D3_OPS(&s->info)->count;
        tex = (void **)s->texHandles;
        want = c->tex;
        for (i = 0; i < n; i++)
            if (tex[i] == want)
                break;
        if (i < n) {
            np = s->info.ntris;
            t = (D3CORNERTRI *)s->info.tris;
            if (np > 0) {
                for (; np > 0; np--, t++) {
                    if ((t->flag24 & 0x80) && !(t->flag26 & 2) && t->tex == want) {
                        c->found = 1;
                        for (i = 0; i < 3; i++) {
                            x = (int)t->v[i]->sx;
                            y = (int)t->v[i]->sy;
                            if (y + x < c->pt[0].y + c->pt[0].x) {
                                c->pt[0].x = x;
                                c->pt[0].y = y;
                                c->uv[0][0] = t->uv[i * 2];
                                c->uv[0][1] = t->uv[i * 2 + 1];
                                c->v[0].x = t->v[i]->x;
                                c->v[0].y = t->v[i]->y;
                                c->v[0].z = t->v[i]->z;
                            }
                            if (dd_Width - x + y - 1 < c->pt[1].y - c->pt[1].x + dd_Width - 1) {
                                c->pt[1].y = y;
                                c->pt[1].x = x;
                                c->uv[1][0] = t->uv[i * 2];
                                c->uv[1][1] = t->uv[i * 2 + 1];
                                c->v[1].x = t->v[i]->x;
                                c->v[1].y = t->v[i]->y;
                                c->v[1].z = t->v[i]->z;
                            }
                            if (dd_Height - y + x - 1 < c->pt[2].x - c->pt[2].y + dd_Height - 1) {
                                c->pt[2].x = x;
                                c->pt[2].y = y;
                                c->uv[2][0] = t->uv[i * 2];
                                c->uv[2][1] = t->uv[i * 2 + 1];
                                c->v[2].x = t->v[i]->x;
                                c->v[2].y = t->v[i]->y;
                                c->v[2].z = t->v[i]->z;
                            }
                            if (dd_Height - y - x + dd_Width - 2 < dd_Height - c->pt[3].y - c->pt[3].x + dd_Width - 2) {
                                c->pt[3].x = x;
                                c->pt[3].y = y;
                                c->uv[3][0] = t->uv[i * 2];
                                c->uv[3][1] = t->uv[i * 2 + 1];
                                c->v[3].x = t->v[i]->x;
                                c->v[3].y = t->v[i]->y;
                                c->v[3].z = t->v[i]->z;
                            }
                        }
                    }
                }
            }
        }
    }
    return 1;
}
/* 0x448e60, the texture coordinates at a screen point: of the polygons of `obj` textured with `tex` it takes the
   four vertices nearest to the screen corners (d3_CornerCB), picks three of them by their (normalized) uv and
   intersects the view ray through (sx, sy) with the plane of the triangle, once for each of the two triangles
   of the quad; returns 1 and the uv when the ray hits one @deep whole function read against the original and reconciled: corrected here were the ray direction locals (dx = sx - d3_CenterX, dy = -(sy - d3_CenterY), dz = d3_ScaleX instead of overwriting sx), the plane/triangle split (the original falls into the second triangle when r + s > 1.0, so the condition is r + s > 1.0 with the second triangle first), the in-triangle test (r >= 0 && r <= 1 && s >= 0 && s <= 1, inclusive upper bounds) and hz as a local; all calls, loops and conditions now agree. Remaining: FPU stack scheduling of the plane intersection (original spills t, the three t*d products and hx/hy, ours keeps them in registers), fld/fld/fcompp vs fcom [mem] compares in the min/best loops, and the min loop assigning with fld/fstp where ours copies through edx; tried: ternary form of the min, nx/ny/nz inlined (worse), p variables dropped (worse), hz variable (identical code) (size: 507 vs 535 instructions, explained: ours 507 vs original 535 instructions, frame 0xcc vs 0x108: op histogram of the difference is +17 fld [mem], +13 fstp [mem] (FPU temporaries the original spills to stack slots: t at [0x14], the t*dx/t*dy/t*dz products at [0x80]/[0x88], hx/hy at [0x3c]/[0x40], py at [0x74], where ours keeps them on the FPU stack), +4 fcompp (compare of two stack values instead of fcom mem), +6 and/jne instead of test/jp pairs; no call, branch or statement differs) */

char d3_ScreenToUV(float sx, float sy, D3OBJECT *obj, void *tex, float *outu, float *outv)
{
    D3CORNERCTX c;
    float mn[2];
    float best_a, best_b, best_c, f;
    float px, py, pz, ax, ay, az, bx, by, bz, nx, ny, nz, dx, dy, dz, hz, den, t, hx, hy, s, m, r;
    int i, j, ia, ib, ic, id;

    if (tex) {
        c.tex = tex;
        c.pt[0].x = dd_Width - 1;
        c.pt[0].y = dd_Height - 1;
        c.found = 0;
        c.pt[1].x = 0;
        c.pt[1].y = dd_Height - 1;
        c.pt[2].x = dd_Width - 1;
        c.pt[2].y = 0;
        c.pt[3].x = 0;
        c.pt[3].y = 0;
        d3_ForEachObject(d3_DefaultScene, obj, 0x40, (D3OBJECTCB)d3_CornerCB, &c);
        if (c.found) {
            mn[0] = 1e10f;
            mn[1] = 1e10f;
            for (i = 0; i < 4; i++) {
                if (c.uv[i][0] <= mn[0])
                    mn[0] = c.uv[i][0];
                if (c.uv[i][1] <= mn[1])
                    mn[1] = c.uv[i][1];
            }
            for (i = 0; i < 4; i++) {
                for (j = 0; j < 2; j++)
                    c.uv[i][j] = c.uv[i][j] - mn[j];
                c.uv[i][1] = 1.0f - c.uv[i][1];
            }
            ia = -1;
            ib = -1;
            ic = -1;
            best_c = 1e10f;
            best_b = -1e10f;
            best_a = -1e10f;
            for (i = 0; i < 4; i++) {
                f = (float)fabs(c.uv[i][1]) - (float)fabs(c.uv[i][0]);
                if (best_a < f) {
                    ia = i;
                    best_a = f;
                }
                f = (float)fabs(c.uv[i][0]) + (float)fabs(c.uv[i][1]);
                if (best_b < f) {
                    ib = i;
                    best_b = f;
                }
                if (f < best_c) {
                    ic = i;
                    best_c = f;
                }
            }
            if (ia != -1 && ib != -1 && ic != -1 && ia != ib && ia != ic && ib != ic) {
                for (id = 0; id == ib || id == ia || id == ic; id++)
                    ;
                px = c.v[ia].x;
                py = c.v[ia].y;
                pz = c.v[ia].z;
                ax = px - c.v[ib].x;
                ay = py - c.v[ib].y;
                az = pz - c.v[ib].z;
                bx = px - c.v[ic].x;
                by = py - c.v[ic].y;
                bz = pz - c.v[ic].z;
                nx = bz * ay - by * az;
                ny = bx * az - bz * ax;
                nz = by * ax - bx * ay;
                dx = sx - d3_CenterX;
                dy = -(sy - d3_CenterY);
                dz = d3_ScaleX;
                den = dx * nx + dy * ny + dz * nz;
                if (1e-07 < fabs(den)) {
                    t = ((nz + ny + nx) * 0.0f + nx * px + ny * py + nz * pz) / den;
                    hx = px - t * dx;
                    hy = py - t * dy;
                    hz = pz - t * dz;
                    s = (hx * ay - hy * ax) / (bx * ay - by * ax);
                    m = -s;
                    hx = bx * m + hx;
                    hy = by * m + hy;
                    hz = m * bz + hz;
                    r = (hx / ax + hy / ay + hz / az) * 0.33333334f;
                    if (r + s > 1.0) {
                        px = c.v[id].x;
                        py = c.v[id].y;
                        pz = c.v[id].z;
                        ax = px - c.v[ib].x;
                        ay = py - c.v[ib].y;
                        az = pz - c.v[ib].z;
                        bx = px - c.v[ic].x;
                        by = py - c.v[ic].y;
                        bz = pz - c.v[ic].z;
                        nx = az * by - ay * bz;
                        ny = ax * bz - az * bx;
                        nz = ay * bx - ax * by;
                        den = dx * nx + dy * ny + dz * nz;
                        if (1e-07 < fabs(den)) {
                            t = ((nz + ny + nx) * 0.0f + nx * px + ny * py + nz * pz) / den;
                            hx = px - t * dx;
                            hy = py - t * dy;
                            hz = pz - t * dz;
                            s = (hx * by - hy * bx) / (ax * by - ay * bx);
                            m = -s;
                            hx = ax * m + hx;
                            hy = ay * m + hy;
                            hz = m * az + hz;
                            r = (hx / bx + hy / by + hz / bz) * 0.33333334f;
                            if (r >= 0.0f && r <= 1.0f && s >= 0.0f && s <= 1.0f) {
                                *outu = (1.0f - r) * ((float)fabs(c.uv[ib][0]) - (float)fabs(c.uv[ia][0])) + (float)fabs(c.uv[ia][0]);
                                *outv = 1.0 - ((1.0f - s) * ((float)fabs(c.uv[ic][1]) - (float)fabs(c.uv[ia][1])) + (float)fabs(c.uv[ia][1]));
                                return 1;
                            }
                        }
                    } else {
                        if (r >= 0.0f && r <= 1.0f && s >= 0.0f && s <= 1.0f) {
                            *outu = ((float)fabs(c.uv[ib][0]) - (float)fabs(c.uv[ia][0])) * r + (float)fabs(c.uv[ia][0]);
                            *outv = 1.0 - (((float)fabs(c.uv[ic][1]) - (float)fabs(c.uv[ia][1])) * s + (float)fabs(c.uv[ia][1]));
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}
/* 0x4495f0, callback of d3_FindObject: sets ctx->found to the first object that matches */

static char d3_FindObjectCB(D3OBJECT *obj, D3FINDCTX *ctx)
{
    char *s = (char *)obj;
    char *t;
    if (ctx->name) {
        t = (*s == '!') ? s + 1 : s;
        if (ctx->obj) {
            if (!strcmp(t, ctx->name) && ctx->obj == s)
                ctx->found = s;
        } else if (!strcmp(t, ctx->name))
            ctx->found = s;
    } else if (ctx->obj == s)
        ctx->found = s;
    return ctx->found != s;
}
/* 0x449690, finds an object by name (and/or address `c`), case-sensitive, among the objects of the default scene
   selected by `flags`; the siblings after `a` are cut off during the search */
D3OBJECT *d3_FindObject(int a, int flags, int c, char *name)
{
    D3FINDCTX ctx;
    D3OBJECT *next;

    if (!c && !name)
        return 0;
    ctx.obj = (char *)c;
    ctx.name = name;
    ctx.found = 0;
    if (a) {
        next = ((D3OBJECT *)a)->next;
        ((D3OBJECT *)a)->next = 0;
    }
    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)a, flags | 0x200, (D3OBJECTCB)d3_FindObjectCB, &ctx);
    if (a)
        ((D3OBJECT *)a)->next = next;
    return (D3OBJECT *)ctx.found;
}
/* 0x449710, like d3_FindObjectCB but case-insensitive */

static char d3_FindObjectNoCaseCB(D3OBJECT *obj, D3FINDCTX *ctx)
{
    char *s = (char *)obj;
    char *t;
    if (ctx->name) {
        t = (*s == '!') ? s + 1 : s;
        if (ctx->obj) {
            if (!_strcmpi(t, ctx->name) && ctx->obj == s)
                ctx->found = s;
        } else if (!_strcmpi(t, ctx->name))
            ctx->found = s;
    } else if (ctx->obj == s)
        ctx->found = s;
    return ctx->found != s;
}
/* 0x449770, d3_FindObject with a case-insensitive name compare */

D3OBJECT *d3_FindObjectNoCase(int a, int flags, int c, char *name)
{
    D3FINDCTX ctx;
    D3OBJECT *next;

    if (!c && !name)
        return 0;
    ctx.obj = (char *)c;
    ctx.name = name;
    ctx.found = 0;
    if (a) {
        next = ((D3OBJECT *)a)->next;
        ((D3OBJECT *)a)->next = 0;
    }
    d3_ForEachObject(d3_DefaultScene, (D3OBJECT *)a, flags | 0x200, (D3OBJECTCB)d3_FindObjectNoCaseCB, &ctx);
    if (a)
        ((D3OBJECT *)a)->next = next;
    return (D3OBJECT *)ctx.found;
}
/* 0x4497f0, converts a world-space position and rotation into the coordinate system of the object's parent
   (the parent's inverse world matrix applied to `pos`, `rot` multiplied by the transposed parent matrix); an
   object without parent copies the position and takes the angles of `rot` */

void d3_WorldToParent(D3OBJECT *obj, float *pos, D3MATRIX *rot, D3VECTOR *outpos, D3VECTOR *outrot)
{
    D3OBJECT *parent;
    D3MATRIX m, w;

    parent = obj->parent;
    if (!parent) {
        outpos->x = pos[0];
        outpos->y = pos[1];
        outpos->z = pos[2];
        d3_MatToAngles((D3VECTOR4 *)rot, (D3VECTOR4 *)outrot);
        return;
    }
    d3_WorldMatrixD(parent, 0, 0, &m);
    d3_MatInverse(&m, &w);
    outpos->x = w._31 * pos[2] + w._11 * pos[0] + w._21 * pos[1] + w._41;
    outpos->y = w._32 * pos[2] + w._12 * pos[0] + w._22 * pos[1] + w._42;
    outpos->z = w._33 * pos[2] + w._13 * pos[0] + w._23 * pos[1] + w._43;
    d3_WorldMatrixD(parent, 0, 1, &m);
    d3_MatTranspose(&m, &w);
    d3_MatMul43(rot, &w, &m);
    d3_MatToAngles((D3VECTOR4 *)&m, (D3VECTOR4 *)outrot);
}
/* 0x449920, sets an object's local position and rotation from a world-space position and rotation (converted
   into the space of the object's parent) */

void d3_SetWorldPosRot(D3OBJECT *obj, float *pos, D3MATRIX *rot)
{
    D3VECTOR4 v;
    D3VECTOR a;

    d3_WorldToParent(obj, pos, rot, (D3VECTOR *)&v, &a);
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
    d3_SetObjectRot(obj, &a);
}

void err_Message(char *msg);                                                                     /* 0x4222b0 */
/* @original: d3_SetFogSet passes the fog colour as the struct (the original pushes it before the second fmul; the
   dword of a D3COLORU pushes it after), d3_engine.c / the header take the dword */
#define d3_SetFogS ((void (*)(float, float, D3COLOR))d3_SetFog)  /* 0x43a6f0 */

/* 0x449960, makes obj a child of parent (0: top level) keeping its world position; light keys too */
char d3_SetParent(D3OBJECT *obj, D3OBJECT *parent)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3MATRIX m;
    D3VECTOR4 kpos[7];
    D3MATRIX km[7];
    D3OBJECT *c;
    int i;

    if (obj == parent || !obj)
        return 0;
    if (parent) {
        for (c = parent->child; c; c = c->next)
            if (c == obj)
                return 1;
        if (d3_FindObject((int)obj, 0x1ff, (int)parent, 0))
            return 0;
    }
    if (D3_X(obj)->type >= 5 && D3_X(obj)->light) {
        rot.x = D3_X(obj)->rot.x;
        rot.y = D3_X(obj)->rot.y;
        rot.z = D3_X(obj)->rot.z;
        for (i = 0; i < 7; i++) {
            d3_PointToWorld(obj, &D3O_LIGHT(obj)->k[i].pos, (D3VECTOR *)&kpos[i]);
            d3_SetObjectRot(obj, &D3O_LIGHT(obj)->k[i].rot);
            d3_WorldMatrixD(obj, 0, 1, &km[i]);
        }
        d3_SetObjectRot(obj, &rot);
    }
    d3_PointToWorld(obj, &D3_X(obj)->pos, (D3VECTOR *)&pos);
    d3_WorldMatrixD(obj, 0, 1, &m);
    d3_Unlink(obj);
    if (!parent)
        d3_LinkTop(obj);
    else
        d3_AddChild(parent, obj);
    d3_SetWorldPosRot(obj, &pos, &m);
    if (D3_X(obj)->type >= 5 && D3_X(obj)->light) {
        for (i = 0; i < 7; i++)
            d3_WorldToParent(obj, &kpos[i], &km[i], &D3O_LIGHT(obj)->k[i].pos, &D3O_LIGHT(obj)->k[i].rot);
    }
    return 1;
}
/* 0x449b20, sets a light to its day-time keys: key (0..6) blended towards the next key by t (0..1),
   shifted by the light's phase; the intensity is only blended between two lit keys (type 8: always) */

char d3_SetLightKey(D3OBJECT *obj, unsigned int key, float t)
{
    float pos[4], rot[4];
    D3LIGHTKEYS *l;
    D3LIGHTKEY *c, *nk;
    unsigned int i, n, p;
    int pon, con, non;

    if (obj && key < 7 && t >= 0.0f && t <= 1.0f && D3_X(obj)->type >= 5 && (l = D3O_LIGHT(obj))) {
        t += l->phase;
        if (t > 1.0f) {
            t -= 1.0f;
            key = (key + 1) % 7;
        } else if (t < -1.0f) {
            t += 1.0f;
            key = (key - 1) % 7;
        }
        n = (key + 1) % 7;
        p = (key - 1) % 7;
        pon = l->k[p].intensity != 0.0f || D3_X(obj)->type == 8;
        c = &l->k[key];
        con = c->intensity != 0.0f || D3_X(obj)->type == 8;
        nk = &l->k[n];
        non = nk->intensity != 0.0f || D3_X(obj)->type == 8;
        if (pon && con && non)
            D3IF_LIGHT(obj)->power = (nk->intensity - c->intensity) * t + c->intensity;
        else if (con && t < 0.5f)
            D3IF_LIGHT(obj)->power = c->intensity;
        else if (non && t >= 0.5f)
            D3IF_LIGHT(obj)->power = nk->intensity;
        else
            D3IF_LIGHT(obj)->power = 0.0f;
        D3IF_LIGHT(obj)->range = (nk->range - c->range) * t + c->range;
        D3IF_LIGHT(obj)->atten = (nk->atten - c->atten) * t + c->atten;
        for (i = 0; i < 3; i++) {
            pos[i] = (((float *)&l->k[n].pos)[i] - ((float *)&l->k[key].pos)[i]) * t + ((float *)&l->k[key].pos)[i];
            ((float *)&D3IF_LIGHT(obj)->color)[i] = (((float *)&l->k[n].color)[i] - ((float *)&l->k[key].color)[i]) * t + ((float *)&l->k[key].color)[i];
            rot[i] = (((float *)&l->k[n].rot)[i] - ((float *)&l->k[key].rot)[i]) * t + ((float *)&l->k[key].rot)[i];
        }
        d3_SetObjectPos(obj, (D3VECTOR *)pos);
        d3_SetObjectRot(obj, (D3VECTOR *)rot);
        return 1;
    }
    return 0;
}
/* 0x449d80, d3_ForEachObject callback: scales the intensity of a light */

char d3_ScaleLightCB(D3OBJECT *obj, float *scale)
{
    D3IF_LIGHT(obj)->power *= *scale;
    return 1;
}
/* 0x449da0, d3_ForEachObject callback: sets a light to the day time *time (key + fraction) */

char d3_SetLightKeyCB(D3OBJECT *obj, float *time)
{
    float t = *time;
    unsigned int key = (unsigned int)t;

    d3_SetLightKey(obj, key, t - key);
    return 1;
}
/* 0x449de0, sets the day-time light set `set` blended towards the next one by t (0..1): the lights,
   the ambient colour (times scale), the sun colour, the fog sets and the sky colour */

char d3_SetLightSet(unsigned int set, float t, float scale, char calc)
{
    float time, u;
    D3VECTOR4 d;
    unsigned int n;
    int j;

    u = 0;
    if (set < 7 && t >= 0.0f && t <= 1.0f) {
        time = set + t;
        d3_ForEachObject(d3_DefaultScene, 0, 0x1e, (D3OBJECTCB)d3_SetLightKeyCB, &time);
        if (scale != 1.0f)
            d3_ForEachObject(d3_DefaultScene, 0, 8, (D3OBJECTCB)d3_ScaleLightCB, &scale);
        n = (set + 1) % 7;
        d.x = d3if_Levels[n].amb.x - d3if_Levels[set].amb.x;
        d.y = d3if_Levels[n].amb.y - d3if_Levels[set].amb.y;
        d.z = d3if_Levels[n].amb.z - d3if_Levels[set].amb.z;
        d3_AmbientColor.x = d.x * t + d3if_Levels[set].amb.x;
        d3_AmbientColor.y = d.y * t + d3if_Levels[set].amb.y;
        d3_AmbientColor.z = d.z * t + d3if_Levels[set].amb.z;
        d3_AmbientColor.x *= scale;
        d3_AmbientColor.y *= scale;
        d3_AmbientColor.z *= scale;
        d3if_Gray = d3_AmbientColor.x * 0.3f + d3_AmbientColor.y * 0.59f + d3_AmbientColor.z * 0.11f;
        d.x = d3if_Levels[n].sun.x - d3if_Levels[set].sun.x;
        d.y = d3if_Levels[n].sun.y - d3if_Levels[set].sun.y;
        d.z = d3if_Levels[n].sun.z - d3if_Levels[set].sun.z;
        d3_SunColor.x = d.x * t + d3if_Levels[set].sun.x;
        d3_SunColor.y = d.y * t + d3if_Levels[set].sun.y;
        d3_SunColor.z = d.z * t + d3if_Levels[set].sun.z;
        u = 1.0f - t;
        for (j = 0; j < 6; j++) {
            D3FOGSET *a = &d3if_Levels[set].fog[j], *b = &d3if_Levels[n].fog[j];
            d3if_Fog[j].col.c.r = (int)(a->col.c.r * u + b->col.c.r * t);
            d3if_Fog[j].col.c.g = (int)(a->col.c.g * u + b->col.c.g * t);
            d3if_Fog[j].col.c.b = (int)(a->col.c.b * u + b->col.c.b * t);
            d3if_Fog[j].start = u * a->start + t * b->start;
            d3if_Fog[j].end = u * a->end + t * b->end;
        }
        d3_LightInvalidate(calc);
        d3o_LightSet = set;
        if (d3_Sky) {
            ((unsigned char *)d3_Sky)[0xaa6] = (int)(d3sky_Colors[n].c.r * t + d3sky_Colors[set].c.r * u);
            ((unsigned char *)d3_Sky)[0xaa5] = (int)(d3sky_Colors[n].c.g * t + d3sky_Colors[set].c.g * u);
            ((unsigned char *)d3_Sky)[0xaa4] = (int)(d3sky_Colors[n].c.b * t + d3sky_Colors[set].c.b * u);
        }
        return 1;
    }
    return 0;
}
/* 0x44a170, sets the fog between fog sets a and b (t = 0..1, 0 = a) */

char d3_SetFogSet(unsigned int a, unsigned int b, float t)
{
    D3COLOR col;

    if (a < 6 && b < 6 && t >= 0.0f && t <= 1.0f) {
        col.r = (int)((d3if_Fog[b].col.c.r - d3if_Fog[a].col.c.r) * t + d3if_Fog[a].col.c.r);
        col.g = (int)((d3if_Fog[b].col.c.g - d3if_Fog[a].col.c.g) * t + d3if_Fog[a].col.c.g);
        col.b = (int)((d3if_Fog[b].col.c.b - d3if_Fog[a].col.c.b) * t + d3if_Fog[a].col.c.b);
        d3_SetFogS(((d3if_Fog[b].start - d3if_Fog[a].start) * t + d3if_Fog[a].start) * d3o_FogScale,
                  ((d3if_Fog[b].end - d3if_Fog[a].end) * t + d3if_Fog[a].end) * d3o_FogScale,
                  col);
        d3o_FogSet = a;
        return 1;
    }
    return 0;
}
/* 0x44a2c0, writes obj (if it is saved: objectFlags bit 1), then its first saved child and its next saved
   sibling, recursively ("wr_object")
   @deep Q2/Q4: read the function against its mirror reader rd_object (d3_int_io.c 0x44abb0) and the other users of every field. Q2 fields with no evidenced meaning anywhere: D3OBJX.unk09c (10 x 6 ints, only zeroed in d3_InitObject, D3E_COPY3'd in d3_CloneObject and serialized for types 2/3, no reader of the values), D3OBJX.unk214 (serialized for mesh types, only init 0 and shadow_fake = 3), D3LIGHTKEYS.unk19c / unk1a0 and D3LIGHTKEY.unk30 / unk34 (written/read by the scene file only, no other use); unk05c/unk090 do have views (D3IFLIGHT, light colour / range / power / atten), the rest do not. Q4: 9 x *(int *)&float serialization casts (radius, the three light params, three key params, pad200) - a typed union view has not been tried; both need match-deep */

void d3_WriteObject(VFSFILE *f, D3OBJECT *obj)
{
    char buf[256];
    char *p;
    D3OBJECT *c;
    int type, i;

    bio_wr_c(f, obj->objectFlags >> 1 & 1);
    if (obj->objectFlags & 2) {
        type = D3_X(obj)->type;
        if (type == 1 && (type = D3_X(obj)->type0, obj->name[0] == '!'))
            bio_wr_s(f, obj->name + 1);
        else
            bio_wr_s(f, obj->name);
        bio_wr_i(f, *(int *)&obj->owner);
        bio_wr_i(f, obj->kind);
        bio_wr_i(f, type);
        bio_wr_c(f, D3_X(obj)->type == 1);
        switch (type) {
        case 1:
        case 4:
            bio_wr_c(f, obj->state >> 7);
            bio_wr_c(f, obj->state >> 5 & 1);
            bio_wr_c(f, obj->objectFlags >> 2 & 1);
            bio_wr_c(f, obj->stateFlags >> 2 & 3);
            bio_wr_c(f, obj->stateFlags >> 4 & 1);
            bio_wr_c(f, obj->stateFlags >> 5 & 1);
            bio_wr_c(f, obj->stateFlags >> 6 & 1);
            bio_wr_i(f, D3_X(obj)->meshMode);
            if (D3_X(obj)->mesh) {
                bio_wr_i(f, ((unsigned char *)D3_X(obj)->mesh)[0x96c]);
                if (((unsigned char *)D3_X(obj)->mesh)[0x96c] > 0) {
                    strcpy(buf, *(char **)((char *)D3_X(obj)->mesh + 0x104));
                    p = strrchr(buf, '_');
                    if (p && strlen(p) == 2 && isdigit(p[1]))
                        *p = 0;
                    bio_wr_s(f, buf);
                }
            } else
                bio_wr_i(f, 0);
        case 2:
        case 3:
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->rot);
            bio_wr_c(f, obj->objectFlags & 1);
            if (obj->objectFlags & 1) {
                bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
                bio_wr_v3(f, (int *)&D3_X(obj)->unk090);
            }
            for (i = 0; i < 10; i++) {
                bio_wr_v3(f, (int *)&D3_X(obj)->views[i].pos);
                bio_wr_v3(f, (int *)&D3_X(obj)->views[i].dir);
            }
            break;
        case 0:
            bio_wr_c(f, obj->objectFlags & 1);
            bio_wr_i(f, *(int *)&D3_X(obj)->radius);
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk090);
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            bio_wr_c(f, obj->state >> 5 & 1);
            bio_wr_c(f, obj->objectFlags >> 4 & 1);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.x);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.y);
            bio_wr_i(f, *(int *)&D3_X(obj)->unk090.z);
            bio_wr_v3(f, (int *)&D3_X(obj)->unk05c);
            bio_wr_v3(f, (int *)&D3_X(obj)->pos);
            bio_wr_v3(f, (int *)&D3_X(obj)->rot);
            bio_wr_i(f, D3O_LIGHT(obj)->unk19c);
            bio_wr_i(f, D3O_LIGHT(obj)->unk1a0);
            for (i = 0; i < 7; i++) {
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].range);
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].intensity);
                bio_wr_i(f, *(int *)&D3O_LIGHT(obj)->k[i].atten);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].color);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].pos);
                bio_wr_v3(f, (int *)&D3O_LIGHT(obj)->k[i].rot);
                bio_wr_i(f, D3O_LIGHT(obj)->k[i].unk30);
                bio_wr_i(f, D3O_LIGHT(obj)->k[i].unk34);
            }
            break;
        default:
            err_Message("Unknown Object-Type in wr_object()!");
        }
    }
    for (c = obj->child; c; c = c->next)
        if (c->objectFlags & 2)
            break;
    bio_wr_c(f, c != 0);
    if (c)
        d3_WriteObject(f, c);
    for (c = obj->next; c; c = c->next)
        if ((c->state & 1) != 1 && (c->objectFlags & 2))
            break;
    bio_wr_c(f, c != 0);
    if (c)
        d3_WriteObject(f, c);
    d3e_SaveEvents(f, (D3EVENT *)D3_X(obj)->events);
}
/* 0x44a770, writes the terrain map of the scene */

void d3_WriteMap(VFSFILE *f, D3FLMAP *map)
{
    int i;

    bio_wr_s(f, map->name);
    if (map->name[0]) {
        bio_wr_i(f, map->size);
        bio_wr_array(f, map->size, map->size, map->height);
        bio_wr_array(f, map->size << 2, map->size, map->lightofs);
        bio_wr_c(f, map->water != 0);
        if (map->water) {
            bio_wr_i(f, map->nregions);
            if (map->nregions > 0)
                bio_wr_array(f, map->size, map->size, map->water);
            for (i = 0; i < map->nregions; i++) {
                if (map->regions[i].tex) {
                    bio_wr_s(f, map->regions[i].tex->name);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, ((unsigned char *)map->regions[i].tex)[0x72] & 0xf);
                    bio_wr_c(f, map->regions[i].tex->state & 1);
                } else {
                    bio_wr_s(f, ch_EmptyName);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, 0);
                    bio_wr_c(f, 0);
                }
                bio_wr_c(f, map->regions[i].flags.b[0]);
                bio_wr_c(f, map->regions[i].flags.b[1]);
                bio_wr_c(f, map->regions[i].flags.b[2] & 1);
                bio_wr_c(f, map->regions[i].flags.b[2] >> 1 & 1);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].f14.bits);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].scrollSpeedU.bits);
                bio_wr_i(f, ((D3IFREGION *)map->regions)[i].scrollSpeedV.bits);
                bio_wr_v4(f, (int *)map->regions[i].speed);
                bio_wr_v4(f, (int *)map->regions[i].amp);
                bio_wr_i(f, map->regions[i].water);
            }
        }
    } else
        bio_wr_c(f, 0);
    bio_wr_s(f, map->texname);
    if (map->texname[0])
        bio_wr_array(f, map->size, map->size, map->types);
    for (i = 0; i < 8; i++)
        bio_wr_s(f, map->typenames[i]);
    bio_wr_i(f, ((D3IFMAP *)map)->scale.bits);
    bio_wr_i(f, ((D3IFMAP *)map)->hscale.bits);
    bio_wr_v3(f, (int *)&map->orgx);
}
/* 0x44a9e0, maps a scene file version (d3_SaveScene writes 0x3a6c00bb) to a format level 0 / 4 / 6 / 7;
   presumably used by the scene loader after this range */

int d3_SceneVersion(unsigned int version)
{
    if (version >= 0x3a6c00ba)
        return 7;
    if (version >= 0x3a6c00a5)
        return 6;
    return version < 0x3a6c00a1 ? 0 : 4;
}
/* 0x44aa10, writes the scene (into f, or into the new file name) */

void d3_SaveScene(char *name, VFSFILE *f)
{
    VFSFILE *out;
    D3OBJECT *o;
    int i, k, n;

    out = f;
    if (f || (out = vfs_Tfopen(name, "wb", (int)vfs_Root))) {
        bio_wr_i(out, 0x3a6c00bb);
        bio_wr_s(out, (char *)d3if_MegaCam);
        bio_wr_i(out, ((D3IFBITS *)&d3if_Gray)->bits);
        bio_wr_v3(out, (int *)&d3_AmbientColor);
        bio_wr_v3(out, (int *)&d3_SunColor);
        bio_wr_i(out, ((D3IFSCENE *)d3_DefaultScene)->mapSize);
        bio_wr_i(out, d3_fog_color.col);
        bio_wr_i(out, ((D3IFBITS *)&d3_FogStart1)->bits);
        bio_wr_i(out, ((D3IFBITS *)&d3_FogEnd2)->bits);
        for (k = 0; k < 7; k++) {
            bio_wr_v3(out, (int *)&d3if_Levels[k].amb);
            bio_wr_v3(out, (int *)&d3if_Levels[k].sun);
            for (i = 0; i < 6; i++) {
                bio_wr_i(out, d3if_Levels[k].fog[i].col.col);
                bio_wr_i(out, ((D3IFBITS *)&d3if_Levels[k].fog[i].start)->bits);
                bio_wr_i(out, ((D3IFBITS *)&d3if_Levels[k].fog[i].end)->bits);
            }
        }
        n = 0;
        for (o = d3_TopFirst; o != &d3_ObjEnd; o = d3_NextGroup(o))
            n++;
        bio_wr_i(out, n);
        for (o = d3_TopFirst; o != &d3_ObjEnd; o = d3_NextGroup(o))
            d3_WriteObject(out, o);
        bio_wr_c(out, d3_Terrain != 0);
        if (d3_Terrain)
            d3_WriteMap(out, d3_Terrain);
        d3e_SaveEvents(out, ((D3IFSCENE *)d3_DefaultScene)->events);
        if (!f)
            vfs_fclose(out);
    }
}

extern char d3if_ParList[0x31c];
/* d3if_SceneState0: view of d3_DefaultD3DConfig (tools/data_views.py) */
/* d3if_SceneState1 is defined in d3_engine.c, right after that block (0x5fb5c4, the same unit) */
int d3o_LightSet = -1;  /* 0x5fbcfc, the current light set */
int d3o_FogSet = -1;  /* 0x5fbd00, the current fog set */
float d3if_Gray = 200.0f;  /* 0x5fcc74, 0.3 r + 0.59 g + 0.11 b of the ambient colour */
D3VECTOR d3_AmbientColor = {200.0f, 200.0f, 200.0f};  /* 0x5fcc78, the ambient colour (r, g, b) */
int d3if_CamOben = 0;  /* 0x6338b8 */
int d3if_CamVorne = 0;  /* 0x6338bc */
int d3if_CamSeite = 0;  /* 0x6338c0 */
unsigned int d3if_SceneSwitches = 0;  /* 0x6339dc, counts the scene switches (an object of its own: 4-aligned after the byte d3if_TexturesEvicted) */
int d3s_ShadowMapSize = 0;  /* 0x633dcc */
char d3if_ParList[0x31c] = {0};  /* 0xfb95e0, the default scene's particle list head node (a D3PARTSYS, 0x31c bytes: d3_ParHead is its next field, +0x318 its prev) */
int d3if_LastMegaCam = 0;  /* 0xfba944 (own object: d3io_StockDir is a 4-byte pointer) */
D3FOGSET d3if_Fog[6] = {0}; /* 0xfba960 */
D3LIGHTSET d3if_Levels[7] = {0}; /* 0xfba9c0 */
