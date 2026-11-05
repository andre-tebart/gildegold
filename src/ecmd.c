/* ecmd.c -- original file name unknown: object script commands ("ecmd_KillObject: Could not find object
   to kill"). Extent in GildeGold.exe (inferred): starts at 0x40d680 right after the character commands
   (chc.c, whose registration ends at 0x40d678) and ends with the command registration at 0x40f960.
   Functions are kept in their original order. Names are ours (ocmd_ + the script name).
   Open: ocmd_Register (0x40f960) still declares the handlers as K&R `int ocmd_X();` placeholders, not ecmd.h's prototypes.
   Open: ecmd.h names par_CreateEmitter's parameters (count, pos, parent, ..); the order is (parent, pos, count, ..).
   unit of rea 8cef7d5b (src/game/ecmd.c); boundaries in decl/text.dcl */
struct D3OBJECT;
struct D3SURF;
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"
#include "d3_sm.h"
#include "path.h"
#include "ch.h"
#define d3anim_StartAnimCh ((int (*)(void *, char *, CHANIMFLAGS))d3anim_StartAnim)  /* 0x435550 (d3.h: D3ANIMFLAGS f): @original this unit passes its field-wise CHANIMFLAGS, 12 bytes like D3ANIMFLAGS, as the original TU did */
#include "ecmd.h"
#include "d3_light.h"
#include "bio.h"
#include "d3_engine.h"
#include "d3_int_io.h"
#include "d3_interface.h"
#include "d3_io_t.h"
#include "d3_par.h"
#include "d3_tools.h"
#include "d3_vec.h"
#include "mem.h"
#include "d3_pose.h"
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests and clears */

#define OCMD_DEG 0.017453292f       /* degrees to radians */


/* 0x40d680, the handle of the object with the name, or 0 after reporting an error */
int ocmd_GetObjectHandle(char **name)
{
    char msg[0x200];
    D3OBJECT *obj;

    obj = d3_FindObject(0, 0x140, 0, *name);
    if (!obj) {
        sprintf(msg, "GetObjectHandle: Could not find object '%s'", *name);
        evt_Error(evt_Cur, evt_Cur->pc, msg);
        return 0;
    }
    return (int)obj;
}
/* 0x40d6e0, collects the objects whose name contains f->pattern */

char ocmd_FindCB(D3OBJECT *obj, OCMDFIND *f)
{
    if (strstr(obj->name, f->pattern)) {
        f->objs[f->n] = obj;
        f->n++;
    }
    return f->n < 32;
}
/* 0x40d730, returns a random object of the default scene whose name contains *pattern (rand() picks among the
   matches); 0 when nothing matches */

int ocmd_GetRndObjectHandle(char **pattern)
{
    OCMDFIND f;

    memset(&f, 0, sizeof(f));
    strcpy(f.pattern, *pattern);
    d3_ForEachObject(d3_DefaultScene, 0, 0x140, (D3OBJECTCB)ocmd_FindCB, &f);
    if (!f.n)
        return 0;
    return (int)f.objs[rand() % f.n];
}
/* 0x40d7b0, as GetRndObjectHandle, but only among the children of *parent; 0 when *parent is NULL / not a live
   object or nothing matches */

int ocmd_GetRndSubObjectHandle(D3OBJECT **parent, char **pattern)
{
    OCMDFIND f;

    if (!*parent)
        return 0;
    if (!mem_IsValid(*parent))
        return 0;
    memset(&f, 0, sizeof(f));
    strcpy(f.pattern, *pattern);
    d3_ForEachObject(d3_DefaultScene, *parent, 0x340, (D3OBJECTCB)ocmd_FindCB, &f);
    if (!f.n)
        return 0;
    return (int)f.objs[rand() % f.n];
}
/* 0x40d860, returns the first child of *parent whose name is *name; a script error and 0 when there is none */

int ocmd_GetSubObjectHandle(D3OBJECT **parent, char **name)
{
    char msg[0x200];
    D3OBJECT *obj;

    obj = d3_FindObject((int)*parent, 0x140, 0, *name);
    if (!obj) {
        sprintf(msg, "GetSubObjectHandle: Could not find object '%s'", *name);
        evt_Error(evt_Cur, evt_Cur->pc, msg);
    }
    return (int)obj;
}
/* 0x40d8d0, creates the object *name at (*x, *y, *z) with a zero rotation and relights it; returns the object,
   0 on failure */

int ocmd_CreateObject(float *x, float *y, float *z, char **name)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *obj;

    rot.x = 0.0f;
    rot.y = 0.0f;
    rot.z = 0.0f;
    pos.x = *x;
    pos.y = *y;
    pos.z = *z;
    obj = d3_CreateObject(0, (D3VECTOR *)&pos, &rot, *name);
    if (obj)
        d3_RelightObject(obj);
    return (int)obj;
}
/* 0x40d940, creates the object *name at the world position of the dummy *dummy, with the dummy's rotation, and
   relights it; 0 when *dummy is NULL or the creation fails */

int ocmd_CreateObjectAtDummy(char **name, D3OBJECT **dummy)
{
    D3VECTOR4 pos;
    D3OBJECT *obj;

    if (!*dummy)
        return 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    obj = d3_CreateObject(0, (D3VECTOR *)&pos, &(*dummy)->rotation, *name);
    if (obj)
        d3_RelightObject(obj);
    return (int)obj;
}
/* 0x40d9a0, loads the group *name at the world position of the dummy *dummy with the dummy's angles and relights
   its objects; 0 when *dummy is NULL */

int ocmd_CreateObjectGroupAtDummy(char **name, D3OBJECT **dummy)
{
    D3VECTOR rot;
    D3VECTOR4 pos;
    D3OBJECT *obj;

    if (!*dummy)
        return 0;
    d3_PointToWorld(*dummy, &(*dummy)->pos, (D3VECTOR *)&pos);
    obj = d3_LoadGroup(*name, 0, 0);
    d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    d3_WorldAngles(*dummy, &rot);
    d3_SetObjectRot(obj, &rot);
    if (obj)
        d3_RelightObjects(obj);
    return (int)obj;
}
/* 0x40da10, deletes *obj (d3if_Delete); a NULL object is only a script error, so 1 is returned either way */

int ocmd_KillObject(D3OBJECT **obj)
{
    if (*obj) {
        d3if_Delete(*obj);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_KillObject: Could not find object to kill...");
    return 1;
}
/* 0x40da50, sets *obj's position from *x / *y / *z and updates it (d3_SetObjectPos); 0, a NULL object is a
   script error */

int ocmd_SetPos(D3OBJECT **obj, float *x, float *y, float *z)
{
    if (*obj) {
        (*obj)->pos.x = *x;
        (*obj)->pos.y = *y;
        (*obj)->pos.z = *z;
        d3_SetObjectPos(*obj, &(*obj)->pos);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_SetPos: Could not find object to set pos...");
    return 0;
}
/* 0x40dab0, sets *obj's angles from *x / *y / *z in degrees (OCMD_DEG) and marks it to be rerotated (state |= 4);
   0, a NULL object is a script error */

int ocmd_SetAngle(D3OBJECT **obj, float *x, float *y, float *z)
{
    if (*obj) {
        (*obj)->rotation.x = *x * OCMD_DEG;
        (*obj)->rotation.y = *y * OCMD_DEG;
        (*obj)->rotation.z = *z * OCMD_DEG;
        (*obj)->state |= 4;
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_SetAngle: Could not find object to set the angle...");
    return 0;
}
/* 0x40db30, moves *obj to (*x, *y, *z) over *time, keeping its angles (d3t_PlaceObject); 0, a NULL object is a
   script error */

int ocmd_MoveObject(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, (float)*x, (float)*y, (float)*z, o->rotation.x, o->rotation.y, o->rotation.z, *time, 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_MoveObject: Could not find object to move...");
    return 0;
}
/* 0x40dbb0, returns its first argument */

int ocmd_RotateObject(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, o->pos.x, o->pos.y, o->pos.z,
                        *x * OCMD_DEG, *y * OCMD_DEG, *z * OCMD_DEG, *time, 0);
        return (int)obj;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_RotateObject: Could not find object to rotate...");
    return (int)obj;
}
/* 0x40dc30, moves *obj by (*x, *y, *z) relative to its position over *time; 0, a NULL object is a script error */

int ocmd_MoveObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, *x + o->pos.x, *y + o->pos.y, *z + o->pos.z,
                        o->rotation.x, o->rotation.y, o->rotation.z, *time, 0);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_MoveObjectRelative: Could not find object to move...");
    return 0;
}
/* 0x40dcb0, rotates *obj by (*x, *y, *z) degrees relative to its angles over *time; 1, a NULL object is a
   script error */

int ocmd_RotateObjectRelative(D3OBJECT **obj, int *x, int *y, int *z, int *time)
{
    D3OBJECT *o;

    if (o = *obj) {
        d3t_PlaceObject(o, o->pos.x, o->pos.y, o->pos.z, *x * OCMD_DEG + o->rotation.x,
                        *y * OCMD_DEG + o->rotation.y, *z * OCMD_DEG + o->rotation.z, *time, 0);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_RotateObjectRelative: Could not find object to rotate...");
    return 1;
}
/* 0x40dd50, sets the ambient light colour from *r / *g / *b and invalidates the light cache; 1 */

int ocmd_SetAmbiente(int *r, int *g, int *b)
{
    d3_AmbientColor.x = (float)*r;
    d3_AmbientColor.y = (float)*g;
    d3_AmbientColor.z = (float)*b;
    d3_LightInvalidate(1);
    return 1;
}
/* 0x40dd90, switches the light recalculation on (*on) or off and invalidates the light cache; 1 */

int ocmd_LightCalc(int *on)
{
    d3_LightInvalidate((char)*on);
    return 1;
}
/* 0x40ddb0, reloads *obj from the model *name (d3if_Reload); 1, a NULL object is a script error */

int ocmd_ReplaceObject(D3OBJECT **obj, char **name)
{
    if (*obj) {
        d3if_Reload(*obj, *name);
        return 1;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_ReplaceObject: Could not find object to replace...");
    return 1;
}
/* 0x40ddf0, loads the scene *name; a script error when it is not on disk; 1 */

int ocmd_LoadScene(char **name)
{
    if (!d3_LoadScene(*name, 0, 0))
        evt_Error(evt_Cur, evt_Cur->pc, "ecmd_LoadScene: Could not find scene on disk...");
    return 1;
}
/* 0x40de30, waits until the animation slots of the object are free */

int ocmd_AttachAnim(D3OBJECT **pobj, char **file, int *once)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    D3OBJECT *obj;
    int i;

    obj = *pobj;
    if (evt_CurCall && evt_CurCall->fn == ocmd_AttachAnim) {
        for (i = 0; i < 3; i++) {
            if (obj->material->tex[i].name) {
                evt_Cur->waitfn = ocmd_AttachAnim;
                return 0;
            }
        }
        return 0;
    }
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    util_BaseName(name);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.u.c.b1 = *once ? 0 : 8;
        f.b8 = 1;
        f.u.c.b1 |= 0x10;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        d3anim_StartAnimCh(obj->info, name, f);
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_AttachAnim;
    return 0;
}
/* 0x40dfc0, loads the animation by attaching and releasing it */

int ocmd_PreloadAnim(D3OBJECT **pobj, char **file)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    D3OBJECT *obj;
    char *base;

    obj = *pobj;
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    base = util_BaseName(name);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = 0x10;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        d3anim_StartAnimCh(obj->info, base, f);
        d3_ReleaseTexture(obj->info, base);
    }
    return 0;
}
/* 0x40e0e0, attaches the animation *file to *obj, looped *n times (b1 0x10 when *n is 1); while the object's
   textures are still loaded the command waits (evt_Cur->waitfn) and returns; a script error when the file is
   missing; 0 */

int ocmd_AttachAnimLooped(D3OBJECT **pobj, char **file, int *n)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    char base[0x100];
    D3OBJECT *obj;
    char *p;
    int i;

    obj = *pobj;
    if (evt_CurCall && evt_CurCall->fn == ocmd_AttachAnimLooped) {
        for (i = 0; i < 3; i++) {
            if (obj->material && obj->material->tex[i].name) {
                evt_Cur->waitfn = ocmd_AttachAnimLooped;
                return 0;
            }
        }
        return 0;
    }
    strcpy(path, *file);
    strcat(path, ".baf");
    strcpy(name, path);
    p = util_BaseName(name);
    if (p)
        strcpy(base, p);
    if (path[0] && obj && obj->info) {
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 0);
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = *n == 1 ? 0x10 : 0;
        f.u.c.b0 = *n;
        f.b = 0;
        f.a = 1;
        if (!d3anim_StartAnimCh(obj->info, name, f))
            evt_Error(evt_Cur, evt_Cur->pc, "AttachAnimLooped(): Could not find animation-file...");
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_AttachAnimLooped;
    return 0;
}
/* 0x40e2b0, attaches a->file to every object whose name contains a->pattern */

char ocmd_AttachAnimCB(D3OBJECT *obj, OCMDANIM *a)
{
    CHANIMFLAGS f;
    char path[0x100];
    char name[0x100];
    char base[0x100];
    D3ANIMSLOT *s;
    char *p;

    if (strstr(obj->name, a->pattern)) {
        strcpy(path, a->file);
        strcat(path, ".baf");
        strcpy(name, path);
        p = util_BaseName(name);
        if (p)
            strcpy(base, p);
        if (path[0] && obj && obj->info) {
            if (!d3_FindAnim(name))
                d3_LoadAnimStream(path, name, 0);
            memset(&f, 0, sizeof(f));
            f.u.all = 0;
            f.b8 = 1;
            f.u.c.b1 = a->n == 1 ? 0x10 : 0;
            f.u.c.b0 = a->n;
            f.b = 0;
            f.a = 1;
            s = (D3ANIMSLOT *)d3anim_StartAnimCh(obj->info, name, f);
            if (s && a->rnd) {
                s->frame = rand() % (s->data->nframes - 1);
                s->nextFrame = s->frame + 1;
            }
        }
    }
    return 1;
}
/* 0x40e430, attaches the animation *file, looped *n times, to every object whose name contains *pattern
   (ocmd_AttachAnimCB; *rnd starts each at a random frame); 0 */

int ocmd_AttachAnimLoopedToAll(char **pattern, char **file, int *n, int *rnd)
{
    OCMDANIM a;

    strcpy(a.pattern, *pattern);
    strcpy(a.file, *file);
    a.n = *n;
    a.rnd = *rnd;
    if (a.pattern[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0x40, (D3OBJECTCB)ocmd_AttachAnimCB, &a);
    return 0;
}
/* 0x40e4c0, d3_ForEachObject callback of DetachAnimFromAll: releases the textures of every object whose name
   contains a->pattern; 1 (keeps the walk going) */

char ocmd_DetachAnimCB(D3OBJECT *obj, OCMDANIM *a)
{
    if (strstr(obj->name, a->pattern))
        d3t_ReleaseObjectTextures(obj);
    return 1;
}
/* 0x40e4f0, releases the animation textures of every object whose name contains *pattern; 0 */

int ocmd_DetachAnimFromAll(char **pattern)
{
    OCMDANIM a;

    strcpy(a.pattern, *pattern);
    strcpy(a.file, "");
    a.n = 0;
    a.rnd = 0;
    if (a.pattern[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, (D3OBJECTCB)ocmd_DetachAnimCB, &a);
    return 0;
}

/* ---- 0x40e560.. (worker ocmd2) ---- */

char d3if_SelectTextureSet(struct D3OBJECT *obj, struct D3SURF *surf, unsigned char set, char reload); /* 0x446f30 */
char d3_SetLightSet(unsigned int set, float t, float scale, char calc);                /* 0x449de0 */
char d3_SetFogSet(unsigned int a, unsigned int b, float t);                            /* 0x44a170 */

/* 0x40e560, d3_ForEachObject callback of SelectAllTextureSets */
char ocmd_SelectTextureSetCB(D3OBJECT *obj, OCMDTEXSEL *ctx)
{
    if (obj->info) {
        if (strstr(obj->name, ctx->name))
            d3if_SelectTextureSet(obj, obj->info, ctx->set, 1);
    }
    return 1;
}
/* 0x40e5a0, switches every object whose name contains *name to the texture set *set (ocmd_SelectTextureSetCB
   through d3_ForEachObject); 0 */

int ocmd_SelectAllTextureSets(char **name, int *set)
{
    OCMDTEXSEL ctx;

    strcpy(ctx.name, *name);
    ctx.file[0] = ch_EmptyName[0];
    ctx.n = 0;
    ctx.rnd = 0;
    ctx.set = *set;
    if (ctx.name[0])
        d3_ForEachObject(d3_DefaultScene, 0, 0xc0, ocmd_SelectTextureSetCB, &ctx);
    return 0;
}
/* 0x40e620, sets the global d3_NoTextures flag; 1 */

int ocmd_SetNoTextures(int *on)
{
    d3_NoTextures = *on;
    return 1;
}
/* 0x40e640, switches the day-time light set of d3_interface.c to *set (no blend); 1 */

int ocmd_SetLightSet(int *set)
{
    d3_SetLightSet(*set, 1.0f, 1.0f, 0);
    return 1;
}
/* 0x40e670, switches the fog to the blend of fog sets *on and *a, *b in percent; 1 */

int ocmd_SetFogSet(int *on, int *a, int *b)
{
    d3_SetFogSet(*on, *a, max(min(*b * 0.01f, 1.0f), 0.0f));
    return 1;
}
/* 0x40e6f0, starts a camera flight through the two dummies *a / *b over *time (17 units per second); a script
   error when a dummy is missing; 1 */

int ocmd_CameraFlight(int *time, char **a, char **b)
{
    if (!d3t_MoveCameraPath(*time / 17, 2, *a, *b))
        evt_Error(evt_Cur, evt_Cur->pc, "CameraFlight(): Could not find one or few dummies");
    return 1;
}
/* 0x40e750, waits until the flight is over */

int ocmd_CameraFlightEnhanced(int *time, char **a, char **b, char **c, char **d, char **e, char **f)
{
    if (evt_CurCall && evt_CurCall->fn == ocmd_CameraFlightEnhanced) {
        if (d3t_CamMoving || d3t_CamMoveI) {
            evt_Cur->waitfn = ocmd_CameraFlightEnhanced;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_CameraFlightEnhanced;
    if (!d3t_MoveCameraPath(*time / 17, 6, *a, *b, *c, *d, *e, *f))
        evt_Error(evt_Cur, evt_Cur->pc, "CameraFlight(): Could not find one or few dummies");
    return 1;
}
/* 0x40e810, same without the error */

int ocmd_NewCameraFlight(int *time, char **a, char **b, char **c, char **d, char **e, char **f)
{
    if (evt_CurCall && evt_CurCall->fn == ocmd_NewCameraFlight) {
        if (d3t_CamMoving || d3t_CamMoveI) {
            evt_Cur->waitfn = ocmd_NewCameraFlight;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_NewCameraFlight;
    d3t_MoveCameraPath(*time / 14, 6, *a, *b, *c, *d, *e, *f);
    return 1;
}
/* 0x40e8b0, flights *obj along the path of up to five dummies *a..*e over *time; waits while a camera flight
   runs, a script error when *obj is NULL; 1 */

int ocmd_ObjectFlight(D3OBJECT **obj, int *time, char **a, char **b, char **c, char **d, char **e)
{
    D3OBJECT *path[5];
    int i, n;

    n = 0;
    if (evt_CurCall && evt_CurCall->fn == ocmd_ObjectFlight) {
        if (d3t_CamMoving || d3t_CamMoveI) {
            evt_Cur->waitfn = ocmd_ObjectFlight;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_ObjectFlight;
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "ObjectFlight(): Could not find object");
        return 0;
    }
    path[0] = d3_FindObject(0, 0x140, 0, *a);
    path[1] = d3_FindObject(0, 0x140, 0, *b);
    path[2] = d3_FindObject(0, 0x140, 0, *c);
    path[3] = d3_FindObject(0, 0x140, 0, *d);
    path[4] = d3_FindObject(0, 0x140, 0, *e);
    for (i = 0; i < 5; i++)
        if (path[i])
            n++;
    d3t_MoveObjectPath(*obj, *time / 14, n, path);
    return 1;
}
/* 0x40ea00, zooms the camera onto the object *name over *time (d3t_PlaceAtObject), waiting while a camera flight
   runs; 1 */

int ocmd_ZoomOnObject(int *time, char **name)
{
    D3OBJECT *obj;

    if (evt_CurCall && evt_CurCall->fn == ocmd_ZoomOnObject) {
        if (d3t_CamMoving || d3t_CamMoveI) {
            evt_Cur->waitfn = ocmd_ZoomOnObject;
            return 0;
        }
        return 0;
    }
    if (evt_Cur->mode == 1)
        evt_Cur->waitfn = ocmd_ZoomOnObject;
    obj = d3_FindObject(0, 0x160, 0, *name);
    if (obj)
        d3t_PlaceAtObject(obj, *time / 17);
    return 1;
}
/* 0x40ea90, creates a rain particle system of *count drops (d3t_CreateRain); returns it */

int ocmd_CreateRain(int *count)
{
    return (int)d3t_CreateRain(*count);
}
/* 0x40eaa0, deletes the rain particle system *rain (d3t_DeleteRain); 0 */

int ocmd_DeleteRain(void **rain)
{
    d3t_DeleteRain(*rain);
    return 0;
}
/* 0x40eac0, callback of CreateParticle: a stream of particles falling from y = 100 */

static void ocmd_ParticleCB(D3PARTSYS *ps, unsigned long time)
{
    int i;
    double dt;

    for (i = 0; i < ps->count; i++) {
        dt = (time - ps->particles[i].time) * 0.001;
        ps->particles[i].pos.x = ps->particles[i].start.x + ps->particles[i].vel.x;
        ps->particles[i].pos.y = ps->particles[i].start.y + ps->particles[i].vel.y;
        ps->particles[i].pos.z = ps->particles[i].start.z + ps->particles[i].vel.z;
        ps->particles[i].size += dt * 2.0;
        ps->particles[i].vel.y -= dt;
        if (ps->particles[i].pos.y < 0.0) {
            ps->particles[i].pos.x = 0.0f;
            ps->particles[i].pos.y = 100.0f;
            ps->particles[i].pos.z = 0.0f;
            ps->particles[i].vel.x = (float)rand() / RAND_MAX * 0.2 - 0.1;
            ps->particles[i].vel.y = 0.1 - ((float)rand() / RAND_MAX + 1.0) * 0.2;
            ps->particles[i].vel.z = -0.5 - ps->particles[i].vel.y;
            ps->particles[i].time = time;
            ps->particles[i].size = ps->size;
        }
        ps->particles[i].start.x = ps->particles[i].pos.x;
        ps->particles[i].start.y = ps->particles[i].pos.y;
        ps->particles[i].start.z = ps->particles[i].pos.z;
        ps->particles[i].a = (unsigned char)max(0.0, ps->particles[i].pos.y * 1.5 - 20.0);
    }
}
/* 0x40ec70, creates a 100-particle "blut" system attached to *parent with ocmd_ParticleCB (a stream falling from
   y = 100) and lays its particles out; returns it */

int ocmd_CreateParticle(void **parent)
{
    D3PARTSYS *ps;
    D3TEXFLAGS flags;
    int i;

    flags.all = 0;
    flags.b[0] = 0x7f;
    flags.b[2] |= 3;
    ps = par_Create(100, "blut", flags, 1.0f, *parent, (D3PARTCB)ocmd_ParticleCB);
    for (i = 0; i < 100; i++) {
        ps->particles[i].pos.x = 0.0f;
        ps->particles[i].pos.y = i * 0.16f;
        ps->particles[i].pos.z = 0.0f;
        ps->particles[i].start.x = ps->particles[i].pos.x;
        ps->particles[i].start.y = ps->particles[i].pos.y;
        ps->particles[i].start.z = ps->particles[i].pos.z;
        ps->particles[i].vel.x = (float)rand() / RAND_MAX * 0.2 - 0.1;
        ps->particles[i].vel.y = 0.1 - ((float)rand() / RAND_MAX + 1.0) * 0.2;
        ps->particles[i].vel.z = -0.5 - ps->particles[i].vel.y;
        ps->particles[i].a = (unsigned char)max(0.0, ps->particles[i].pos.y * 1.5 - 20.0);
    }
    return (int)ps;
}
/* 0x40edc0, deletes the particle system *ps (par_Delete); an invalid handle is a script error; 0 */

int ocmd_KillParticle(D3PARTSYS **ps)
{
    if (mem_IsValid(*ps)) {
        par_Delete(*ps);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "ecmd_KillParticle: invalid handle...");
    return 0;
}
/* 0x40ee10, puts the camera at the position and angles of the camera dummy *name (d3t_FindObject flag 2); a
   script error when there is no such dummy; 0 */

int ocmd_SetCameraToDummy(char **name)
{
    D3OBJECT *dummy;
    char msg[0x80];

    dummy = d3t_FindObject(*name, 2);
    if (dummy) {
        d3_SetObjectPos(d3_Camera, &dummy->savedPos);
        d3_SetObjectRot(d3_Camera, &dummy->savedRot);
        return 0;
    }
    sprintf(msg, "SetCameraToDummy(): Could not find Cameradummy '%s'!", *name);
    evt_Error(evt_Cur, evt_Cur->pc, msg);
    return 0;
}
/* 0x40eea0, stops the camera flight: frees the camera's path and clears the moving flags; 0 */

int ocmd_StopCameraFlight(void)
{
    d3anim_FreePath(d3_Camera);
    d3t_CamMoving = d3t_CamMoveI = 0;
    return 0;
}
/* 0x40eec0, renames *obj to *name; 0 when *obj is NULL, else 1 */

int ocmd_RenameObject(D3OBJECT **obj, char **name)
{
    if (!*obj)
        return 0;
    strcpy((*obj)->name, *name);
    return 1;
}
/* 0x40eef0, sets / clears the blocked bit of *obj's stateFlags from *on; a script error (0) for a NULL object,
   else 1 */

int ocmd_SetBlocked(D3OBJECT **obj, int *on)
{
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetBlocked: Illegal object...");
        return 0;
    }
    ((OCMDOBJBITS *)&(*obj)->stateFlags)->blocked = *on;
    return 1;
}
/* 0x40ef40, sets the two transient bits of *obj's stateFlags from *mode; a script error (0) for a NULL object,
   else 1 */

int ocmd_SetTransient(D3OBJECT **obj, int *mode)
{
    if (!*obj) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetTransient: Illegal object...");
        return 0;
    }
    ((OCMDOBJBITS *)&(*obj)->stateFlags)->transient = *mode;
    return 1;
}
/* 0x40ef90, fills the emitter template d3_EmitterCfg (type *a, the default speeds / sizes, the direction d3_YAxis,
   the flags from *f0 / *f1) and creates a *count emitter of the texture *tex and size *size at the origin,
   attached to *parent; returns it */

int ocmd_CreateEmitter(int *a, int *count, int *parent, int *size, char **tex, int *f0, int *f1)
{
    OCMDTEXFLAGS flags;

    D3_ZERO_AT(offset d3_EmitterCfg, sizeof(d3_EmitterCfg));
    d3_EmitterCfg.type = *a;
    d3_EmitterCfg.f34 = 10.0f;
    d3_EmitterCfg.f38 = 50.0f;
    d3_EmitterCfg.f3c = 30.0f;
    d3_EmitterCfg.dir = d3_YAxis;
    d3_EmitterCfg.f80 = 1.0f;
    d3_EmitterCfg.f8c = 255.0f;
    d3_EmitterCfg.i90 = 20;
    d3_EmitterCfg.i94 = 160;
    d3_EmitterCfg.i98 = 180;
    d3_EmitterCfg.c9e = 0xff;
    d3_EmitterCfg.c9d = 0xff;
    d3_EmitterCfg.c9c = 0xff;
    d3_EmitterCfg.c9f = 0;
    d3_EmitterCfg.flags |= 0x20;
    flags.f0 = *f1;
    flags.f1 = 1;
    flags.b0 = *f0;
    return (int)par_CreateEmitter(*count, &d3_Origin, *parent, *tex, flags, (float)*size, &d3_EmitterCfg);
}
/* 0x40f0c0, deletes the emitter *e (par_Delete); an invalid handle is a script error; 0 */

int ocmd_KillEmitter(D3EMITTER **e)
{
    if (mem_IsValid(*e)) {
        par_Delete((D3PARTSYS *)*e);
        return 0;
    }
    evt_Error(evt_Cur, evt_Cur->pc, "KillEmitter: invalid handle...");
    return 0;
}
/* 0x40f110, sets the emitter's start amplitudes (*a..*c / 100), the end amplitude amp3 (*d / 100) and amp4
   (*f / 100); 0, an invalid handle is a script error */

int ocmd_SetEmitterAmplitude(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterAmplitude: invalid handle...");
        return 0;
    }
    {
        float fa, fb, fc, fd, ff;

        fa = *a * 0.01f;
        fb = *b * 0.01f;
        fc = *c * 0.01f;
        ff = *f * 0.01f;
        fd = *d * 0.01f;
        em->amp[0] = fa;
        em->amp[1] = fb;
        em->amp[2] = fc;
        em->amp3 = fd;
        em->amp4 = ff;
    }
    return 0;
}
/* 0x40f1b0, sets the emitter's start phase speeds (*a..*c / 100), phase3 (*d / 100) and phase4 (*f / 100); 0,
   an invalid handle is a script error */

int ocmd_SetEmitterPhasespeed(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterPhasespeed: invalid handle...");
        return 0;
    }
    {
        float fa, fb, fc, fd, ff;

        fa = *a * 0.01f;
        fb = *b * 0.01f;
        fc = *c * 0.01f;
        ff = *f * 0.01f;
        fd = *d * 0.01f;
        em->phase[0] = fa;
        em->phase[1] = fb;
        em->phase[2] = fc;
        em->phase3 = fd;
        em->phase4 = ff;
    }
    return 0;
}
/* 0x40f250, sets the emitter's direction from *x / *y / *z (in hundredths); 0, an invalid handle is a script error */

int ocmd_SetEmitterDirection(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterDirection: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->dir[0] = fx;
        em->dir[1] = fy;
        em->dir[2] = fz;
    }
    return 0;
}
/* 0x40f2c0, sets the emitter's start sizes from *x / *y / *z; 0, an invalid handle is a script error */

int ocmd_SetEmitterSize(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterSize: invalid handle...");
        return 0;
    }
    em->size[0] = (float)*x;
    em->size[1] = (float)*y;
    em->size[2] = (float)*z;
    return 0;
}
/* 0x40f320, sets the emitter's start velocities from *x / *y / *z (in hundredths); 0, an invalid handle is a script
   error */

int ocmd_SetEmitterVelocity(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterVelocity: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->vel[0] = fx;
        em->vel[1] = fy;
        em->vel[2] = fz;
    }
    return 0;
}
/* 0x40f390, sets the emitter's accelerations from *x / *y / *z (in thousandths); 0, an invalid handle is a script
   error */

int ocmd_SetEmitterAcceleration(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterAcceleration: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.001f;
        fy = *y * 0.001f;
        fz = *z * 0.001f;
        em->acc[0] = fx;
        em->acc[1] = fy;
        em->acc[2] = fz;
    }
    return 0;
}
/* 0x40f400, sets the emitter's random velocity range from *x / *y / *z (in hundredths); 0, an invalid handle is a
   script error */

int ocmd_SetEmitterRndVelocity(D3EMITTER **e, int *x, int *y, int *z)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterRndVelocity: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->rnd[0] = fx;
        em->rnd[1] = fy;
        em->rnd[2] = fz;
    }
    return 0;
}
/* 0x40f470, sets the emitter's plane normal from *x / *y / *z (in hundredths) and the plane distance *d; 0, an
   invalid handle is a script error */

int ocmd_SetEmitterPlane(D3EMITTER **e, int *x, int *y, int *z, int *d)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterPlane: invalid handle...");
        return 0;
    }
    {
        float fx, fy, fz;

        fx = *x * 0.01f;
        fy = *y * 0.01f;
        fz = *z * 0.01f;
        em->plane[0] = fx;
        em->plane[1] = fy;
        em->plane[2] = fz;
    }
    em->plane3 = (float)*d;
    return 0;
}
/* 0x40f4f0, sets the emitter's particle time (*t) and the three alpha values *a / *b / *c; 0, an invalid handle is
   a script error */

int ocmd_SetEmitterTimeAndAlpha(D3EMITTER **e, int *a, int *b, int *c, int *t)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTimeAndAlpha: invalid handle...");
        return 0;
    }
    em->alpha[0] = *a;
    em->alpha[1] = *b;
    em->alpha[2] = *c;
    em->time = (float)*t;
    return 0;
}
/* 0x40f560, sets the emitter's colour bytes r / g / b / a from *r / *g / *b / *a; 0, an invalid handle is a script
   error */

int ocmd_SetEmitterColor(D3EMITTER **e, int *r, int *g, int *b, int *a)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterColor: invalid handle...");
        return 0;
    }
    em->r = *r;
    em->g = *g;
    em->b = *b;
    em->a = *a;
    return 0;
}
/* 0x40f5d0, sets the emitter's max trigger (*a / 100), texture mode *b, the init / rebirth fill flags *c / *d and
   the trigger flags *f / *g; 0, an invalid handle is a script error */

int ocmd_SetEmitterFlags(D3EMITTER **e, int *a, int *b, int *c, int *d, int *f, int *g)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterFlags: invalid handle...");
        return 0;
    }
    em->maxtrigger = *a * 0.01f;
    em->texmode = *b;
    em->initfill = *c;
    em->rebirthfill = *d;
    em->istrigger = *f;
    em->triggeronce = *g;
    return 0;
}
/* 0x40f690, sets the emitter's init-fill flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterInitFill(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterInitFill: invalid handle...");
        return 0;
    }
    em->initfill = *on;
    return 0;
}
/* 0x40f6f0, sets the emitter's rebirth-fill flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterRebirthFill(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterRebirthFill: invalid handle...");
        return 0;
    }
    em->rebirthfill = *on;
    return 0;
}
/* 0x40f750, sets the emitter's texture mode from *mode; 0, an invalid handle is a script error */

int ocmd_SetEmitterTextureMode(D3EMITTER **e, int *mode)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTextureMode: invalid handle...");
        return 0;
    }
    em->texmode = *mode;
    return 0;
}
/* 0x40f7a0, sets the emitter's is-trigger flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterIsTrigger(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterIsTrigger: invalid handle...");
        return 0;
    }
    em->istrigger = *on;
    return 0;
}
/* 0x40f800, sets the emitter's trigger-once flag from *on; 0, an invalid handle is a script error */

int ocmd_SetEmitterTriggerOnce(D3EMITTER **e, int *on)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterTriggerOnce: invalid handle...");
        return 0;
    }
    em->triggeronce = *on;
    return 0;
}
/* 0x40f850, sets the emitter's max trigger from *n; 0, an invalid handle is a script error */

int ocmd_SetEmitterMaxTrigger(D3EMITTER **e, int *n)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetEmitterMaxTrigger: invalid handle...");
        return 0;
    }
    em->maxtrigger = (float)*n;
    return 0;
}
/* 0x40f8a0, raises the emitter's trigger bit; 0, an invalid handle is a script error */

int ocmd_TriggerEmitter(D3EMITTER **e)
{
    D3EMITTER *em = *e;

    if (!mem_IsValid(em)) {
        evt_Error(evt_Cur, evt_Cur->pc, "TriggerEmitter: invalid handle...");
        return 0;
    }
    em->trigger = 1;
    return 0;
}
/* 0x40f8f0, moves the particle system *ps to (*x, *y, *z) (par_SetPos); 0, an invalid handle is a script error */

int ocmd_SetParticlePos(D3PARTSYS **ps, int *x, int *y, int *z)
{
    D3PARTSYS *p = *ps;
    D3VECTOR4 pos;

    if (!mem_IsValid(p)) {
        evt_Error(evt_Cur, evt_Cur->pc, "SetParticlePos: invalid handle...");
        return 0;
    }
    pos.x = (float)*x;
    pos.y = (float)*y;
    pos.z = (float)*z;
    par_SetPos(p, (D3VECTOR *)&pos);
    return 0;
}

/* ---- 0x40f960.. (worker sndreg) ---- */
/* handlers registered by ocmd_Register; defined above by workers ocmd / ocmd2 (prototypes here are
   address placeholders, the orchestrator reconciles them with the real definitions) */
int ocmd_CreateObject();  /* 0x40d8d0 */
int ocmd_CreateObjectAtDummy();  /* 0x40d940 */
int ocmd_CreateObjectGroupAtDummy();  /* 0x40d9a0 */
int ocmd_KillObject();  /* 0x40da10 */
int ocmd_SetPos();  /* 0x40da50 */
int ocmd_SetAngle();  /* 0x40dab0 */
int ocmd_MoveObject();  /* 0x40db30 */
int ocmd_RotateObject();  /* 0x40dbb0 */
int ocmd_MoveObjectRelative();  /* 0x40dc30 */
int ocmd_RotateObjectRelative();  /* 0x40dcb0 */
int ocmd_SetAmbiente();  /* 0x40dd50 */
int ocmd_LightCalc();  /* 0x40dd90 */
int ocmd_ReplaceObject();  /* 0x40ddb0 */
int ocmd_LoadScene();  /* 0x40ddf0 */
int ocmd_AttachAnim();  /* 0x40de30 */
int ocmd_PreloadAnim();  /* 0x40dfc0 */
int ocmd_AttachAnimLooped();  /* 0x40e0e0 */
int ocmd_AttachAnimLoopedToAll();  /* 0x40e430 */
int ocmd_DetachAnimFromAll();  /* 0x40e4f0 */
int ocmd_GetObjectHandle();  /* 0x40d680 */
int ocmd_GetRndObjectHandle();  /* 0x40d730 */
int ocmd_GetRndSubObjectHandle();  /* 0x40d7b0 */
int ocmd_GetSubObjectHandle();  /* 0x40d860 */
int ocmd_SetNoTextures();  /* 0x40e620 */
int ocmd_SetLightSet();  /* 0x40e640 */
int ocmd_SetFogSet();  /* 0x40e670 */
int ocmd_CameraFlight();  /* 0x40e6f0 */
int ocmd_ZoomOnObject();  /* 0x40ea00 */
int ocmd_CameraFlightEnhanced();  /* 0x40e750 */
int ocmd_NewCameraFlight();  /* 0x40e810 */
int ocmd_ObjectFlight();  /* 0x40e8b0 */
int ocmd_CreateRain();  /* 0x40ea90 */
int ocmd_DeleteRain();  /* 0x40eaa0 */
int ocmd_CreateParticle();  /* 0x40ec70 */
int ocmd_KillParticle();  /* 0x40edc0 */
int ocmd_SetCameraToDummy();  /* 0x40ee10 */
int ocmd_StopCameraFlight();  /* 0x40eea0 */
int ocmd_RenameObject();  /* 0x40eec0 */
int ocmd_SetBlocked();  /* 0x40eef0 */
int ocmd_SetTransient();  /* 0x40ef40 */
int ocmd_CreateEmitter();  /* 0x40ef90 */
int ocmd_KillEmitter();  /* 0x40f0c0 */
int ocmd_SetEmitterAmplitude();  /* 0x40f110 */
int ocmd_SetEmitterPhasespeed();  /* 0x40f1b0 */
int ocmd_SetEmitterDirection();  /* 0x40f250 */
int ocmd_SetEmitterSize();  /* 0x40f2c0 */
int ocmd_SetEmitterVelocity();  /* 0x40f320 */
int ocmd_SetEmitterAcceleration();  /* 0x40f390 */
int ocmd_SetEmitterRndVelocity();  /* 0x40f400 */
int ocmd_SetEmitterPlane();  /* 0x40f470 */
int ocmd_SetEmitterTimeAndAlpha();  /* 0x40f4f0 */
int ocmd_SetEmitterColor();  /* 0x40f560 */
int ocmd_SetEmitterFlags();  /* 0x40f5d0 */
int ocmd_SetEmitterInitFill();  /* 0x40f690 */
int ocmd_SetEmitterRebirthFill();  /* 0x40f6f0 */
int ocmd_SetEmitterTextureMode();  /* 0x40f750 */
int ocmd_SetEmitterIsTrigger();  /* 0x40f7a0 */
int ocmd_SetEmitterTriggerOnce();  /* 0x40f800 */
int ocmd_SetEmitterMaxTrigger();  /* 0x40f850 */
int ocmd_TriggerEmitter();  /* 0x40f8a0 */
int ocmd_SetParticlePos();  /* 0x40f8f0 */
int ocmd_SelectAllTextureSets();  /* 0x40e5a0 */

/* 0x40f960 */
void ocmd_Register(void)
{
    evt_ImportCommand("CreateObject", ocmd_CreateObject, 1, 4, 7, 7, 7, 6);
    evt_ImportCommand("CreateObjectAtDummy", ocmd_CreateObjectAtDummy, 1, 2, 6, 1);
    evt_ImportCommand("CreateObjectGroupAtDummy", ocmd_CreateObjectGroupAtDummy, 1, 2, 6, 1);
    evt_ImportCommand("KillObject", ocmd_KillObject, 5, 1, 1);
    evt_ImportCommand("SetPos", ocmd_SetPos, 5, 4, 1, 7, 7, 7);
    evt_ImportCommand("SetAngle", ocmd_SetAngle, 5, 4, 1, 7, 7, 7);
    evt_ImportCommand("MoveObject", ocmd_MoveObject, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("RotateObject", ocmd_RotateObject, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("MoveObjectRelative", ocmd_MoveObjectRelative, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("RotateObjectRelative", ocmd_RotateObjectRelative, 5, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetAmbiente", ocmd_SetAmbiente, 5, 3, 1, 1, 1);
    evt_ImportCommand("LightCalc", ocmd_LightCalc, 5, 1, 1);
    evt_ImportCommand("ReplaceObject", ocmd_ReplaceObject, 1, 2, 1, 6);
    evt_ImportCommand("LoadScene", ocmd_LoadScene, 5, 1, 6);
    evt_ImportCommand("AttachAnim", ocmd_AttachAnim, 1, 3, 1, 6, 1);
    evt_ImportCommand("PreloadAnim", ocmd_PreloadAnim, 1, 2, 1, 6);
    evt_ImportCommand("AttachAnimLooped", ocmd_AttachAnimLooped, 1, 3, 1, 6, 1);
    evt_ImportCommand("AttachAnimLoopedToAll", ocmd_AttachAnimLoopedToAll, 1, 4, 6, 6, 1, 1);
    evt_ImportCommand("DetachAnimFromAll", ocmd_DetachAnimFromAll, 5, 1, 6);
    evt_ImportCommand("GetObjectHandle", ocmd_GetObjectHandle, 1, 1, 6);
    evt_ImportCommand("GetRndObjectHandle", ocmd_GetRndObjectHandle, 1, 1, 6);
    evt_ImportCommand("GetRndSubObjectHandle", ocmd_GetRndSubObjectHandle, 1, 2, 1, 6);
    evt_ImportCommand("GetSubObjectHandle", ocmd_GetSubObjectHandle, 1, 2, 1, 6);
    evt_ImportCommand("SetNoTextures", ocmd_SetNoTextures, 5, 1, 1);
    evt_ImportCommand("SetLightSet", ocmd_SetLightSet, 5, 1, 1);
    evt_ImportCommand("SetFogSet", ocmd_SetFogSet, 5, 3, 1, 1, 1);
    evt_ImportCommand("CameraFlight", ocmd_CameraFlight, 5, 3, 1, 6, 6);
    evt_ImportCommand("ZoomOnObject", ocmd_ZoomOnObject, 5, 2, 1, 6);
    evt_ImportCommand("CameraFlightEnhanced", ocmd_CameraFlightEnhanced, 5, 7, 1, 6, 6, 6, 6, 6, 6);
    evt_ImportCommand("NewCameraFlight", ocmd_NewCameraFlight, 5, 7, 1, 6, 6, 6, 6, 6, 6);
    evt_ImportCommand("ObjectFlight", ocmd_ObjectFlight, 5, 7, 1, 1, 6, 6, 6, 6, 6);
    evt_ImportCommand("CreateRain", ocmd_CreateRain, 1, 1, 1);
    evt_ImportCommand("DeleteRain", ocmd_DeleteRain, 1, 1, 1);
    evt_ImportCommand("CreateParticle", ocmd_CreateParticle, 5, 1, 1);
    evt_ImportCommand("KillParticle", ocmd_KillParticle, 5, 1, 1);
    evt_ImportCommand("SetCameraToDummy", ocmd_SetCameraToDummy, 5, 1, 6);
    evt_ImportCommand("StopCameraFlight", ocmd_StopCameraFlight, 5, 0);
    evt_ImportCommand("RenameObject", ocmd_RenameObject, 1, 2, 1, 6);
    evt_ImportCommand("SetBlocked", ocmd_SetBlocked, 1, 2, 1, 1);
    evt_ImportCommand("SetTransient", ocmd_SetTransient, 1, 2, 1, 1);
    evt_ImportCommand("CreateEmitter", ocmd_CreateEmitter, 1, 7, 1, 1, 1, 1, 6, 1, 1);
    evt_ImportCommand("KillEmitter", ocmd_KillEmitter, 1, 1, 1);
    evt_ImportCommand("SetEmitterAmplitude", ocmd_SetEmitterAmplitude, 1, 6, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterPhasespeed", ocmd_SetEmitterPhasespeed, 1, 6, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterDirection", ocmd_SetEmitterDirection, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterSize", ocmd_SetEmitterSize, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterVelocity", ocmd_SetEmitterVelocity, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterAcceleration", ocmd_SetEmitterAcceleration, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterRndVelocity", ocmd_SetEmitterRndVelocity, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterPlane", ocmd_SetEmitterPlane, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterTimeAndAlpha", ocmd_SetEmitterTimeAndAlpha, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterColor", ocmd_SetEmitterColor, 1, 5, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterFlags", ocmd_SetEmitterFlags, 1, 7, 1, 1, 1, 1, 1, 1, 1);
    evt_ImportCommand("SetEmitterInitFill", ocmd_SetEmitterInitFill, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterRebirthFill", ocmd_SetEmitterRebirthFill, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterTextureMode", ocmd_SetEmitterTextureMode, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterIsTrigger", ocmd_SetEmitterIsTrigger, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterTriggerOnce", ocmd_SetEmitterTriggerOnce, 1, 2, 1, 1);
    evt_ImportCommand("SetEmitterMaxTrigger", ocmd_SetEmitterMaxTrigger, 1, 2, 1, 1);
    evt_ImportCommand("TriggerEmitter", ocmd_TriggerEmitter, 1, 1, 1);
    evt_ImportCommand("SetParticlePos", ocmd_SetParticlePos, 1, 4, 1, 1, 1, 1);
    evt_ImportCommand("SelectAllTextureSets", ocmd_SelectAllTextureSets, 1, 2, 6, 1);
}

D3EMITTERCFG d3_EmitterCfg = {0};  /* 0x1468620 */
