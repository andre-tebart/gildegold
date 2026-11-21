/* d3_event.c -- the 3D engine's object / scene events (0x43e6b0..0x43eab0): the default resolver, the event names
   d3e_Events and their lookups, d3e_RegisterEvent / d3e_RegisterSceneEvent ("d3e:RegisterEvent(eventtable)") and
   the event tables' save / load. Its .data block is d3e_Events 0x5fb800 and d3e_Resolve 0x5fb920 right after
   d3_engine's last strings ("_S" / "_1" / "_0", 0x43df60's): a plain datum after literals, a new object; then
   its strings "NONE" 0x5fb924 (0x43e710's), "d3e:Register..." (0x43e740 / 0x43e820). The original's "d3e:"
   tag names the module; the file name is ours. The first function is 0x43e6b0 (the resolver d3e_Resolve
   starts out as) or an earlier one down to 0x43e090 (no strings in between).
   Open: the first function (bprobe: no function changes at 0x43e090 .. 0x43e6c0).
   Open: head: the includes are d3_engine.c's; d3e_LoadEvents' prototype is still in d3_int_io.h.
   split from d3_engine (rea 8cef7d5b, src/game/d3_engine.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
/* d3_fl.c defines these with char / unsigned int parameters; the 0x43c4e0 caller pushes ints. */
#define d3fl_SetMips d3fl_SetMips
#define d3fl_SetDetail d3fl_SetDetail
/* d3.h declares d3_ForEachObject as void; it returns a char. */
#define d3_ForEachObject d3_ForEachObject_void
/* and d3_UpdateObject's second parameter as int; it is a byte. */
#define d3_UpdateObject d3_UpdateObject_int
/* and d3_UpdateWorldMatrix's third parameter as int; it is a char. */
#define d3_UpdateWorldMatrix d3_UpdateWorldMatrix_int
#include "d3.h"
#undef d3_ForEachObject
#undef d3_UpdateObject
#undef d3_UpdateWorldMatrix
#include "d3_sky.h"
#include "d3_vec.h"
#include "d3_engine.h"
#include "d3_event.h"
#include "d3_anim.h"
/* d3_mir.h types d3_MirDrawObject / d3_MirBeginFrame with D3MIRROR *; d3_engine.h keeps the D3OBJECT * views. */
#define d3_MirDrawObject d3_Unk4550e0_mirview
#define d3_MirBeginFrame d3_Unk4554b0_mirview
#include "d3_mir.h"
#undef d3_MirDrawObject
#undef d3_MirBeginFrame
#include "d3_light.h"
#include "data_views.h"
#include "d3_d3d.h"
#include "d3_fl.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_a.h"
#include "d3_io_t.h"
#include "d3_par.h"
#include "d3_sm.h"
#include "d3_snd.h"
#include "d3_tools.h"
#include "d3s.h"
#include "dd_vesa.h"
#include "f3gm.h"
#include "f3init.h"
#include "mem.h"
#include "tb.h"
#include "ts_texture.h"
#include "d3_pose.h"
#include "d3_vtx.h"
#include "poker.h"
#include "ch.h"
#include "bio.h"
#define dd_ClearB ((void (*)(D3RECT *, unsigned char, unsigned char, unsigned char))dd_Clear)  /* 0x41e100 (dd_vesa.h: RECT *, int x3): @original this unit passes the fog colour as bytes, its D3RECT viewport */
#define d3_MirBeginFrameE ((void (*)(D3OBJECT *, D3OBJECT **))d3_MirBeginFrame)  /* 0x4554b0 (d3_mir.h: D3MIRROR *): this unit passes the mirror as its D3OBJECT ** view */
/* The stock list head lives inside its 0xfc2e00 head sentinel (the original's +0x1fc relationship). As a
   separate COMMON pointer the linker would not keep it adjacent, so the loader would link the first object
   into one address while the readers start from another. */
extern D3INITCFG d3_Cfg;
extern D3PARTSYS d3_ParEnd;
void d3fl_FreeMap(struct D3FLMAP *map);     /* 0x43ff90, d3_fl.c (d3.h types d3_Terrain as void *) */

/* 0x43e6b0, default for d3e_Resolve: the handler of an event, by name */

D3EVHANDLER d3e_DefaultResolve(void *obj, int id, char *name)
{
    return (D3EVHANDLER)ai_AlwaysTrue;
}

/* 0x5fb800 */
D3EVNAME d3e_Events[] = {
    { "NONE", 0 },
    { "TEST", 1 },
    { "ZOOM_IN_OBJECT", 2 },
    { "ZOOM_OUT_OBJECT", 3 },
    { "TEST", 1 },
    { "SCENE_ENTER", 4 },
    { "SCENE_EXIT", 5 },
    { "SCENE_SUPERVISOR", 6 },
};

/* 0x43e6c0, the id of an event name ("SCENE_ENTER", case-insensitive), 0 when the name is unknown */
int d3e_EventId(char *name)
{
    unsigned int i;

    for (i = 0; i < sizeof(d3e_Events) / sizeof(d3e_Events[0]); i++)
        if (!strcmpi(name, d3e_Events[i].name))
            return d3e_Events[i].id;
    return 0;
}
/* 0x43e710, the name of an event id, "NONE" when the id is unknown */

char *d3e_EventName(int id)
{
    unsigned int i;

    for (i = 0; i < sizeof(d3e_Events) / sizeof(d3e_Events[0]); i++)
        if (id == d3e_Events[i].id)
            return d3e_Events[i].name;
    return "NONE";
}

D3EVRESOLVE d3e_Resolve = d3e_DefaultResolve;  /* 0x5fb920 */

/* 0x43e740, sets (fn != 0) or clears an event handler of an object */
char d3e_RegisterEvent(D3OBJECT *obj, int id, D3EVHANDLER fn, char *name)
{
    int i;

    if (!obj)
        return 0;
    if (id) {
        if (fn) {
            if (!D3_X(obj)->events)
                D3_X(obj)->events = mem_Alloc(D3E_NEVENTS * sizeof(D3EVENT), "d3e:RegisterEvent(eventtable)");
            ((D3EVENT *)D3_X(obj)->events)[id].name[0x7f] = 0;
            strncpy(((D3EVENT *)D3_X(obj)->events)[id].name, name, 0x7f);
            ((D3EVENT *)D3_X(obj)->events)[id].fn = fn;
            return 1;
        }
        if (D3_X(obj)->events && ((D3EVENT *)D3_X(obj)->events)[id].fn) {
            ((D3EVENT *)D3_X(obj)->events)[id].fn = 0;
            for (i = 0; i < D3E_NEVENTS; i++)
                if (((D3EVENT *)D3_X(obj)->events)[i].fn)
                    break;
            if (i == D3E_NEVENTS) {
                mem_Free(D3_X(obj)->events);
                D3_X(obj)->events = 0;
            }
        }
    }
    return 1;
}
/* 0x43e820, the same for a scene */

char d3e_RegisterSceneEvent(D3SCENE *scene, int id, D3EVHANDLER fn, char *name)
{
    int i;

    if (!scene)
        return 0;
    if (id) {
        if (fn) {
            if (!scene->events)
                scene->events = mem_Alloc(D3E_NEVENTS * sizeof(D3EVENT), "d3e:RegisterSceneEvent(eventtable)");
            ((D3EVENT *)scene->events)[id].name[0x7f] = 0;
            strncpy(((D3EVENT *)scene->events)[id].name, name, 0x7f);
            ((D3EVENT *)scene->events)[id].fn = fn;
            return 1;
        }
        if (scene->events && ((D3EVENT *)scene->events)[id].fn) {
            ((D3EVENT *)scene->events)[id].fn = 0;
            for (i = 0; i < D3E_NEVENTS; i++)
                if (((D3EVENT *)scene->events)[i].fn)
                    break;
            if (i == D3E_NEVENTS) {
                mem_Free(scene->events);
                scene->events = 0;
            }
        }
    }
    return 1;
}
/* 0x43e900, writes an event table */

void d3e_SaveEvents(void *f, D3EVENT *t)
{
    int i, n;

    if (t) {
        n = 0;
        for (i = 0; i < D3E_NEVENTS; i++)
            if (t[i].fn)
                n++;
        bio_wr_i(f, n);
        for (i = 0; i < D3E_NEVENTS; i++, t++) {
            if (t->fn) {
                bio_wr_s(f, d3e_EventName(i));
                bio_wr_s(f, t->name);
            }
        }
    } else
        bio_wr_i(f, 0);
}
/* 0x43e970, reads the event table of an object */

void d3e_LoadEvents(void *f, D3OBJECT *obj)
{
    int n, i, id;
    char hname[0x100];
    char ename[0x100];

    bio_rd_i(f, &n);
    for (i = 0; i < n; i++) {
        bio_rd_s(f, ename);
        id = d3e_EventId(ename);
        bio_rd_s(f, hname);
        if (obj)
            d3e_RegisterEvent(obj, id, d3e_Resolve(obj, id, hname), hname);
    }
}
/* 0x43ea10, reads the event table of the default scene */

void d3e_LoadSceneEvents(void *f)
{
    int n, i, id;
    char hname[0x100];
    char ename[0x100];

    bio_rd_i(f, &n);
    for (i = 0; i < n; i++) {
        bio_rd_s(f, ename);
        id = d3e_EventId(ename);
        bio_rd_s(f, hname);
        d3e_RegisterSceneEvent(d3_DefaultScene, id, d3e_Resolve(d3_DefaultScene, id, hname), hname);
    }
}
