/* ch.c -- original file name unknown: characters (ch_* error strings): animation slots and morphs, the
   action queue and its callbacks, path search and walking. Extent in GildeGold.exe: 0x402140 (right after
   the UDP code; its .data starts at 0x5f1120 with the animation path) .. 0x40b580.
   Functions are kept in their original order. Names are ours.
   cm_SpeedFactor (0x5f1290, the game speed factor) is defined here under cm's name.
   Open: not included yet (dcp headers conflicts ch HEADER): sv.h; held by calls whose argument count differs
   from the definition (read the original's pushes): sv_ReturnZero.
   unit of rea 8cef7d5b (src/game/ch.c); boundaries in decl/text.dcl */
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
#include "data_views.h"
#include "d3_engine.h"
#include "d3_fl.h"
#include "d3_interface.h"
#include "d3_io_t.h"
#include "d3_light.h"
#include "d3_snd.h"
#include "d3_tools.h"
#include "d3_vec.h"
#include "err.h"
#include "f3gm.h"
#include "mem.h"
#include "sblib.h"
#include "tr.h"
#include "d3_pose.h"
#include "vfs.h"
#include "chc.h"                      /* ch_StepSample */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define d3t_CamMoveI (*(int *)&d3t_CamMove)  /* 0x6310e8, d3t_CamMove (d3_tools.c: D3ANIM *) as the int this unit tests and clears */

char ch_AnimPath[0x100] = "x:\\gfx\\engine\\animations\\character";  /* 0x5f1120 */
int ch_LastUpdate = -1;  /* 0x5f1224 */
char ch_ShadowEnabled = 1;  /* 0x5f122c */
float cm_SpeedFactor = 1.0f;  /* 0x5f1290, 1.0 */
int ch_CurrentScriptGroup = -1;  /* 0x5f1298, the script group that goes with it; -1 = none */
int ch_MorphTimeout = -1;  /* 0x5f129c (-1), the D3ANIMSLOT.timeout of morphs in scene != 0 */
int ch_MotionTimeout = 2;  /* 0x5f12a0 (2), the same for motions */
int ch_Enabled = 1;  /* 0x5f12a4 */
int ch_DirtyX0 = 256;  /* 0x5f12a8, changed ground rectangle */
int ch_DirtyZ0 = 256;  /* 0x5f12ac */

extern unsigned char ch_PathBuf[1024];
extern CHARACTER *ch_List[CH_MAX];
extern PATHOUT ch_Path;
extern CHACTDEF ch_ActDefs[64];
extern int ch_Ground0[65536];
extern int ch_Ground1[65536];
extern CHFADE ch_Fades[16];
extern float ai_FloatZero;  /* 0x5eb788, the shared 0.0f (cm.c) */
extern const float ai_FloatOne;  /* 0x5eb780, the shared 1.0f (bio.c) */
CHACTION * ch_Queue = 0;  /* 0x631050 */
int ch_Time = 0;  /* 0x631054 */
char ch_MarkWalkErrors = 0;  /* 0x631058, when set, a failed walk animation marks the character's
                                flags bit 8; a debug switch (no writer in the image, always 0) */
int ch_SndLib = 0;  /* 0x63105c, sound library for step sounds */
int ch_StepSounds = 0;  /* 0x631060 */
D3VECTOR ch_PathOffset = {0};  /* 0x631068, added to the last path cell */
int ch_DirtyX1 = 0;  /* 0x63107c */
int ch_DirtyZ1 = 0;  /* 0x631080 */
D3SCENE * ch_Saved1 = 0;  /* 0x631088, the same for ch_Ground1 */
D3SM * ch_PathMap = 0;  /* 0x63108c */
char ch_EmptyName[1] = {0};  /* 0x631090, "" */
D3SCENE d3_Scenes[D3_MAX_SCENES] = {0}; /* 0xfd7ba0 */
unsigned char ch_PathBuf[1024] = {0};  /* 0x14686e0, the cells path_Find writes for ch_Path */
CHARACTER *ch_List[CH_MAX] = {0};  /* 0x1468ae0, the characters by id (CHARACTER.id) */
PATHOUT ch_Path = {0};  /* 0x14696e0, the result of a path search (xy = ch_PathBuf, max 0x200); nothing of the
                           original reads 0x14696ec..0x1469700 */
CHACTDEF ch_ActDefs[64] = {0};  /* 0x1469700, the action types (ch_DeclareAction); the name of type m is also the
                                   motion name ch_AttachMotion loads; ch_Open clears all 0x1300 bytes */
int ch_Ground0[65536] = {0};  /* 0x146aa00, ground types of d3_Scenes[0] */
int ch_Ground1[65536] = {0};  /* 0x14aaa20, ground types of the default scene */
CHFADE ch_Fades[16] = {0};                /* 0x14eaa20 */

/* 0x402140, creates the height map of the character's scene on first use */
struct D3HMAP *ch_GetHMap(CHARACTER *ch)
{
    int cur = d3_CurrentScene;
    unsigned int flags = 0;
    unsigned int df;
    struct D3HMAP *h;

    if (!(h = ch->scene->hmap)) {
        df = d3_SceneFlags;
        if (ch->scene != d3_DefaultScene)
            df = ((D3IFSCENE *)ch->scene)->sceneFlags;
        if (!ch->scene->dirty && df) {
            d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
            flags = df & ~2;
            d3_InflateScene(flags);
        }
        ch->scene->hmap = d3sm_Create(d3_Terrain, ((D3IFSCENE *)ch->scene)->mapSize, 2);
        h = ch->scene->hmap;
        if (ch->scene != &d3_Scenes[0] && d3_DefaultScene == ch->scene && h)
            ch_SaveGround(h);
        if (flags) {
            d3_DeflateScene(flags);
            d3if_SelectScene(cur, 1);
        }
    }
    return h;
}
/* 0x402220, the same for a scene */

struct D3HMAP *ch_GetSceneHMap(D3SCENE *scene)
{
    int cur = d3_CurrentScene;
    unsigned int flags = 0;
    unsigned int df;
    struct D3HMAP *h;

    if (!(h = scene->hmap)) {
        df = d3_SceneFlags;
        if (scene != d3_DefaultScene)
            df = ((D3IFSCENE *)scene)->sceneFlags;
        if (!scene->dirty && df) {
            d3if_SelectScene(d3t_SceneIndex(scene), 1);
            flags = df & ~2;
            d3_InflateScene(flags);
        }
        h = scene->hmap = d3sm_Create(d3_Terrain, ((D3IFSCENE *)scene)->mapSize, 2);
        if (scene != &d3_Scenes[0] && d3_DefaultScene == scene && h)
            ch_SaveGround(h);
        if (flags) {
            d3_DeflateScene(flags);
            d3if_SelectScene(cur, 1);
        }
    }
    return h;
}
/* 0x4022e0, fades characters in or out over 50 time units */

void ch_UpdateFades(void)
{
    int i;
    CHARACTER *ch;
    float a;
    D3TEXFLAGS f;

    for (i = 0; i < 16; i++) {
        ch = ch_Fades[i].ch;
        if (ch) {
            a = ((float)ch_Time - ch_Fades[i].start) * 0.02f;
            if (!ch_Fades[i].in)
                a = 1.0f - a;
            a = max(a, 0.0f);
            a = min(a, 1.0f);
            a *= 255.0f;
            memset(&f, 0, sizeof(f));
            f.b[2] &= 0xfe;
            f.b[0] = (unsigned char)a;
            if (a != 255.0f && a != 0.0f) {
                d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
                if (ch->transport)
                    d3_SetObjectAlpha(*ch->transport, ch->obj->info, f);
            }
            if (a == 0.0f || a == 255.0f) {
                ch->flags.fading = 0;
                f.b[0] = 0xff;
                d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
                if (ch->transport)
                    d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
                if (a == 0.0f)
                    ch_SetVisible(ch, 0);
                else if (a == 255.0f)
                    ch_SetVisible(ch, 1);
                ch_Fades[i].ch = 0;
            }
        }
    }
}
/* 0x402480, shows or hides a character and its objects (relights them when shown) */

void ch_SetVisible(CHARACTER *ch, int show)
{
    if (ch && ch->obj) {
        d3if_ShowTree(ch->obj, show);
        if (ch->rightHand)
            d3if_Show(ch->rightHand, show);
        if (show)
            d3_RelightObject(ch->obj);
        if (ch->transport) {
            d3if_ShowTree(*ch->transport, show);
            if (show)
                d3_RelightObject(*ch->transport);
        }
        if (ch->lowPolyBody)
            d3if_Show(ch->lowPolyBody, show);
        if (show)
            ch->flags.hidden = 0;
        else
            ch->flags.hidden = 1;
        ch_UpdateLowPoly(ch);
    } else
        err_Message("ch_SetVisible(): invalid character");
}
/* 0x402540, inflates the models of a character and its parts (each not loaded yet) */

void ch_InflateModel(CHARACTER *ch)
{
    if (!ch->obj->info)
        d3_InflateObject(ch->obj);
    if (ch->rightHand && !ch->rightHand->info)
        d3_InflateObject(ch->rightHand);
    if (ch->transport && *ch->transport && !(*ch->transport)->info)
        d3_InflateObject(*ch->transport);
    if (ch->lowPolyBody && !ch->lowPolyBody->info)
        d3_InflateObject(ch->lowPolyBody);
}
/* 0x4025c0, moves a character to a position, puts it and its transport on the ground (relight: relight them) */

void ch_SetPos(CHARACTER *ch, D3VECTOR *pos, int relight)
{
    d3_SetObjectPos(ch->obj, pos);
    if (relight)
        d3_RelightObject(ch->obj);
    ch_PlaceOnGround(ch, 0);
    if (ch->transport) {
        ch_PlaceTransport(ch, 0);
        if (relight)
            d3_RelightObject(*ch->transport);
    }
    if (ch->lowPolyBody)
        ch_UpdateLowPoly(ch);
}
/* 0x402640, puts the character at the object's position, turned the way the object faces */

void ch_PlaceAtObject(CHARACTER *ch, D3OBJECT *obj)
{
    D3VECTOR rot;
    D3VECTOR dir;
    D3VECTOR4 pos;

    if (ch) {
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        rot.x = 0.0f;
        rot.y = 0.0f;
        rot.z = 0.0f;
        d3_DirToWorld(obj, &d3_ZAxis, &dir);
        rot.y = d3_VecAngle(&d3_ZAxis, &dir);
        d3_SetObjectRot(ch->obj, &rot);
        ch_SetPos(ch, (D3VECTOR *)&pos, 1);
    }
}
/* 0x4026c0, characters in scene (-1: all scenes); all = 0 skips objects of type 1 */

int ch_Count(int scene, int all)
{
    int i, n = 0;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && (scene == -1 || ch->scene == &d3_Scenes[scene]) && (all || ch->obj->type != 1))
            n++;
    }
    return n;
}
/* 0x402720, the same, counting only characters at pos */

int ch_CountAt(int scene, int all, D3VECTOR *pos, float eps)
{
    int i, n = 0;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && (scene == -1 || ch->scene == &d3_Scenes[scene]) && (all || ch->obj->type != 1)
            && d3_VecEqual(&ch->obj->pos, pos, eps))
            n++;
    }
    return n;
}
/* 0x4027b0, gives the 3D object of every dirty character of the default scene the character's offset */

void ch_SyncObjectOffsets(void)
{
    int i;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && ch->flags.dirty && ch->scene == d3_DefaultScene)
            d3_SetObject078(ch->obj, &ch->offset);
    }
}
/* 0x402800, moves every dirty character of the default scene to its object position plus its offset and clears
   the dirty flag */

void ch_UpdatePositions(void)
{
    int i;
    CHARACTER *ch;
    D3VECTOR4 pos;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && ch->flags.dirty && ch->scene == d3_DefaultScene) {
            pos.x = ch->obj->pos.x + ch->offset.x;
            pos.y = ch->obj->pos.y + ch->offset.y;
            pos.z = ch->obj->pos.z + ch->offset.z;
            if (ch->anim) {
                if (ch->anim->name[9] != '-')
                    ch_SetPos(ch, (D3VECTOR *)&pos, 0);
            } else
                ch_SetPos(ch, (D3VECTOR *)&pos, 0);
            ch->flags.dirty = 0;
        }
    }
}
/* 0x4028a0, deletes a character: takes it out of the list, releases its objects and frees it (1: deleted) */

int ch_Delete(CHARACTER *ch)
{
    int i = 0;
    int cur;

    if (ch) {
        for (; ch_List[i] != ch && i < CH_MAX; i++)
            ;
        if (i < CH_MAX) {
            ch_List[i] = 0;
            ch->id = -1;
            ch_ClearActions(ch);
            if (ch->obj) {
                cur = -1;
                ch_ReleaseMorph(ch);
                if (d3_DefaultScene != ch->scene) {
                    cur = d3_CurrentScene;
                    d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
                }
                if (ch->onDelete)
                    ch->onDelete(ch);
                d3if_Delete(ch->obj);
                ch->obj = 0;
                if (ch->transport && *ch->transport)
                    d3if_Delete(*ch->transport);
                if (ch->lowPolyBody)
                    d3if_Delete(ch->lowPolyBody);
                if (cur != -1)
                    d3if_SelectScene(cur, 1);
            }
            if (ch->transport)
                ch_DetachTransport(ch);
            mem_Free(ch);
            return 1;
        }
    }
    return 0;
}
/* 0x4029c0, allocates an empty character and takes the first free list slot (0 when the list is full) */

CHARACTER *ch_Alloc(void)
{
    int i = 0;
    CHARACTER *ch = mem_Alloc(sizeof(CHARACTER), "ch:Create:Character");

    memset(ch, 0, sizeof(CHARACTER));
    for (; ch_List[i] && i < CH_MAX; i++)
        ;
    if (i < CH_MAX) {
        ch_List[i] = ch;
        ch->id = i;
        return ch;
    }
    mem_Free(ch);
    err_Message("ch_Create:Too many characters!");
    return 0;
}
/* 0x402a30, allocates an empty character for the given id and puts it in that list slot (0 when the slot is taken) */

CHARACTER *ch_AllocAt(int id)
{
    CHARACTER *ch = mem_Alloc(sizeof(CHARACTER), "ch:Create:Character");

    memset(ch, 0, sizeof(CHARACTER));
    if (ch_List[id])
        return 0;
    ch_List[id] = ch;
    ch->id = id;
    return ch;
}
/* 0x402a70, finds the character whose object's name matches name (case-insensitive), 0 when none */

CHARACTER *ch_FindByName(char *name)
{
    int i;

    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i] && !_strcmpi(ch_List[i]->obj->name, name))
            break;
    if (i < CH_MAX)
        return ch_List[i];
    return 0;
}
/* 0x402ac0, finds the character whose object is obj, 0 when none */

CHARACTER *ch_FindByObject(D3OBJECT *obj)
{
    int i;

    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i] && ch_List[i]->obj == obj)
            return ch_List[i];
    return 0;
}
/* 0x402af0, 0/1 create the low-poly bodies, 2 deletes them */

void ch_SetLowPoly(unsigned char mode)
{
    int cur = d3_CurrentScene;
    int i;
    CHARACTER *ch;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch) {
            d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
            if (mode == 2) {
                if (ch->lowPolyBody) {
                    d3t_ReleaseObjectTextures(ch->lowPolyBody);
                    d3if_Delete(ch->lowPolyBody);
                    ch->lowPolyBody = 0;
                }
            } else if ((mode == 0 || mode == 1) && !ch->lowPolyBody) {
                sv_ReturnZero(ch);
                ch_UpdateLowPoly(ch);
            }
        }
    }
    ch_LowPoly = 2 - mode;
    d3if_SelectScene(cur, 1);
}
/* 0x402ba0, ch_Updatelowpoly: shows the body or the low-poly body and keeps the latter in sync
   @deep Read in full against 0x402ba0..0x403066. Meaning fix: in the scene branch the low-poly body is
   hidden only on the near path (the original's 0x402dd8 sits in the near block, after
   d3if_Show/d3_RelightObject of the body; the far path goes from d3_SetObjectRot straight to the animation
   part); D3OBJ_UNK200 replaced by D3OBJECT.owner. All statements line up (ours 361 vs 368). Left: block
   placement and load CSE - the original lays the near block (0x402db8..0x402dee) after the far block and
   reaches it with 'jnp' from the fcomp, ours inlines it after 'jp far' (the condition written either way,
   as || of negations or && with near first, and with or without the (float) cast compiles the same or
   worse); in the animation tail the original re-reads anim->slots[0].data for each of the three ifs ('+
   mov esi,[eax+0x84]' / '+ mov edx,[eax+0x84]' and re-tests the slot, 0x402f72..0x402fa3) where ours keeps
   the first load; the frame and every call/argument are the same. (references: M8 at +0x250: ours refers
   to d3_SetObjectRot (0x43b860), the original's field to 0x4471b0 (d3if_Show), explained: the M8 at +0x250
   (d3_SetObjectRot | d3if_Show) is the misalignment of the near block: both sides call d3_SetObjectRot at
   the end of the far block and d3if_Show(lowPolyBody, 0) at the end of the near block (asmdiff --symbols)) */

void ch_UpdateLowPoly(CHARACTER *ch)
{
    float dx, dy, dz;
    D3ANIMCTL *anim;
    D3ANIMSLOT *s;
    CHANIMFLAGS f;
    char name[0x100];
    char path[0x100];
    char msg[0x200];

    if (!ch->lowPolyBody)
        return;
    if (ch_LowPoly) {
        ch->lowPolyBody->owner = ch->obj->owner;
        ch->lowPolyBody->pickId = ch->obj->pickId;
        if (ch->flags.lowPoly) {
            if (ch->obj->type != 1)
                d3if_Show(ch->obj, 0);
            if (ch->lowPolyBody->type == 1) {
                d3if_Show(ch->lowPolyBody, 1);
                d3_RelightObject(ch->lowPolyBody);
            }
            return;
        }
        if (ch->scene == &d3_Scenes[0]) {
            if (ch_LowPoly != 1 || !d3_Camera
                || (dx = d3_Camera->pos.x - ch->obj->pos.x,
                    dy = d3_Camera->pos.y - ch->obj->pos.y,
                    dz = d3_Camera->pos.z - ch->obj->pos.z,
                    !((float)sqrt(dx * dx + dy * dy + dz * dz) < 1500.0f))) {
                if (ch->obj->type != 1)
                    d3if_Show(ch->obj, 0);
                if ((ch->group == ch_CurrentGroup && ch->scriptGroup == ch_CurrentScriptGroup) || !ch->scriptGroup) {
                    if (ch->lowPolyBody->type == 1 && !ch->flags.hidden) {
                        d3if_Show(ch->lowPolyBody, 1);
                        d3_RelightObject(ch->lowPolyBody);
                    }
                } else if (ch->lowPolyBody->type != 1)
                    d3if_Show(ch->lowPolyBody, 0);
                if (ch->lowPolyBody->type != 1 && ch->lowPolyBody->info
                    && !d3_VecEqual(&ch->lowPolyBody->pos, &ch->obj->pos, 2.0f))
                    d3_SetObjectPos(ch->lowPolyBody, &ch->obj->pos);
                if (ch->obj->info && !d3_VecEqual(&ch->lowPolyBody->rotation, &ch->obj->rotation, 0.05f))
                    d3_SetObjectRot(ch->lowPolyBody, &ch->obj->rotation);
            } else {
                if (ch->obj->type == 1 && !ch->flags.hidden) {
                    d3if_Show(ch->obj, 1);
                    d3_RelightObject(ch->obj);
                }
                if (ch->lowPolyBody->type != 1)
                    d3if_Show(ch->lowPolyBody, 0);
            }
            anim = D3OBJ_ANIM(ch->lowPolyBody);
            if (!anim || ch->flags.lowPolyAnimFailed)
                return;
            s = ch->slot;
            if (s && !anim->slots[0].data) {
                if (!strstr(s->data->name, "gehen") && !strstr(ch->slot->data->name, "karren_ziehen"))
                    return;
                sprintf(path, "lowpolycharacter/%s/%s_%s_LOW.baf", ch->lowname, "gehen", ch->lowname);
                sprintf(name, "%s_%s_LOW.baf", "gehen", ch->lowname);
                vfs_ToSlashes(name);
                memset(&f, 0, sizeof(f));
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 8;
                f.u.c.b0 = 0;
                f.b = 0;
                f.a = 0;
                if (!d3_FindAnim(name))
                    d3_LoadAnimStream(path, name, 1);
                s = (D3ANIMSLOT *)d3anim_StartAnimCh(ch->lowPolyBody->material->animCtl, name, f);
                if (!s) {
                    sprintf(msg, "ch_Updatelowpoly(): Could not attach ani: %s", path);
                    ch->flags.lowPolyAnimFailed = 1;
                    return;
                }
                D3OBJ_ANIM(ch->lowPolyBody)->slots[0].frame = ch->slot->frame;
                s->timeout = -1;
                return;
            }
            if (!s && anim->slots[0].data) {
                d3t_ReleaseObjectTextures(ch->lowPolyBody);
                return;
            }
            if (s && anim->slots[0].data)
                anim->slots[0].speed = s->speed;
            return;
        }
        if ((ch->group == ch_CurrentGroup && ch->scriptGroup == ch_CurrentScriptGroup) || !ch->scriptGroup) {
            if (ch->obj->type == 1 && !ch->flags.hidden) {
                d3if_Show(ch->obj, 1);
                d3_RelightObject(ch->obj);
            }
        } else if (ch->obj->type != 1)
            d3if_Show(ch->obj, 0);
    } else if (ch->obj->type == 1 && !ch->flags.hidden) {
        d3if_Show(ch->obj, 1);
        d3_RelightObject(ch->obj);
    }
    if (ch->lowPolyBody->type != 1)
        d3if_Show(ch->lowPolyBody, 0);
}
/* 0x403070, name is <...>_<body>[_<variant>] */

int ch_CreateMesh(CHARACTER *ch, char *name, D3VECTOR *pos)
{
    D3VECTOR4 rot = {0.0f, 0.0f, 0.0f, 0.0f};
    char buf[0x100];
    char msg[0x200];
    char *p, *q;

    if (!ch)
        return 0;
    ch->obj = d3_CreateObject(0, pos, (D3VECTOR *)&rot, name);
    ch->scriptId = -1;
    if (!ch->obj) {
        sprintf(msg, "ch_CreateMesh(): Could not load 3D-Character: %s", name);
        return 0;
    }
    strcpy(buf, name);
    if (p = strrchr(buf, '_')) {
        p++;
        if (q = strrchr(p, '_'))
            *q = 0;
        strcpy(ch->body, p);
        p[-1] = 0;
        strcpy(ch->fullname, buf);
    } else
        ch->body[0] = ch_EmptyName[0];
    ch->scene = d3_DefaultScene;
    ch->obj->stateFlags |= 0xc;
    ch->obj->objectFlags &= ~2;
    ch->obj->kind = 2;
    ch->obj->mask = 1;
    ch->obj->flags &= ~4;
    D3_MESH(ch->obj)->lightScale = 1.8f;
    ch->walkSpeed = 1.0f;
    ch->group = -1;
    ch->scriptGroup = -1;
    strcpy(ch->name, name);
    ch->obj->objectFlags |= 4;
    if (!d3t_SceneIndex(ch->scene))
        D3OBJ_BITS(ch->obj, 0x211)->b3 = ch_ShadowEnabled;
    else
        D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
    ch->obj->pickId = 0x3000000;
    ch_PlaceOnGround(ch, 0);
    d3_RelightObject(ch->obj);
    d3_UpdateObject(ch->obj, 1);
    ch->kind = 1;
    if (strstr(ch->name, "RATTE") || strstr(ch->name, "HUND") || strstr(ch->name, "KATZE")
        || strstr(ch->name, "PFERD")) {
        ch->kind = 2;
        if (strstr(ch->name, "RATTE")) {
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
            ch->obj->objectFlags &= ~4;
        }
        if (strstr(ch->name, "PFERD")) {
            ch->kind |= 8;
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
        }
    }
    ch_PreloadAnims(ch, 2, "bewegung/gehen", "stehen/stehen_newnoise");
    if (ch_LowPoly && ch->lowPolyBody) {
        ch_LoadLowAnims(ch, 1, "gehen");
        ch_UpdateLowPoly(ch);
    }
    ch->flags.idlePending = 1;
    return 1;
}
/* 0x403390, allocates a character and builds its mesh from name at pos (deletes it, 0 when the mesh fails) */

CHARACTER *ch_Create(char *name, D3VECTOR *pos)
{
    CHARACTER *ch = ch_Alloc();

    if (!ch_CreateMesh(ch, name, pos)) {
        ch_Delete(ch);
        return 0;
    }
    return ch;
}
/* 0x4033c0, ch_MoveCharacter2Universe */

int ch_MoveToScene(CHARACTER *ch, D3SCENE *scene)
{
    char msg[0x100];

    if (ch && ch->scene && scene && d3if_MoveObject(ch->obj, ch->scene, scene)) {
        d3_SetObject078(ch->obj, &d3_Origin);
        if (ch->transport)
            d3if_MoveObject(*ch->transport, ch->scene, scene);
        if (ch->lowPolyBody)
            d3if_MoveObject(ch->lowPolyBody, ch->scene, scene);
        ch->scene = scene;
        if (!d3t_SceneIndex(scene))
            D3OBJ_BITS(ch->obj, 0x211)->b3 = ch_ShadowEnabled;
        else
            D3OBJ_BITS(ch->obj, 0x211)->b3 = 0;
        if (scene == d3_DefaultScene && !ch->obj->info) {
            sprintf(msg, "ch_MoveCharacter2Universe(): character deflated, will be inflated:%s", ch->fullname);
            ch_InflateModel(ch);
        }
        return 1;
    }
    return 0;
}
/* 0x4034e0, puts obj 70 units behind the character */

CHTRANSPORT *ch_AttachTransport(CHARACTER *ch, D3OBJECT *obj)
{
    CHTRANSPORT *t = mem_Alloc(sizeof(CHTRANSPORT), "ch:ch_AttachTransport");
    D3VECTOR4 pos;

    pos.x = ai_FloatZero * ch->obj->matrix._11 + ai_FloatZero * ch->obj->matrix._21 - 70.0f * ch->obj->matrix._31 + ch->obj->matrix._41;
    pos.y = ai_FloatZero * ch->obj->matrix._12 + ai_FloatZero * ch->obj->matrix._22 - 70.0f * ch->obj->matrix._32 + ch->obj->matrix._42;
    pos.z = ai_FloatZero * ch->obj->matrix._13 + ai_FloatZero * ch->obj->matrix._23 - 70.0f * ch->obj->matrix._33 + ch->obj->matrix._43;
    d3_SetObjectPos(obj, (D3VECTOR *)&pos);
    d3_SetObjectRot(obj, &ch->obj->rotation);
    obj->flags &= ~4;
    obj->stateFlags = obj->stateFlags & ~8 | 4;
    obj->objectFlags &= ~2;
    ch->transport = (D3OBJECT **)t;
    t->reserved14 = 0;
    t->obj = obj;
    return t;
}
/* 0x4035f0, frees the transport record of ch and clears its transport */

void ch_DetachTransport(CHARACTER *ch)
{
    if (ch->transport) {
        mem_Free(ch->transport);
        ch->transport = 0;
    }
}
/* 0x403620, drags the transport behind the character and puts it on the ground */

void ch_PlaceTransport(CHARACTER *ch, char now)
{
    D3OBJECT *body = ch->obj;
    D3SM *hmap = (D3SM *)ch_GetHMap(ch);
    D3OBJECT *obj;
    int z, x;
    float h2;
    D3VECTOR4 v;
    D3VECTOR4 d;
    float len, k, dx, dy, dz, a;
    D3VECTOR rot;
    int t;
    float h;

    if (!ch->transport)
        return;
    obj = CH_TRANSPORT(ch)->obj;
    if (d3_VecEqual(&body->pos, &CH_TRANSPORT(ch)->pos, 0.01f) && !now)
        return;
    v.x = ch->obj->posOffset.x + ch->obj->pos.x;
    v.y = ch->obj->posOffset.y + ch->obj->pos.y;
    v.z = ch->obj->posOffset.z + ch->obj->pos.z;
    dx = v.x - CH_TRANSPORT(ch)->pos.x;
    dy = v.y - CH_TRANSPORT(ch)->pos.y;
    dz = v.z - CH_TRANSPORT(ch)->pos.z;
    len = (float)sqrt(dx * dx + dy * dy + dz * dz);
    v.x = 0.0f * obj->matrix._11 + 0.0f * obj->matrix._21 + len * obj->matrix._31;
    v.y = 0.0f * obj->matrix._12 + 0.0f * obj->matrix._22 + len * obj->matrix._32;
    v.z = 0.0f * obj->matrix._13 + 0.0f * obj->matrix._23 + len * obj->matrix._33;
    d.x = v.x + obj->pos.x;
    d.y = v.y + obj->pos.y;
    d.z = v.z + obj->pos.z;
    v.x = body->pos.x - d.x;
    v.y = body->pos.y - d.y;
    v.z = body->pos.z - d.z;
    k = (float)(70.0f / (float)sqrt(v.x * v.x + v.y * v.y + v.z * v.z));
    d.x = k * v.x;
    d.y = k * v.y;
    d.z = k * v.z;
    v.x = body->pos.x - d.x;
    v.y = body->pos.y - d.y;
    v.z = body->pos.z - d.z;
    d3_SetObjectPos(obj, (D3VECTOR *)&v);
    a = -(6.2831855f - d3_YawTo(obj, &body->pos));
    rot.x = obj->rotation.x;
    rot.y = obj->rotation.y;
    rot.z = obj->rotation.z;
    rot.y += a;
    d3_SetObjectRot(obj, &rot);
    if (d3_GetHeightAt(hmap, &obj->pos, &x, &z, &h)) {
        t = hmap->entries[hmap->size * z + x].type;
        if ((t == 0 || t == 0xd) && !ch->scene->map)
            goto done;
        if (ch->scene->map)
            map_Pick(ch->scene->map, &obj->pos, 1, &x, &z, &h2);
        if (t == 0 || t == 0xd || !(h2 - h < -5.0))
            h = h2;
        if (!d3t_SceneIndex(ch->scene))
            h += 3.0f;
        if (now)
            obj->pos.y = h;
        else
            obj->pos.y = (h - obj->pos.y) * 0.25f + obj->pos.y;
    }
    d3_SetObjectPos(obj, &obj->pos);
done:
    CH_TRANSPORT(ch)->pos.x = body->pos.x;
    CH_TRANSPORT(ch)->pos.y = body->pos.y;
    CH_TRANSPORT(ch)->pos.z = body->pos.z;
}
/* 0x403950, loads and attaches action motion <motion> of the character's body (0 when it cannot be attached) */

D3ANIMSLOT *ch_AttachMotion(CHARACTER *ch, int motion)
{
    CHANIMFLAGS f;
    D3ANIMSLOT *s;
    char name[0x110];
    char path[0x110];
    char msg[0x200];

    sprintf(path, "character/%s/%s_%s.baf", ch->body, ch_ActDefs[motion].name, ch->body);
    sprintf(name, "%s_%s", ch_ActDefs[motion].name, ch->body);
    vfs_ToSlashes(name);
    if (path[0]) {
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.b8 = 1;
        f.u.c.b1 = 0x18;
        f.u.c.b0 = 0;
        f.b = 0;
        f.a = 1;
        if (!d3_FindAnim(name))
            d3_LoadAnimStream(path, name, 1);
        s = (D3ANIMSLOT *)d3anim_StartAnimCh(ch->obj->material->animCtl, name, f);
        if (!s) {
            sprintf(msg, "ch_AttachMotion(): Could not attach ani: %s", path);
            return 0;
        }
    }
    if (!d3t_SceneIndex(ch->scene))
        s->timeout = -1;
    else
        s->timeout = ch_MotionTimeout;
    return s;
}
/* 0x403ab0, 1 when the current animation has reached its last frame (last, or nframes when last is -1),
   or when bit 5 of its flags byte is set; 1 also when the character has neither a slot nor a next anim
   @deep behaviour: close (8 differing asm lines / 14 bytes), only the zero-extension of the last return: original xor ecx,ecx; mov cl,[eax+0x6d]; mov eax,ecx; shr eax,5; and eax,1, ours mov al,[eax+0x6d]; shr eax,5; and eax,1 (same value, the upper bits are dead before the shift). spell.py menu: only dup-tail 403af2, an artifact of the 3 extra original instructions - the if/else and both early returns match instruction for instruction (23 equal), so the dup-tail prescriptions (statements in both arms / pull after the branch / swap the branch polarity) do not apply. Width spellings tried, one edit + one build each, all identical: unsigned int local, unsigned short local, (unsigned int) cast, (unsigned short) cast, b[1] / 32, ((unsigned)b[1] >> 5) % 2, unsigned char bitfield view flags6d : 8, (b[1] & 0x20) ? 1 : 0. Earlier tries: (unsigned)/(int)/(unsigned long) casts, (b & 0x20) >> 5, a local, byte and int bitfield overlays, a ternary. VC6 canonicalises every spelling to the trimmed AL load; the original kept the zero-extended byte. Q6 description added, Q1/Q2/Q3/Q4/Q5/Q7 pass. */

int ch_AnimDone(CHARACTER *ch)
{
    D3ANIMSLOT *s = ch->slot;
    int n;

    if (!ch->slot && !ch->next)
        return 1;
    if (!s)
        return 0;
    n = s->data->last != -1 ? s->data->last : s->data->nframes;
    if (s->frame >= n)
        return 1;
    return s->flags.b[1] >> 5 & 1;
}
/* 0x403b00, loads the animation the character switches to next (ch_CheckAniMorph) */

D3ANIMDATA *ch_SetNextAnim(CHARACTER *ch, char *anim, char loops)
{
    int walk = 0;
    D3ANIMDATA *a;
    char name[0x110];
    char path[0x110];

    if (ch->next)
        return 0;
    vfs_ToSlashes(anim);
    sprintf(path, "character/%s/%s_%s.baf", ch->body, anim, ch->body);
    sprintf(name, "%s_%s", anim, ch->body);
    vfs_ToSlashes(name);
    a = (D3ANIMDATA *)d3_FindAnim(name);
    if (!_strcmpi(anim, "bewegung/gehen") || !_strcmpi(anim, "bewegung/karren_ziehen"))
        walk = 1;
    if (!a)
        ch->next = d3_LoadAnimStream(path, name, walk);
    else
        ch->next = a;
    if (!ch->next)
        return 0;
    ch->nextloops = loops;
    ch->next->refs++;
    return ch->next;
}
/* 0x403c10, ends the current animation, blending from its frame into a morph */

void ch_StopAnim(CHARACTER *ch)
{
    D3OBJECT *obj = ch->obj;
    CHANIMFLAGS f;
    char name[0x60];
    D3ANIMSLOT *s;

    if (ch->slot) {
        sprintf(name, "morph_%i", ch);
        if (!d3_FindAnim(name)) {
            if (obj->info && d3anim_CreateMorphAnim(D3OBJ_ANIM(obj)->mesh, ch->slot->data,
                                                      &ch->slot->data->frames[ch->slot->frame], 0, 0,
                                                      D3OBJ_ANIM(obj)->mesh, name, 4)) {
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 0x18;
                f.b = 0;
                f.a = 0;
                ch->morph = (D3ANIMSLOT *)d3anim_StartAnimCh(obj->material->animCtl, name, f);
                if (ch->morph) {
                    if (!d3t_SceneIndex(ch->scene))
                        ch->morph->timeout = -1;
                    else
                        ch->morph->timeout = ch_MorphTimeout;
                }
            }
            s = ch->slot;
            if (!s->data->relative && (s->flags.b[4] & 3))
                d3anim_ApplyRootMotion(obj, s, 0, s->frame, 3);
            d3_ReleaseTexture(obj->material->animCtl, s->data->name);
            ch->slot = 0;
        }
    }
}
/* 0x403d60, releases the character's morph animation (its texture and anim data) and clears it */

void ch_ReleaseMorph(CHARACTER *ch)
{
    char name[0x60];
    D3ANIMDATA *a;

    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        a = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(a, 1);
        ch->morph = 0;
    }
}
/* 0x403dc0, ends a finished animation or morph and starts the next animation, blending into it */

void ch_CheckAniMorph(CHARACTER *ch)
{
    int released = 0;
    D3OBJECT *obj;
    CHANIMFLAGS f;
    D3ANIMSLOT *s;
    char name[0x60];
    char msg[0x200];
    D3ANIMDATA *a;
    D3ANIMFRAME *fr, *nf;
    int n, i;
    char loops;

    s = ch->slot;
    obj = ch->obj;
    if (s && (s->flags.b[1] & 0x20) && !(s->flags.b[8] & 8) && !ch->flags.holdAnim) {
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(obj, s, 0, s->frame, 3);
        d3_ReleaseTexture(obj->material->animCtl, s->data->name);
        ch->slot = 0;
    }
    if (ch->morph) {
        if (!(ch->morph->flags.b[1] & 0x20))
            return;
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(obj->material->animCtl, name);
        a = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(a, 1);
        ch->morph = 0;
        released = 1;
    }
    if (!ch->next)
        return;
    if (!ch->morph && ch->slot) {
        s = ch->slot;
        i = ch->next->first != -1 ? ch->next->first : 0;
        sprintf(name, "morph_%i", ch);
        a = ch->slot->data;
        fr = &a->frames[ch->slot->frame];
        nf = &ch->next->frames[i];
        n = nf->time - fr->time;
        n = (n + a->frames[a->nframes - 1].time) / 2;
        n = (int)(n + 0.5f);
        if (obj->info) {
            if (d3anim_CreateMorphAnim(D3OBJ_ANIM(obj)->mesh, a, fr, ch->next, nf, 0, name, n)) {
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 0x18;
                f.b = 0;
                f.a = 0;
                ch->morph = (D3ANIMSLOT *)d3anim_StartAnimCh(obj->material->animCtl, name, f);
                if (ch->morph) {
                    if (!d3t_SceneIndex(ch->scene))
                        ch->morph->timeout = -1;
                    else
                        ch->morph->timeout = ch_MorphTimeout;
                }
            } else
                ch->morph = 0;
        }
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(obj, s, 0, s->frame, 3);
        d3_ReleaseTexture(obj->material->animCtl, s->data->name);
        if (D3OBJ_ANIM(obj) && D3OBJ_ANIM(obj)->slots[0].data && D3OBJ_ANIM(obj)->slots[1].data)
            err_Message("ch_CheckAniMorph(): huston we got a problem...");
        ch->slot = 0;
        return;
    }
    if (!ch->morph && !ch->slot) {
        memset(&f, 0, sizeof(f));
        f.u.all = 0;
        f.u.c.b0 = 1;
        f.b8 = 1;
        loops = ch->nextloops;
        if (loops >= 1)
            f.u.c.b0 = loops;
        f.u.c.b1 = loops != 1 ? 8 : 0x18;
        ch->next->refs--;
        f.b = 0;
        f.a = 0;
        if (!d3_FindAnim(ch->next->name)) {
            sprintf(msg, "character/%s/%s.baf", ch->body, ch->next->name, ch->body);
            d3_LoadAnimStream(msg, ch->next->name, 0);
        }
        ch->slot = (D3ANIMSLOT *)d3anim_StartAnimCh(obj->material->animCtl, ch->next->name, f);
        ch->lastActionFrame = -1;
        if (ch->slot) {
            if (!d3t_SceneIndex(ch->scene))
                ch->slot->timeout = -1;
            else
                ch->slot->timeout = ch_MotionTimeout;
            if ((n = ch->next->first) != -1 && released) {
                ch->slot->frame = n;
                ch->slot->nextFrame = n + 1;
                d3anim_PoseBones(ch->obj);
            }
        } else
            sprintf(msg, "ch_CheckAniMorph(): Could not attach CurrentAnimStream: %s", ch->next->name);
        ch->next = 0;
    }
}
/* 0x404200, loads the named animations of the character once (and releases them again) */

void ch_PreloadAnims(CHARACTER *ch, int n, ...)
{
    va_list ap;
    int i;
    char *anim;
    char walk;
    CHANIMFLAGS f;
    char name[0x100];
    char path[0x100];

    va_start(ap, n);
    for (i = 0; i < n; i++) {
        anim = va_arg(ap, char *);
        if (*anim) {
            walk = 0;
            if (!_strcmpi(anim, "bewegung/gehen") || !_strcmpi(anim, "bewegung/karren_ziehen"))
                walk = 1;
            sprintf(path, "character/%s/%s_%s.baf", ch->body, anim, ch->body);
            sprintf(name, "%s_%s", anim, ch->body);
            vfs_ToSlashes(name);
            if (!d3_FindAnim(name)) {
                d3_LoadAnimStream(path, name, walk);
                memset(&f, 0, sizeof(f));
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 0x18;
                f.u.c.b0 = 0;
                f.b = 0;
                f.a = 1;
                d3anim_StartAnimCh(ch->obj->material->animCtl, name, f);
                d3_ReleaseTexture(ch->obj->material->animCtl, name);
            }
        }
    }
    va_end(ap);
}

/* ---- 0x404360..0x405810 (worker ch2) ---- */

/* 0x404360, loads the low-poly animations name... of the model attached at unk22c @deep behaviour: only the 12-byte D3ANIMFLAGS init placement is left (code hunk at 36): the original schedules the init stores between the d3_FindAnim(buf) call and its r test, ours before the call, so the zero lives in eax vs the original's ecx and the two result tests are test vs cmp eax,ebx (all other instructions equal). Tried this session: init after the call (kept), before the call, split around it (memset+f.words[0] before, byte ops after) - VC6 hoists the whole block in every source position; memset vs three explicit dword stores; explicit ((int *)&f)[k] vs the kept union view (unchanged code). spell menu 4043d8 dup-tail prescriptions (both arms / pull after the branch / swap polarity) do not apply: the moved instruction is the call, no duplicated tail. Needs scheduling/allocation variants (try.py, slotmap). Readability done: union view removed the Q4 raw *(int *)&f access, no Q failure left. */
void ch_LoadLowAnims(CHARACTER *ch, int n, ...)
{
    char buf[256];
    char file[256];
    union {
        D3ANIMFLAGS anim;
        int words[3];
    } f;
    char *name;
    int i, r;
    va_list args;

    va_start(args, n);
    for (i = 0; i < n; i++) {
        name = va_arg(args, char *);
        if (*name) {
            sprintf(file, "lowpolycharacter/%s/%s_%s_LOW.baf", CH_MODEL2(ch), name, CH_MODEL2(ch));
            sprintf(buf, "%s_%s_LOW.baf", name, CH_MODEL2(ch));
            vfs_ToSlashes(buf);
            r = d3_FindAnim(buf);
            memset(&f, 0, sizeof(f));
            f.words[0] = 0;
            f.anim.b[1] = 8;
            f.words[1] &= ~3;
            f.anim.b[8] |= 2;
            f.anim.b[0] = 0;
            if (r || d3_LoadAnimFile(file, buf, 1)) {
                d3_AttachAnimF(ch->lowPolyBody->material->animCtl, buf, f.anim);
                d3_ReleaseTexture(ch->lowPolyBody->material->animCtl, buf);
            }
        }
    }
}
/* 0x4044a0, starts the animation <model>_<name> on the character @deep behaviour: one code hunk left at 404502 (34 instr), the 12-byte D3ANIMFLAGS init; same statements as the original. Original builds b[1] as (loop&1)<<3|0x10 (mov al,[loop]; and al,1; shl al,3; or al,0x10), ours factors it to ((loop&1)|2)<<3, and the original keeps a second dword0 zero store and pushes buf at the block start; the rest (40454c/404562/4045ca/4045ec/404608) is regs-class. Tried this session, one build each: two f.b[1]|= lines (kept, byte-identical to the session baseline), single 0x10|(loop&1)<<3, reversed order, + 0x10, * 8 (gives the original's and/shl/or order but re-schedules the block and raises the byte diff 198->257), extra f.words[0]=0 duplicate dword0 store (->255); union dword view vs ((int*)&f)[1] is byte-neutral and kept. spell 404502 dup-tail entries (both arms / pull after branch / swap polarity) do not describe it: the moved items are stores and the call's push, no duplicated tail. Needs vargen/register+slot scheduling variants. Readability done: union view removed the raw dword cast, CH_SLOT/CH_FRAME replaced by ch->slot/ch->lastActionFrame, no Q failure left. */

int ch_AttachAni(CHARACTER *ch, char *name, unsigned char loop)
{
    char msg[512];
    char file[272];
    char buf[272];
    union {
        D3ANIMFLAGS anim;
        int words[3];
    } f;
    D3ANIMSLOT *s;

    vfs_ToSlashes(name);
    sprintf(file, "character/%s/%s_%s.baf", CH_MODEL(ch), name, CH_MODEL(ch));
    sprintf(buf, "%s_%s", name, CH_MODEL(ch));
    vfs_ToSlashes(buf);
    if (*name) {
        memset(&f, 0, sizeof(f));
        f.anim.b[0] = 0;
        f.anim.b[1] |= 0x10;
        f.anim.b[1] |= (loop & 1) << 3;
        f.words[1] |= 3;
        f.anim.b[8] |= 2;
        if (!d3_FindAnim(buf)) {
            if (strcmp(name, "bewegung/gehen") && strcmp(name, "bewegung/karren_ziehen"))
                d3_LoadAnimFile(file, buf, 0);
            else
                d3_LoadAnimFile(file, buf, 1);
        }
        s = (D3ANIMSLOT *)d3_AttachAnimF(ch->obj->material->animCtl, buf, f.anim);
        if (!s) {
            sprintf(msg, "ch_AttachAni(): Could not attach ani: %s", name);
            return 0;
        }
        if (!d3t_SceneIndex(ch->scene))
            s->timeout = -1;
        else
            s->timeout = ch_MotionTimeout;
    }
    ch->slot = s;
    ch->lastActionFrame = -1;
    return (int)s;
}
/* 0x404640, action callback: plays the action's animation */

int ch_ActAnim(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3ANIMSLOT *s;

    if (!ch->slot && !a->count) {
        ch->slot = ch_AttachMotion(ch, a->type);
        return a->started;
    }
    if (ch->slot && (ch->slot->flags.b[1] & 0x20)) {
        s = ch->slot;
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(ch->obj, s, 0, s->frame, 3);
        if (!(ch->flags.bytes.lo & 2))
            d3_ReleaseTexture(a->ch->obj->material->animCtl, ch->slot->data->name);
        ch->slot = 0;
        return -1;
    }
    if (ch->slot && a->stop) {
        s = ch->slot;
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(ch->obj, s, 0, s->frame, 3);
        if (!(ch->flags.bytes.lo & 2))
            d3_ReleaseTexture(a->ch->obj->material->animCtl, ch->slot->data->name);
        ch->slot = 0;
        return -1;
    }
    if (!ch->slot)
        return -1;
    return a->started;
}
/* 0x404760, returns the first free entry of the action queue ch_Queue, 0 when all are in use */

CHACTION *ch_GetFreeQueueEntry(void)
{
    int i;

    for (i = 0; i < CH_QUEUE; )
        if (!ch_Queue[i++].func)
            break;
    if (i >= CH_QUEUE)
        return 0;
    return &ch_Queue[i - 1];
}
/* 0x4047a0, unlinks the action from its character's queue and frees it */

int ch_RemoveAction(CHACTION *a)
{
    CHACTION *prev, *next;

    if (!a)
        return 0;
    if (!a->next && a->ch && a == CH_ACTIONS(a->ch)) {
        CH_ACTIONS(a->ch) = 0;
        memset(a, 0, sizeof(*a));
        return 1;
    }
    prev = a->prev;
    if (prev)
        prev->next = a->next;
    next = a->next;
    if (next)
        next->prev = prev;
    if (!prev && a->ch)
        CH_ACTIONS(a->ch) = next;
    memset(a, 0, sizeof(*a));
    return 1;
}
/* 0x404810, removes every action of the character's queue, freeing the path of a walk action */

int ch_ClearActions(CHARACTER *ch)
{
    CHACTION *a;

    while (a = CH_ACTIONS(ch)) {
        if (a->type == '-' && a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
    }
    return 1;
}
/* 0x404860, consistency checks of the queue */

void ch_CheckQueue(CHARACTER *ch, CHACTION *a)
{
    CHACTION *p;

    if (a != CH_ACTIONS(ch)) {
        if (!a->prev)
            __asm int 3
        if (a->next && a->next->prev != a)
            __asm int 3
    }
    for (p = CH_ACTIONS(ch); p; p = p->next)
        if (p->next && p->next->ch != ch)
            __asm int 3
}
/* 0x4048b0, queues action type before the action "before"; the action's arguments follow */

CHACTION *ch_InsertAction(CHARACTER *ch, CHACTION *before, unsigned int type, ...)
{
    int arg0;
    CHACTION *a, *prev;
    int i, x, z, s;
    va_list args;

    if (!before)
        return 0;
    if (!ch_ActDefs[type].func) {
        arg0 = type;
        type = 0;
    }
    a = ch_GetFreeQueueEntry();
    if (!a) {
        err_Message("ch_InsertAction(): Could not ch_GetFreeQueueEntry()");
        return 0;
    }
    prev = before->prev;
    before->prev = a;
    a->prev = prev;
    a->next = before;
    if (prev)
        prev->next = a;
    else
        CH_ACTIONS(before->ch) = a;
    a->func = ch_ActDefs[type].func;
    a->ch = ch;
    a->type = type;
    a->count = 0;
    va_start(args, type);
    for (i = 0; i < ch_ActDefs[a->type].nargs; i++)
        a->args[i] = va_arg(args, int);
    if (type == 0x33) {
        if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
            err_Message("ch_InsertAction(): move2universe: invalid combination of ids and universeindex");
            ch_RemoveAction(a);
            return 0;
        }
    } else if (type == 0x2d) {
        if (a->args[0] < 0 || a->args[0] > 256 || a->args[1] < 0 || a->args[1] > 256 || a->args[2] < 0
            || a->args[2] > 64 || d3_Scenes[a->args[2]].dirty) {
            ch_RemoveAction(a);
            return 0;
        }
        if (ch->scene->hmap && (a->args[0] > D3IF_SCENE(a->args[2]).mapSize
                                || a->args[1] > D3IF_SCENE(a->args[2]).mapSize)) {
            ch_RemoveAction(before);
            return 0;
        }
    } else if (type == 0)
        a->args[0] = arg0;
    a->started = 0;
    ch_CheckQueue(ch, a);
    return a;
}
/* 0x404a80, puts the character on the ground (force: also when the ground type is 0 or 0xd); returns the ground
   type (0: none)
   @deep behaviour: close (5 differing instructions), one code hunk. The original materialises the h==0 return value with a separate xor eax,eax at 404a98 (scheduled before test esi,esi, je to 404b7d past the second xor) and the d3_GetHeightAt==0 return with a second xor eax,eax at 404b7b *before* pop edi/pop esi; ours merges both failure paths into one shared xor placed after pop edi/pop esi, so 404a98 is missing and 404b7b moves. Same statements and returns (checked against 404a80..404b83). Tried this session: baseline nested if (h) {...} + final return 0 (kept), inner return 0 + final return 0, if (!h) return 0 guard (25 differing lines, layout flips cold), if (!d3_GetHeightAt) return 0 guard, else return 0 forms, add-lea/clamp untouched; spell 404b79 push-pop prescriptions both tried (early guard vs one final return; the callee-saved crossing is not expressible in C). Earlier tries: early return before/after obj=ch->obj, else return 0, else type=0; return type;, int ret=0 local. Needs the tail-merge/vargen toolbox. */

int ch_PlaceOnGround(CHARACTER *ch, int force)
{
    D3SM *h;
    D3OBJECT *obj;
    D3VECTOR4 pos;
    int x, z;
    float y;
    int type;

    h = (D3SM *)ch_GetHMap(ch);
    obj = ch->obj;
    if (h) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y)) {
            type = h->entries[h->size * z + x].type;
            if (force || type != 0 && type != 0xd) {
                if (ch->scene->map)
                    map_Pick(ch->scene->map, (D3VECTOR *)&pos, 1, &x, &z, &y);
                if (!d3t_SceneIndex(ch->scene))
                    pos.y = y + 3.0f;
                else
                    pos.y = y;
                d3_SetObjectPos(obj, (D3VECTOR *)&pos);
            }
            return type;
        }
    }
    return 0;
}
/* 0x404b90, runs the first action of the queue */

int ch_UpdateAction(CHARACTER *ch)
{
    CHACTION *a;
    D3ANIMSLOT *s;

    if (!CH_ACTIONS(ch))
        return 0;
    a = CH_ACTIONS(ch);
    if (a && a->ch && a->func) {
        a->func(a);
        a->count++;
        if (a->func2 && (s = ch->slot) && !ch->next && CH_FRAME(ch) != s->flags.b[0]
            && s->frame >= a->func2Frame) {
            a->func2(a);
            CH_FRAME(ch) = ch->slot->flags.b[0];
        }
    }
    return 1;
}
/* 0x404c20, pauses (pause non-zero) or resumes the character's current animation slot */

void ch_PauseAnim(CHARACTER *ch, int pause)
{
    if (CH_ACTIONS(ch) && ch->slot) {
        if (pause) {
            d3anim_PauseAnim(ch->obj->material->animCtl, ch->slot->data->name);
            ch->flags.all |= 4;
        } else {
            d3anim_PlayAnim(ch->obj->material->animCtl, ch->slot->data->name, 1);
            ch->flags.all &= ~4;
        }
    }
}
/* 0x404c80, camera position and angles for view 0..3 relative to the character */

void ch_GetCamera(CHARACTER *ch, int view, D3VECTOR4 *pos, D3VECTOR *rot)
{
    if (!ch)
        return;
    switch (view) {
    case 0:
        pos->x = -3.0f;
        pos->y = 63.0f;
        pos->z = 36.0f;
        rot->y = 3.1415927f;
        rot->x = 0.0f;
        rot->z = 0.0f;
        break;
    case 1:
        pos->x = -10.0f;
        pos->y = 63.0f;
        pos->z = -14.0f;
        rot->x = 0.08726647f;
        rot->y = 0.0f;
        rot->z = 0.0f;
        break;
    case 2:
        pos->x = 10.0f;
        pos->y = 63.0f;
        pos->z = -14.0f;
        rot->x = 0.08726647f;
        rot->y = 0.0f;
        rot->z = 0.0f;
        break;
    case 3:
        pos->x = 0.0f;
        pos->y = 65.0f;
        pos->z = 0.0f;
        rot->y = 0.0f;
        rot->x = 0.0f;
        rot->z = 0.0f;
        break;
    }
    if (ch->flags.bytes.lo & 0x10)
        pos->y -= 15.0;
    d3_LocalToWorld(ch->obj, (D3VECTOR *)pos, (D3VECTOR *)pos);
    rot->x += ch->obj->rotation.x;
    rot->y += ch->obj->rotation.y;
    rot->z += ch->obj->rotation.z;
}
/* 0x404d80, puts d3_Camera at the character's view 0..3 (ch_GetCamera) */

void ch_SetCamera(CHARACTER *ch, int view)
{
    D3VECTOR rot;
    D3VECTOR4 pos;

    ch_GetCamera(ch, view, &pos, &rot);
    d3_SetObjectRot(d3_Camera, &rot);
    d3_SetObjectPos(d3_Camera, (D3VECTOR *)&pos);
}
/* 0x404dd0, moves d3_Camera over time to the character's view 0..3 (ch_GetCamera) */

void ch_MoveCamera(CHARACTER *ch, int view, int time)
{
    D3VECTOR rot;
    D3VECTOR4 pos;

    if (ch) {
        ch_GetCamera(ch, view, &pos, &rot);
        d3anim_FreePath(d3_Camera);
        d3t_CamMoving = 0;
        d3t_CamMoveI = 0;
        d3t_PlaceObjectV(d3_Camera, (D3VECTOR *)&pos, &rot, time, 0x28);
    }
}
/* 0x404e40, a random number in 0..n-1 (0 when n is 0), the pick of ch_FindDummy */

int ch_Random(unsigned short n)
{
    int r = 0;

    if (n)
        r = rand() % n;
    return r;
}
/* 0x404e60, saves the ground types of a scene's height map */

void ch_SaveGround(struct D3HMAP *hmap)
{
    D3SM *h = (D3SM *)hmap;
    int *buf;
    int i, j;

    if (d3_Scenes[0].hmap == hmap)
        buf = ch_Ground0;
    else if (d3_DefaultScene->hmap == hmap)
        buf = ch_Ground1;
    else
        return;
    for (i = 0; i < h->size; i++)
        for (j = 0; j < h->size; j++)
            buf[h->size * j + i] = h->entries[h->size * j + i].type;
}
/* 0x404ed0, writes the saved ground types of the changed rectangle back into the height map */

void ch_RestoreGround(struct D3HMAP *hmap)
{
    D3SM *h = (D3SM *)hmap;
    int *buf;
    int i, j;

    if (d3_Scenes[0].hmap == hmap)
        buf = ch_Ground0;
    else if (d3_DefaultScene->hmap == hmap)
        buf = ch_Ground1;
    else
        return;
    ch_DirtyX0 = max(0, ch_DirtyX0);
    if (h->size < ch_DirtyX1)
        ch_DirtyX1 = h->size;
    ch_DirtyZ0 = max(0, ch_DirtyZ0);
    if (h->size < ch_DirtyZ1)
        ch_DirtyZ1 = h->size;
    for (i = ch_DirtyX0; i < ch_DirtyX1; i++)
        for (j = ch_DirtyZ0; j < ch_DirtyZ1; j++)
            h->entries[h->size * j + i].type = buf[h->size * j + i];
    ch_DirtyX0 = h->size;
    ch_DirtyX1 = 0;
    ch_DirtyZ0 = h->size;
    ch_DirtyZ1 = 0;
}
/* 0x404fc0, sets the ground type of a diamond of radius r around (x, z) */

int ch_SetGroundDiamond(int x, int z, unsigned char scene, int type, int r)
{
    D3SM *h;
    int i = 0;
    int d, xx, zz, x1, z1;

    h = (D3SM *)d3_Scenes[scene].hmap;
    if (d3_Scenes[0].hmap != (struct D3HMAP *)h && d3_DefaultScene->hmap != (struct D3HMAP *)h)
        return 0;
    if (!h)
        return 0;
    for (; i < h->size; i++) {
        if (i >= r)
            break;
        z1 = min(h->size - 2, z + i);
        for (zz = max(1, z - i); zz <= z1; zz++) {
            d = i - abs(z - zz);
            x1 = min(h->size - 2, x + d);
            for (xx = max(1, x - d); xx <= x1; xx++)
                if (zz >= 1 && zz < h->size - 1 && xx >= 1 && xx < h->size - 1)
                    h->entries[h->size * zz + xx].type = type;
        }
    }
    if (x - r < ch_DirtyX0)
        ch_DirtyX0 = x - r;
    if (x + r > ch_DirtyX1)
        ch_DirtyX1 = x + r + 1;
    if (z - r < ch_DirtyZ0)
        ch_DirtyZ0 = z - r;
    if (z + r > ch_DirtyZ1)
        ch_DirtyZ1 = z + r + 1;
    return 1;
}
/* 0x405160, a random free map vertex near pos */

int ch_RandomNear(D3VECTOR *pos, D3VECTOR *out)
{
    D3SM *h = (D3SM *)d3_DefaultScene->hmap;
    int x, z;
    float y;
    int nx, nz;

    if (!h)
        return 0;
    d3_GetHeightAt(h, pos, &x, &z, &y);
    nx = util_RandomU(10) - 4 + x;
    nz = util_RandomU(10) - 4 + z;
    if (h->size - 2 < nx)
        nx = h->size - 2;
    if (nx < 2)
        nx = 2;
    if (h->size - 2 < nz)
        nz = h->size - 2;
    if (nz < 2)
        nz = 2;
    if (ch_FindFreeCell(h, pos, pos, nx, nz, &nx, &nz, 600, 0))
        d3sm_GetVertex(h, nx, nz, out);
    else
        d3sm_GetVertex(h, x, z, out);
    return 1;
}
/* 0x405260, up to 16 other characters of the same scene within dist of ch */

int ch_FindNear(CHARACTER *ch, CHARACTER **out, float dist)
{
    int i, n = 0;
    CHARACTER *o;

    for (i = 0; i < CH_MAX; i++) {
        if (n >= 16)
            break;
        o = ch_List[i];
        if (o && ch != o && ch->scene == o->scene && ch->group == o->group
            && (!CH_ACTIONS(o) || CH_ACTIONS(o)->type != '-')
            && (o->obj->type != 1 || o->lowPolyBody && o->lowPolyBody->type != 1)
            && d3_VecEqual(&ch->obj->pos, &o->obj->pos, dist))
            out[n++] = ch_List[i];
    }
    return n;
}
/* 0x405310, per frame: runs the characters' actions */

int ch_Update(void)
{
    int i;
    CHACTION *act;
    D3SM *h;
    int x, z;
    float y;
    D3VECTOR4 pos;
    CHARACTER *found[32];

    ch_Time = d3_Time;
    ch_StepSounds = 0;
    if (!ch_Enabled)
        return 0;
    if (ch_Saved0 != d3_DefaultScene || ch_Saved1 != d3_DefaultScene) {
        if (d3_Scenes[0].hmap && ch_Saved0 != d3_DefaultScene) {
            ch_SaveGround(d3_Scenes[0].hmap);
            ch_Saved0 = d3_DefaultScene;
        }
        if (d3_DefaultScene && d3_CurrentScene && d3_DefaultScene->hmap && ch_Saved1 != d3_DefaultScene) {
            ch_SaveGround(d3_DefaultScene->hmap);
            ch_Saved1 = d3_DefaultScene;
        }
    }
    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i] && !(ch_List[i]->flags.all & 4)) {
            act = CH_ACTIONS(ch_List[i]);
            ch_CheckAniMorph(ch_List[i]);
            if (act) {
                if ((ch_List[i]->flags.all & 0x2000) && ch_List[i]->slot) {
                    d3_ReleaseTexture(ch_List[i]->obj->material->animCtl, ch_List[i]->slot->data->name);
                    ch_List[i]->slot = 0;
                    ch_List[i]->flags.all &= ~0x2000;
                }
                ch_UpdateAction(ch_List[i]);
            } else if (ch_List[i]->scene == d3_DefaultScene && !(ch_List[i]->flags.all & 0x32)
                       && !(ch_List[i]->flags.all & 0x8000) && !ch_List[i]->transport) {
                if (ch_FindNear(ch_List[i], found, 20.0f)) {
                    h = (D3SM *)ch_GetHMap(ch_List[i]);
                    ch_RandomNear(&ch_List[i]->obj->pos, (D3VECTOR *)&pos);
                    if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y))
                        ch_AddAction(ch_List[i], 0x2d, x, z, d3_CurrentScene);
                }
            } else
                ch_List[i]->flags.all &= ~8;
            if (ch_List[i]->transport)
                ch_PlaceTransport(ch_List[i], 0);
            if (ch_List[i]->lowPolyBody)
                ch_UpdateLowPoly(ch_List[i]);
            if (!act && d3_DefaultScene == ch_List[i]->scene && d3_CurrentScene && !ch_List[i]->slot
                && (ch_List[i]->flags.all & 0x1000) && !(ch_List[i]->flags.all & 2)
                && !(D3_MESH(ch_List[i]->obj)->flags & 0x40)) {
                if (ch_List[i]->flags.all & 0x10) {
                    ch_AttachAni(ch_List[i], "sitzend/sitz_newnoise", 0);
                    if (ch_List[i]->slot) {
                        ch_List[i]->slot->flags.b[0] = 0;
                        ch_List[i]->slot->flags.b[1] &= ~0x10;
                        ch_List[i]->flags.all |= 0x2000;
                    } else
                        ch_List[i]->flags.all &= ~0x1000;
                } else {
                    ch_AttachAni(ch_List[i], "stehen/stehen_newnoise", 0);
                    if (ch_List[i]->slot) {
                        ch_List[i]->slot->flags.b[0] = 0;
                        ch_List[i]->slot->flags.b[1] &= ~0x10;
                        ch_List[i]->flags.all |= 0x2000;
                    } else
                        ch_List[i]->flags.all &= ~0x1000;
                }
            }
            if (ch_List[i] && ch_List[i]->scene != ch_List[i]->obj->scene)
                err_Message("ch_Update(): invalid c->universe!");
        }
    }
    ch_UpdateFades();
    ch_LastUpdate = d3_Time;
    return 1;
}
/* 0x405610, stops the current action and drops the queued ones */

int ch_StopActions(CHARACTER *ch)
{
    CHACTION *a;

    if (!ch)
        return 0;
    a = CH_ACTIONS(ch);
    if (!a)
        return 0;
    a->stop = 1;
    while (a->next)
        ch_RemoveAction(a->next);
    if (ch->flags.all & 0x10)
        ch_StandUp(ch, "sitzend/aufstehen");
    return 1;
}
/* 0x405670, declares an action type once: its function, name and argument count */

int ch_DeclareAction(int id, CHACTFUNC func, char flags, char *name, int nargs)
{
    if (id > 63)
        return 0;
    if (ch_ActDefs[id].func) {
        err_Message("ch_DeclareAction:Function already declared!");
        return 0;
    }
    ch_ActDefs[id].func = func;
    ch_ActDefs[id].flags = flags;
    ch_ActDefs[id].nargs = nargs;
    strcpy(ch_ActDefs[id].name, name);
    return 1;
}
/* 0x4056e0, action callback: plays the animation a->u.name a->args[0] times (-1: until stopped) */

void ch_ActPlay(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3ANIMSLOT *s;

    if (a->stop && (s = ch->slot) && s->flags.b[0] > 2)
        s->flags.b[0] = 2;
    if (!a->count) {
        if (a->args[0] == -1)
            ch->flags.all |= 2;
        ch_SetNextAnim(ch, a->u.name, max(1, a->args[0]));
    } else if (a->stop || a->args[0] != -1) {
        if (a->args[0] == -1)
            ch->flags.all &= ~2;
        if (ch_AnimDone(ch)) {
            ch_RemoveAction(a);
            ch_CheckAniMorph(ch);
        }
    }
    if (ch->slot && a->speed != 0.0f)
        ch->slot->speed = a->speed;
}
/* 0x4057a0, queues the animation name n times */

int ch_QueueAnim(CHARACTER *ch, char *name, int n)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->args[0] = n;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActPlay;
    a->type = 0x2e;
    a->speed = 1.0f;
    a->next = 0;
    a->started = 0;
    strcpy(a->u.name, name);
    return 1;
}

/* ---- 0x405810..0x406e40 (worker ch3) ---- */

/* 0x405810, queues the animation name n times at the given speed */
CHACTION *ch_QueueAnimSpeed(CHARACTER *ch, char *name, int n, float speed)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->args[0] = n;
    a->func = (CHACTFUNC)ch_ActPlay;
    a->type = 0x2e;
    a->speed = 1.0f;
    a->next = 0;
    a->started = 0;
    strcpy(a->u.name, name);
    a->speed = speed;
    return a;
}
/* 0x405870, action callback: sits down with the animation a->u.name */

void ch_ActSitDown(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    int cur;

    if (!a->count) {
        if (a->u.name[0] && !D3OBJ_ANIM(ch->obj)->slots[0].data) {
            ch->flags.all |= 2;
            if (a->scriptAction.flags & 1) {
                ch->sitSlot = ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, a->u.name, 1);
                d3anim_SetFrame(ch->obj, ch->slot, -1);
            } else
                ch->sitSlot = ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, a->u.name, 1);
        }
        if (ch->slot) {
            ch->slot->flags.b[0] = 1;
            ((int *)&ch->slot->flags)[1] &= ~3;
            ch->slot->flags.b[1] |= 0x10;
        }
    } else if (ch_ActAnim(a) == -1) {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        a->ch->flags.all |= 0x10;
        a->ch->flags.all &= ~2;
        d3if_SelectScene(cur, 1);
        ch_RemoveAction(a);
    }
}
/* 0x405970, queues the sit-down action (type 0x2f) with the animation name */

CHACTION *ch_SitDown(CHARACTER *ch, char *name)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActSitDown;
    a->type = 0x2f;
    a->next = 0;
    a->started = 0;
    strcpy(a->u.name, name);
    return a;
}
/* 0x4059c0, drops the sitting animation */

void ch_ReleaseSit(CHARACTER *ch)
{
    if (ch->flags.all & 0x10) {
        if (ch->sitSlot) {
            if (!ch->obj->info)
                ch_InflateModel(ch);
            if (ch->sitSlot->data)
                d3_ReleaseTexture(ch->obj->material->animCtl, ch->sitSlot->data->name);
            ch->sitSlot = 0;
        }
        ch->flags.all &= ~0x10;
    }
}
/* 0x405a20, action callback: stands up with the animation a->u.name */

void ch_ActStandUp(CHACTION *a)
{
    CHARACTER *ch = a->ch;

    if ((ch->slot || ch->morph) && !(a->started & 1)) {
        a->count = -1;
        return;
    }
    a->started |= 1;
    if (!a->count) {
        if (!(ch->flags.all & 0x10)) {
            ch_RemoveAction(a);
            return;
        }
        ch->slot = (D3ANIMSLOT *)ch_AttachAni(ch, a->u.name, 1);
        if (!ch->slot) {
            ch_RemoveAction(a);
            return;
        }
        ch->slot->flags.b[0] = 1;
        ((int *)&ch->slot->flags)[1] &= ~3;
        ch->slot->flags.b[1] |= 0x10;
    } else if (ch_ActAnim(a) == -1) {
        ch_ReleaseSit(ch);
        ch_RemoveAction(a);
    }
}
/* 0x405ad0, queues the stand-up action (type 0x30) for a sitting character; 0 if it is not seated */

int ch_StandUp(CHARACTER *ch, char *name)
{
    CHACTION *a;

    if (!(ch->flags.all & 0x10))
        return 0;
    a = ch_NewAction(ch);
    if (!a)
        return 0;
    a->ch = ch;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActStandUp;
    a->type = 0x30;
    a->next = 0;
    a->started = 0;
    strcpy(a->u.name, name);
    return 1;
}
/* 0x405b30, action callback: walks to the gate object name2 in the action's target scene, then moves there */

void ch_ActUsingGate(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    CHACTION *b;
    int cur, scene;
    D3OBJECT *obj;
    struct D3HMAP *h;
    int x, z;
    float y;
    D3VECTOR4 pos;
    char buf[0x200];

    if (a->count)
        return;
    cur = d3_CurrentScene;
    scene = d3t_SceneIndex(ch->scene);
    if (scene == CH_ACTOBJ(a)->data[0]) {
        ch_RemoveAction(a);
        return;
    }
    d3if_SelectScene(scene, 1);
    obj = d3_FindObject(0, 0x120, 0, CH_ACTOBJ(a)->name2);
    if (obj) {
        h = d3_Scenes[scene].hmap;
        d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
        if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y))
            ch_InsertAction(ch, a, 0x2d, x, z, scene);
        else
            sprintf(buf, "UsingGate: d3sm_3D2SuperMap() failed... scene:%s   object:%s", ch->scene, obj);
    }
    d3if_SelectScene(cur, 1);
    if (ch->obj->type != 1)
        ch_InsertAction(ch, a, 0x38, 0);
    b = ch_InsertAction(ch, a, 0x33, CH_ACTOBJ(a)->data[0], CH_ACTOBJ(a)->data[1], CH_ACTOBJ(a)->data[2]);
    strcpy(b->u.name, a->u.name);
    ch_InsertAction(ch, b, 0x38, 1);
    ch_RemoveAction(a);
}
/* 0x405cb0, queues a walk to the gate object and the move to the scene/group/scriptGroup it carries */

CHACTION *ch_UseGate(CHARACTER *ch, char *name, char *gate, int scene, int group, int scriptGroup)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->func = (CHACTFUNC)ch_ActUsingGate;
    a->type = 0x34;
    strcpy(CH_ACTOBJ(a)->name, name);
    strcpy(CH_ACTOBJ(a)->name2, gate);
    CH_ACTOBJ(a)->data[0] = scene;
    CH_ACTOBJ(a)->data[1] = group;
    if (CH_ACTOBJ(a)->data[2] == -1)
        err_Message("ch_UseGate: room_id == ID_NONE");
    CH_ACTOBJ(a)->data[2] = scriptGroup;
    return a;
}
/* 0x405d40, action callback: plays a->u.name and takes the object name2 at frame args[0] */

void ch_ActTakingObject(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    char buf[0x100];

    if (!a->count) {
        if (a->u.name[0])
            CH_ACTOBJ(a)->data[0] = (int)ch_SetNextAnim(ch, a->u.name, 1);
        else {
            ch_SetHandObject(ch, a->args[2], CH_ACTOBJ(a)->name2);
            ch_RemoveAction(a);
            return;
        }
    }
    if (a->args[0] != 99999999) {
        if (ch->next)
            return;
        if (ch->slot && ch->slot->frame >= a->args[0]) {
            ch_SetHandObject(a->ch, a->args[2], CH_ACTOBJ(a)->name2);
            a->args[0] = 99999999;
        }
    }
    if (ch->next)
        return;
    if (!ch_AnimDone(ch))
        return;
    if (a->args[0] != 99999999) {
        sprintf(buf, "ch_TakingObject(): object not taken yet (should be in frame:%i)!", a->args[0]);
        ch_SetHandObject(a->ch, a->args[2], CH_ACTOBJ(a)->name2);
    }
    if (ch->next)
        sprintf(buf, "ch_TakingObject(): c->NextAnim still valid %s!\n", ch->next->name);
    ch_CheckAniMorph(ch);
    ch_RemoveAction(a);
}
/* 0x405e70, queues the take-object action (type 0x31): the animation name, and the object's model name */

int ch_TakeObject(CHARACTER *ch, char *name, D3OBJECT *obj, int frame)
{
    CHACTION *a = ch_NewAction(ch);

    if (a) {
        a->ch = ch;
        a->args[0] = frame;
        a->count = 0;
        a->func = (CHACTFUNC)ch_ActTakingObject;
        a->type = 0x31;
        a->next = 0;
        a->started = 0;
        a->args[1] = 0;
        a->args[2] = 2;
        strcpy(a->u.name, name);
        if (obj && obj->material && obj->material->name) {
            strcpy(CH_ACTOBJ(a)->name2, obj->material->name);
            return 1;
        }
        ch_RemoveAction(a);
    }
    return 0;
}
/* 0x405f10, queues the take-object action (type 0x31) for the left hand (args[2] = 1): the animation name and the object's model name */

int ch_TakeObject1(CHARACTER *ch, char *name, D3OBJECT *obj, int frame)
{
    CHACTION *a = ch_NewAction(ch);

    if (a) {
        a->ch = ch;
        a->args[0] = frame;
        a->count = 0;
        a->func = (CHACTFUNC)ch_ActTakingObject;
        a->type = 0x31;
        a->next = 0;
        a->started = 0;
        a->args[1] = 0;
        a->args[2] = 1;
        strcpy(a->u.name, name);
        if (obj && obj->material && obj->material->name) {
            strcpy(CH_ACTOBJ(a)->name2, obj->material->name);
            return 1;
        }
        ch_RemoveAction(a);
    }
    return 0;
}
/* 0x405fb0, action callback: plays a->u.name and drops the object at frame args[0] @deep behaviour: the only meaning-class hunk left is 406050 (7 instr): the original reloads a->ch ([esi+0x14]) into ecx for the ch_SetHandObject call inside the frame check, ours forwards the entry local ch (edi); the other two ch_SetHandObject sites reload a->ch in both sides. Same statements and call arguments (read against 405fb0..4060d7). Tried this session: source already passes a->ch (spell reload prescription 'drop the local, re-read the source expression'; the local is required for ch->next/ch->slot/ch_AnimDone), an explicit ch = a->ch; before the call (optimised away, byte-identical 85/272), and the spell push-pop/dup-tail hunks at 406083/4060c3 are regs/order consequences of the same load. Previous attempts: shared unk080 re-test, count re-test, raw offsets, inline helper, first flag, slot local, no ch local. Needs vargen/alias forms. Readability advanced: CHARACTER.unk064 named rightHand (0x64, ch_SetHandObject hand 2, ch_SetVisible), CH_SLOT/CH_UNK080 replaced by ch->slot/ch->next, Q4 clean. Q2: CH_ACTOBJ.data[3] is the action's int payload now, its per-action meanings in its comment (gate: scene / group / scriptGroup; take / drop: the animation it started). */

void ch_ActDroppingObject(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    char buf[0x100];

    if (!a->count) {
        if (!ch->rightHand) {
            ch_RemoveAction(a);
            return;
        }
        if (a->u.name[0])
            CH_ACTOBJ(a)->data[1] = (int)ch_SetNextAnim(ch, a->u.name, 1);
        if (ch->next)
            return;
        ch_SetHandObject(a->ch, a->args[2], 0);
        ch_RemoveAction(a);
        return;
    }
    if (ch->next)
        return;
    if (ch->slot && ch->slot->frame >= a->args[0]) {
        ch_SetHandObject(a->ch, a->args[2], 0);
        a->args[0] = 99999999;
    }
    if (ch->next)
        return;
    if (!ch_AnimDone(ch))
        return;
    if (a->args[0] != 99999999) {
        ch_SetHandObject(a->ch, a->args[2], 0);
        sprintf(buf, "ch_DroppingObject(): object not dropped yet (should be in frame:%i)!", a->args[0]);
    }
    ch_CheckAniMorph(ch);
    ch_RemoveAction(a);
}
/* 0x4060d0, queues the drop-object action (type 0x32) for the right hand (args[2] = 2): the animation name and the object's model name */

int ch_DropObject(CHARACTER *ch, char *name, char *obj, int frame)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->ch = ch;
    a->args[0] = frame;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActDroppingObject;
    a->type = 0x32;
    a->next = 0;
    a->started = 0;
    a->args[1] = 0;
    a->args[2] = 2;
    if (name)
        strcpy(a->u.name, name);
    else
        a->u.name[0] = 0;
    if (obj)
        strcpy(CH_ACTOBJ(a)->name2, obj);
    else
        CH_ACTOBJ(a)->name2[0] = 0;
    return 1;
}
/* 0x406170, queues the drop-object action (type 0x32) for the left hand (args[2] = 1): the animation name and the object's model name */

int ch_DropObject1(CHARACTER *ch, char *name, char *obj, int frame)
{
    CHACTION *a = ch_NewAction(ch);

    if (!a)
        return 0;
    a->ch = ch;
    a->args[0] = frame;
    a->count = 0;
    a->func = (CHACTFUNC)ch_ActDroppingObject;
    a->type = 0x32;
    a->next = 0;
    a->started = 0;
    a->args[1] = 0;
    a->args[2] = 1;
    if (name)
        strcpy(a->u.name, name);
    else
        a->u.name[0] = 0;
    if (obj)
        strcpy(CH_ACTOBJ(a)->name2, obj);
    else
        CH_ACTOBJ(a)->name2[0] = 0;
    return 1;
}
/* 0x406210, action callback: turns towards the object args[0] */

void ch_ActTurnTo(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3OBJECT *obj = (D3OBJECT *)a->args[0];
    float ang;
    D3VECTOR rot, from, to;
    D3VECTOR4 target;

    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&target);
    rot.y = (ang = d3_YawTo(ch->obj, (D3VECTOR *)&target));
    rot.x = 0.0f;
    rot.z = 0.0f;
    d3_UnwrapAngles(&rot, &ch->obj->rotation, &from, &to);
    rot.x += ch->obj->rotation.x;
    rot.y += ch->obj->rotation.y;
    rot.z += ch->obj->rotation.z;
    ch_InsertAction(ch, a, 7, (int)(ang * 57.295776f), 0);
    ch_RemoveAction(a);
}
/* 0x4062d0, action callback: plays a->u.name once */

void ch_ActPlayOnce(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3ANIMSLOT *s;

    if (!a->count) {
        ch->slot = (D3ANIMSLOT *)ch_AttachAni(ch, a->u.name, 1);
        if (ch->slot) {
            ch->flags.all |= 2;
            ch->slot->flags.b[0] = 1;
            ((int *)&ch->slot->flags)[1] &= ~3;
            ch->slot->flags.b[1] |= 0x10;
        } else
            ch_RemoveAction(a);
    } else if ((s = ch->slot) && (s->flags.b[1] & 0x20))
        ch_RemoveAction(a);
}
/* 0x406340, d3_ForEachObject callback: collects the dummy objects characters walk to */

char ch_FindDummyCB(D3OBJECT *obj, CHOBJLIST *list)
{
    char names[15][0x40] = {
        "dummy_WAIT",
        "dummy_FEGEN1",
        "dummy_FEGEN2",
        "dummy_FEGEN3",
        "dummy_FEGEN4",
        "dummy_PAUSE",
        "dummy_PAUSE2",
        "dummy_AUSGANG"     /* sic, no comma */
        "dummy_LAUFEN1",
        "dummy_LAUFEN2",
        "dummy_LAUFEN3",
        "dummy_LAUFEN4",
        "dummy_STEHPLATZ_01",
        "dummy_STEHPLATZ_02",
        "dummy_STEHPLATZ_03"
    };
    int i;

    for (i = 0; i < 15; i++) {
        if (names[i][0] && strstr(obj->name, names[i])) {
            list->objs[list->n] = obj;
            list->n++;
            break;
        }
    }
    return list->n < 32;
}
/* 0x406790, a random dummy object of the default scene */

D3OBJECT *ch_FindDummy(void)
{
    CHOBJLIST list;

    memset(&list, 0, sizeof(list));
    d3_ForEachObject(d3_DefaultScene, 0, 0x100, (D3OBJECTCB)ch_FindDummyCB, &list);
    if (!list.n)
        return 0;
    return list.objs[(unsigned short)ch_Random((unsigned short)list.n)];
}
/* 0x4067f0, action callback: moves the character to scene args[0] (ids args[1], args[2]) */

void ch_ActMove2Universe(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    int cur;
    D3OBJECT *obj;
    D3VECTOR4 pos;
    char name[0x80];

    if (a->args[0] < 0) {
        err_Message("ch_Move2Universe(): Invalid universe (<0)!");
        ch_RemoveAction(a);
        return;
    }
    if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
        if (!a->args[0] && a->args[1] != -1)
            err_Message("ch_Move2Universe(): invalid combination of ids and universeindex (Action->Data[0]==0 && Action->Data[1]!=CH_ID_NONE)");
        else
            err_Message("ch_Move2Universe(): invalid combination of ids and universeindex (Action->Data[0]!=0 && Action->Data[1]==CH_ID_NONE)");
        ch_RemoveAction(a);
        return;
    }
    if (!ch_MoveToScene(ch, &d3_Scenes[a->args[0]])) {
        err_Message("ch_Move2Universe(): Could not change universe...");
        ch_RemoveAction(a);
        return;
    }
    cur = d3_CurrentScene;
    d3if_SelectScene(a->args[0], 1);
    if (!a->args[0] && ch_CurrentGroup == ch->group)
        d3if_SelectTextureSet(ch->obj, ch->obj->material->animCtl, 0, 0);
    else if (ch_CurrentGroup == ch->group)
        d3if_SelectTextureSet(ch->obj, ch->obj->material->animCtl, ch->texSet, 1);
    ch_ReleaseSit(ch);
    if (a->move.hasPos)
        ch_SetPos(ch, &a->move.pos, 0);
    else {
        if (a->u.name[0])
            strcpy(name, a->u.name);
        else
            strcpy(name, "dummy_EINGANG");
        obj = d3_FindObject(0, 0x100, 0, name);
        if (!obj)
            obj = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (!obj)
            obj = d3_FindObject(0, 0x100, 0, "dummy_EINGANG");
        if (obj) {
            d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
            ch_SetPos(ch, (D3VECTOR *)&pos, 0);
        }
    }
    ch->group = a->args[1];
    ch->scriptGroup = a->args[2];
    ch->pos.x = 0;
    ch->pos.y = 0;
    ch->pos.z = 0;
    ch_SetVisible(ch, 1);
    if (cur == a->args[0]) {
        if (ch->group != -1 && ch_CurrentGroup != ch->group || ch_CurrentScriptGroup != -1 && ch_CurrentScriptGroup != ch->scriptGroup)
            ch_SetVisible(ch, 0);
    }
    if (a->next && a->next->type == 0x38)
        ch_SetVisible(ch, 0);
    d3if_SelectScene(cur, 1);
    ch_RemoveAction(a);
}
/* 0x406a70, keeps the character on the ground; returns the ground type under it */

int ch_FollowGround(CHACTION *a)
{
    D3SM *h;
    CHARACTER *ch;
    D3OBJECT *obj;
    int type;
    int x, z;
    float y, y2;
    D3VECTOR4 pos;

    h = (D3SM *)ch_GetHMap(a->ch);
    ch = a->ch;
    type = 0;
    obj = ch->obj;
    if (h) {
        pos.x = obj->pos.x;
        pos.y = obj->pos.y;
        pos.z = obj->pos.z;
        if (d3_GetHeightAt(h, (D3VECTOR *)&pos, &x, &z, &y)) {
            type = h->entries[h->size * z + x].type;
            if (type && type != 13 || ch->scene->map) {
                obj = ch->obj;
                if (ch->scene->map) {
                    map_Pick(ch->scene->map, &obj->pos, 1, &x, &z, &y2);
                    if (!type || type == 13 || !(y2 - y < -5.0))
                        y = y2;
                }
                if (!d3t_SceneIndex(ch->scene))
                    y += 3.0f;
                obj->pos.y += (y - obj->pos.y) * 0.25f;
                d3_SetObjectPos(obj, &obj->pos);
            }
        }
    }
    return type;
}
/* 0x406bc0, every step-th point of path (always the first and the last) into out; returns the count */

int ch_ThinPath(CHARACTER *ch, PATHOUT *path, CHPATHPT *out, CHWALKDEF *w)
{
    int n = 0;
    int i = 1;
    int step;
    int x, z;

    if (ch && (ch->kind & 0x10))
        step = 2;
    else {
        step = (int)(12.0f / w->speed + 0.5f);
        if (step < 1)
            step = 1;
    }
    for (; i < path->count; i += step) {
        x = path->xy[i * 2];
        z = path->xy[i * 2 + 1];
        out[n].x = x;
        out[n].z = z;
        n++;
    }
    if (i != path->count - 1) {
        out[n].x = path->xy[(path->count - 1) * 2];
        out[n].z = path->xy[(path->count - 1) * 2 + 1];
        n++;
    }
    return n;
}
/* 0x406c60, creates the object name in a hand of the character (name 0: removes it) */

void ch_SetHandObject(CHARACTER *ch, int hand, char *name)
{
    D3OBJECT **slot = 0;
    int cur;
    D3VECTOR4 pos = {0.0f, 0.0f, 0.0f, 0.0f};
    D3VECTOR rot = {0.0f, 0.0f, 0.0f};

    if (hand == 1)
        slot = CH_HAND(ch, 0x68);
    else if (hand == 2)
        slot = CH_HAND(ch, 0x64);
    else if (hand == 3)
        slot = CH_HAND(ch, 0x6c);
    if (name) {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        if (*slot) {
            d3if_Delete(*slot);
            *slot = 0;
        }
        *slot = d3_CreateObject((int)ch->obj, (D3VECTOR *)&pos, &rot, name);
        if (!*slot) {
            d3if_SelectScene(cur, 1);
            return;
        }
        (*slot)->stateFlags |= 0xc;
        (*slot)->objectFlags &= ~2;
        (*slot)->flags &= ~4;
        (*slot)->kind = 5;
        d3_RelightObject(*slot);
        if (hand == 1)
            d3_AttachToBone(*slot, "d3_LeftHand");
        else if (hand == 2)
            d3_AttachToBone(*slot, "d3_RightHand");
        else if (hand == 3)
            d3_AttachToBone(*slot, "d3_Head");
        d3if_SelectScene(cur, 1);
    } else {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        if (*slot) {
            d3if_Delete(*slot);
            *slot = 0;
        }
        d3if_SelectScene(cur, 1);
    }
}

/* ---- 0x406e40..0x408690 (worker ch4) ---- */

/* 0x406e40, plays an animation action once and removes it when it fails */
void ch_ActAnimOnce(CHACTION *a)
{
    if (ch_ActAnim(a) == -1)
        ch_RemoveAction(a);
}
/* 0x406e60, turn action: args[0] degrees (args[1]: relative to the z axis), plays "dreh_90_*" and turns @deep behaviour: far draft, 125 differing instructions / 586 bytes. Same flow; the code hunks are the two D3ANIMFLAGS/key blocks (the original builds the flags block twice, one per branch of if (ch->slot), as a load/or/or/store after a dword zero store with dword 1 enregistered, and loads turn.z/turn.y/turn.x and key[0].time before the stores) and the key[] stores. Read against 406e60..407200. Last-bytes leads tried this session: the 406f5f branch spell (jp vs and eax,0x100;je) both ways - !(angle<0.2f) with the two attach calls swapped gave 135 asm lines, angle>=0.2f 137 lines, the kept nested if (angle<0.2) form is 125 (baseline) so both reverted; spell reload at 406fbf/407098 and dup-tail at 4071fb need register/slot variants. Earlier attempts: flags b1|=0x14;b1|=8 folded to 0x1c or hoisted, memset stores dword 1, fr=&frames[n-1], keys order. Readability done: ch->slot typed, CHARACTER+0x78 named turnAnim (CH_ANIM078 macro gone), no Q failure left. */

void ch_ActTurn(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    float angle;
    CHKEYFLAGS f;
    D3ANIMSLOT *s;
    D3VECTOR4 dir;
    D3ANIMFRAME *fr;
    D3ANIMKEY keys[2];

    if ((ch->slot || ch->morph) && !a->u.turn.started) {
        a->count = -1;
        return;
    }
    if (!a->count) {
        angle = a->args[0] * 0.017453292f;
        if (a->args[1]) {
            d3_DirToWorld(ch->obj, &d3_ZAxis, (D3VECTOR *)&dir);
            angle = -d3_VecAngle(&d3_ZAxis, (D3VECTOR *)&dir) - angle;
            if (angle > 3.1415927f)
                angle -= 6.2831855f;
            else if (angle < -3.1415927f)
                angle += 6.2831855f;
        } else {
            a->rot.x = 0.0f;
            a->rot.y = angle;
            a->rot.z = 0.0f;
            a->rot.x = ch->obj->rotation.x;
            a->rot.y = angle + ch->obj->rotation.y;
            a->rot.z = ch->obj->rotation.z;
        }
        if (angle < 0.2) {
            if (angle < -0.2)
                ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, "bewegung/dreh_90_rechts", 1);
        } else
            ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, "bewegung/dreh_90_links", 1);
        a->u.turn.started = 1;
        if (ch->slot) {
            s = ch->slot;
            fr = &s->data->frames[s->data->nframes - 1];
            dir.z = fr->turn.z;
            dir.y = fr->turn.y;
            keys[0].time = fr->time - 30;
            memset(&f, 0, sizeof(f));
            f.u.c.b1 |= 0x14;
            f.u.c.b1 |= 8;
            f.mid = 0;
            f.w.c.b8 = 1;
            keys[0].rot.x = 0.0f;
            keys[0].rot.y = 0.0f;
            keys[0].rot.z = 0.0f;
            keys[0].pos.x = 0.0f;
            keys[0].pos.y = 0.0f;
            keys[0].pos.z = 0.0f;
            keys[1].pos.x = 0.0f;
            keys[1].pos.y = 0.0f;
            keys[1].pos.z = 0.0f;
            keys[1].rot.x = -fr->turn.x;
            keys[1].rot.y = angle - dir.y;
            dir.z = -dir.z;
            keys[1].rot.z = dir.z;
            ch->turnAnim = d3anim_CreatePathK(a->ch->obj, f, 2, keys);
        } else {
            keys[0].time = 8;
            memset(&f, 0, sizeof(f));
            f.u.c.b1 |= 0x14;
            f.u.c.b1 |= 8;
            f.mid = 0;
            f.w.c.b8 = 1;
            keys[0].rot.x = 0.0f;
            keys[0].rot.y = 0.0f;
            keys[0].rot.z = 0.0f;
            keys[0].pos.x = 0.0f;
            keys[0].pos.y = 0.0f;
            keys[0].pos.z = 0.0f;
            keys[1].pos.x = 0.0f;
            keys[1].pos.y = 0.0f;
            keys[1].pos.z = 0.0f;
            keys[1].rot.x = 0.0f;
            keys[1].rot.y = angle;
            keys[1].rot.z = 0.0f;
            ch->turnAnim = d3anim_CreatePathK(a->ch->obj, f, 2, keys);
        }
        return;
    }
    if (ch->turnAnim && ch->turnAnim->flags2d & 0x20) {
        d3anim_FreePath(ch->obj);
        ch->turnAnim = 0;
        if (!ch->slot) {
            ch_RemoveAction(a);
            a = 0;
        }
    }
    if (ch->slot && ch->slot->flags.b[1] & 0x20) {
        d3_ReleaseTexture(a->ch->obj->material->animCtl, (char *)ch->slot->data);
        d3_SetObjectRot(a->ch->obj, &a->rot);
        d3anim_FreePath(a->ch->obj);
        ch->turnAnim = 0;
        ch_RemoveAction(a);
    } else if (!ch->slot && !ch->turnAnim && a)
        ch_RemoveAction(a);
}
/* 0x407200, up to 16 other characters of the same scene within 300 units of ch */

int ch_FindNear300(CHARACTER *ch, CHARACTER **out)
{
    int i, n = 0;
    CHARACTER *o;

    for (i = 0; i < CH_MAX; i++) {
        if (n >= 16)
            break;
        o = ch_List[i];
        if (o && ch != o && ch->scene == o->scene && ch->group == o->group
            && (o->obj->type != 1 || o->lowPolyBody && o->lowPolyBody->type != 1)
            && d3_VecEqual(&ch->obj->pos, &o->obj->pos, 300.0f))
            out[n++] = ch_List[i];
    }
    return n;
}
/* 0x4072a0, finds a walkable cell (type not 0 or 0xd) near x/z, searching rings of growing radius up to max;
   flags & 1 or 0x10: the one with the shortest way from -> cell -> to (0x10: x/z itself if walkable)
   @deep behaviour: no meaning difference left; the 54 code instructions and 24 fpu are the stack-slot permutation of the three D3VECTOR4 locals (ours dst,v,src at 0x18/0x28/0x38; original src,dst,v at 0x18/0x28/0x38) plus the loop temporaries living in the dead parameter slots; the fpu hunks are the same subtractions with the swapped slots (checked: fld src/fsub v both sides, 40759d). Earlier attempts: declaration order, renaming, struct copies, 12-byte src, scalar sx/sy/sz, dead init, swapped loop order. This session re-read 4072a0..407690 statement by statement: bounds min(1,z-r)/min(h->size-1,..) and the type tests are identical. Needs slotmap/vargen. Readability already clean (grade readable). */


char ch_FindFreeCell(D3SM *h, D3VECTOR *from, D3VECTOR *to, int x, int z, int *px, int *pz, int max, int flags)
{
    int r = 0;
    float best = 1e11f;
    D3VECTOR4 src, dst, v;
    int t, d, zi, xi, zmax, xmax;
    float dist, dx, dy, dz;

    if (h && h->entries && px && pz) {
        if (flags & 0x10) {
            t = h->entries[h->size * z + x].type;
            if (t && t != 0xd) {
                *px = x;
                *pz = z;
                return 1;
            }
            dst.x = to->x;
            dst.y = to->y;
            dst.z = to->z;
        } else
            d3sm_GetVertex(h, x, z, (D3VECTOR *)&dst);
        src.x = from->x;
        src.y = from->y;
        src.z = from->z;
        *px = -1;
        *pz = -1;
        if (!(flags & 1) && !(flags & 0x10)) {
            for (; r < h->size; r++) {
                if (r >= max)
                    break;
                zmax = min(h->size - 1, r + z);
                for (zi = max(1, z - r); zi <= zmax; zi++) {
                    d = r - abs(z - zi);
                    xmax = min(h->size - 1, d + x);
                    for (xi = max(0, x - d); xi <= xmax; xi++) {
                        if (zi >= 0 && zi < h->size && xi >= 0 && xi < h->size
                            && (t = h->entries[h->size * zi + xi].type) && t != 0xd) {
                            *px = xi;
                            *pz = zi;
                            return 1;
                        }
                    }
                }
            }
        } else {
            for (; r < h->size; r++) {
                if (r >= max)
                    break;
                zmax = min(h->size - 1, r + z);
                for (zi = max(1, z - r); zi <= zmax; zi++) {
                    d = r - abs(z - zi);
                    xmax = min(h->size - 1, d + x);
                    for (xi = max(0, x - d); xi <= xmax; xi++) {
                        if (zi >= 0 && zi < h->size && xi >= 0 && xi < h->size
                            && (t = h->entries[h->size * zi + xi].type) && t != 0xd
                            && d3sm_GetVertex(h, xi, zi, (D3VECTOR *)&v)) {
                            dx = src.x - v.x;
                            dy = src.y - v.y;
                            dz = src.z - v.z;
                            dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
                            dx = dst.x - v.x;
                            dy = dst.y - v.y;
                            dz = dst.z - v.z;
                            dist += (float)sqrt(dx * dx + dy * dy + dz * dz);
                            if (dist < best) {
                                best = dist;
                                *px = xi;
                                *pz = zi;
                            }
                        }
                    }
                }
            }
        }
        if (*px != -1 || *pz != -1)
            return 1;
        *px = x;
        *pz = z;
    }
    return 0;
}
/* 0x407690, allocates the path map's origin / cell-size node (0x14 bytes, "ch_map") */

void *ch_MapAlloc(float cellSize)
{
    CHMAPNODE *p = mem_Alloc(sizeof(CHMAPNODE), "ch_map");

    p->cellSize = cellSize;
    return p;
}
/* 0x4076b0, frees a path map node allocated by ch_MapAlloc */

void ch_MapFree(void *p)
{
    mem_Free(p);
}
/* 0x4076c0, copies the vector src to dst */

void ch_CopyVec(D3VECTOR *dst, D3VECTOR *src)
{
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}
/* 0x4076e0, height map cell nearest to pos */

int ch_PosToCell(D3SM *h, D3VECTOR *pos, int *x, int *z)
{
    D3VECTOR4 d;
    float f;

    d.x = pos->x - h->origin.x;
    d.z = pos->z - h->origin.z;
    f = 1.0f / h->cell.x;
    d.x *= f;
    d.z *= f;
    *x = (int)(d.x + 0.5);
    *z = (int)(d.z + 0.5);
    return 1;
}
/* 0x407740, converts cell x/z of map h to the cell of map to */

int ch_CellToCell(D3SM *h, int x, int z, D3SM *to, int *px, int *pz)
{
    D3VECTOR4 v;

    if (d3sm_GetVertex(h, x, z, (D3VECTOR *)&v)) {
        ch_PosToCell(to, (D3VECTOR *)&v, px, pz);
        return 1;
    }
    return 0;
}
/* 0x407790, the character's cells in ch_PathMap (transport, body, the next path cells), then a corner
   cell for each diagonal step
   @deep behaviour: close, 10 differing instructions. Two hunks: (1) 4077d7 slots - the address-taken x and z locals of ch_PosToCell are swapped (ours z,x at 0x1c/0x20; original x,z) and the same pair again at 0x2c/0x30; earlier attempts (declaration order, inner block, int xz[2], reusing i/n, (void)z;, if(0) z=0;, all 6 store orders); (2) 40783c moved - original mov ebp,ecx; add ebp,edi for n = i + (ch->transport ? 2 : 1), ours add ecx,edi; mov ebp,ecx; spell moved prescriptions tried this session: split the init into two statements before the loop (byte-identical) and swapped the operand order (byte-identical). Statements read against 407790..4078a0: same bounds and calls. Readability done this session: u.walk.unk0f0 -> pathLen (cell count, indexes *2 into path) and u.walk.unk154 -> retryAt (ch_Time of the last failed path search + 200/225 ms wait, -1: none), Q2 clean. */

int ch_GetWaypoints(D3SM *h, CHARACTER *ch, CHWAYPT *out)
{
    CHACTION *a = CH_ACTIONS(ch);
    CHWAYPT *start = out;
    CHWAYPT *p;
    int x, z, i, n;
    unsigned char *c;

    if (ch->transport) {
        ch_PosToCell(ch_PathMap, &CH_TRANSPORT(ch)->obj->pos, &out->x, &out->z);
        out->flags = 8;
        out++;
    }
    ch_PosToCell(ch_PathMap, &ch->obj->pos, &x, &z);
    out->x = x;
    out->z = z;
    out->flags = 1;
    out++;
    if (a && a->type == '-' && a->u.walk.path && a->u.walk.retryAt == -1) {
        for (i = a->u.walk.cur, n = i + (ch->transport ? 2 : 1); i < n && i < a->u.walk.pathLen; i++) {
            c = (unsigned char *)a->u.walk.path + i * 2;
            ch_CellToCell(h, c[0], c[1], ch_PathMap, &out->x, &out->z);
            out->flags = 2;
            out++;
        }
    }
    out->flags = 0;
    for (p = start; p->flags; p++) {
        if (p->flags & 4 || !p[1].flags || p[1].flags & 4)
            break;
        if (p[1].x > p->x && p[1].z < p->z) {
            out->flags = p->flags | 4;
            out->x = p->x + 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z - 1;
            out++;
        } else if (p[1].x > p->x && p[1].z > p->z) {
            out->flags = p->flags | 4;
            out->x = p->x + 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z + 1;
            out++;
        } else if (p[1].x < p->x && p[1].z > p->z) {
            out->flags = p->flags | 4;
            out->x = p->x - 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z + 1;
            out++;
        } else if (p[1].x < p->x && p[1].z < p->z) {
            out->flags = p->flags | 4;
            out->x = p->x - 1;
            out->z = p->z;
            out++;
            out->flags = p->flags | 4;
            out->x = p->x;
            out->z = p->z - 1;
            out++;
        }
    }
    out->flags = 0;
    return 1;
}
/* 0x4079b0, walk actions: checks the next path cells against the nearby characters others[0..n-1] and,
   if one blocks the way, marks them in the ground map and searches a new path around them ("ch:GetNewPath"
   is the mem_Alloc tag). Returns the new path length, 0 nothing to do, -1 failed, -2 no way around.
   @deep Read in full against 0x4079b0..0x408486: the blocker scan over the waypoints (flags 2 / 1 / 8, the
   contact links, the speed scale), the contacts table, the ground marks of the blockers' paths and
   positions, the rest of the path kept, the free cells, path_Find, the thinned path and its end. All
   statements line up (ours 828 vs 832); the contacts store now writes ch then flags through the
   flags-biased pointer as the original (dist 203 -> 200). Left: allocation and frame - our frame is 0x9bc
   vs 0x9b8 (one 4-byte local more: the escaped cells cx/cz/sx/i/gx share slots otherwise, the prover stops
   at frame+0x4), the scene-map test loads into edi with walk.cur read first in the original (+0x86), the
   rest loop biases its pointer to rest+1 in the original ('+ lea ecx,[esp+0x1c9] / mov [ecx-1],bl / mov
   [ecx],bl', an nrest-indexed loop compiles far worse: 297), 'or al,1' vs 'or ebx,1' on walk.flags | 1
   with the FindFreeCell product in eax vs ebx (+0x878), the loop counters i / others pointer in
   [esp+0x18]/[esp+0x1c] vs ours [esp+0x1c]/[esp+0x2c]. (references: M8 at +0x86: ours refers to
   d3_DefaultScene (0x5fb5b0), the original's field to 0x63108c (ch_PathMap), explained: the M8 at +0x86
   (d3_DefaultScene | ch_PathMap) is the shifted alignment of the scene-map test: both sides read
   d3_DefaultScene[scene].map and store 60.0 to ch_PathMap->cell.x on both paths (asmdiff --symbols)) */

int ch_GetNewPath(CHARACTER *ch, int n, CHARACTER **others)
{
    int res = 0;
    CHACTION *a = CH_ACTIONS(ch);
    int scene = d3t_SceneIndex(ch->scene);
    D3SM *h;
    int steps, cx, cz, tx, tz, bx, bz, gx, gz;
    int i, j, k, r, e, last, nrest, cnt, m, sx;
    float y, d, dx, dy, dz;
    CHACTION *oa;
    CHWAYPT wp[32];
    unsigned char *p, *q;
    unsigned char rest[0x800];

    if (a->type != '-')
        return -1;
    h = (D3SM *)ch_GetHMap(ch);
    ch_CopyVec((D3VECTOR *)ch_PathMap, (D3VECTOR *)h);
    steps = (int)(50.0f / h->cell.x + 0.5);
    if (steps > 10)
        steps = 10;
    if (d3_DefaultScene[scene].map)
        ch_PathMap->cell.x = 60.0f;
    else
        ch_PathMap->cell.x = 60.0f;
    p = (unsigned char *)a->u.walk.path + a->u.walk.cur * 2;
    ch_CellToCell(h, p[0], p[1], ch_PathMap, &cx, &cz);
    if (ch->transport)
        ch_PosToCell(ch_PathMap, &CH_TRANSPORT(ch)->obj->pos, &tx, &tz);
    ch_PosToCell(ch_PathMap, &ch->obj->pos, &bx, &bz);
    for (i = 0; i < n; i++) {
        if (res & 1)
            break;
        oa = CH_ACTIONS(others[i]);
        for (k = 0; k < 16; k++)
            if (ch->contacts[k].ch == others[i] && ch->contacts[k].flags & 1)
                break;
        ch_GetWaypoints(h, others[i], wp);
        for (j = 0; j < 32; j++) {
            if (!wp[j].flags)
                break;
            if (wp[j].flags & 2) {
                if (wp[j].x == cx && wp[j].z == cz) {
                    res = 1;
                    break;
                }
            } else if (wp[j].flags & 1) {
                if (wp[j].x == bx && wp[j].z == bz) {
                    res = 5;
                    break;
                }
                if (wp[j].x == cx && wp[j].z == cz) {
                    if (!oa || oa->type != '-' || oa->u.walk.retryAt != -1) {
                        res = 5;
                        break;
                    }
                    for (k = 0; k < 16 && ch->contacts[k].ch; k++)
                        if (others[i]->contacts[k].ch == ch && others[i]->contacts[k].flags & 2)
                            goto linked;
                    dx = ch->obj->pos.x - others[i]->obj->pos.x;
                    dy = ch->obj->pos.y - others[i]->obj->pos.y;
                    dz = ch->obj->pos.z - others[i]->obj->pos.z;
                    d = sqrt(dx * dx + dy * dy + dz * dz);
                    ch->speedScale = d = (float)(d * 0.01f) * 0.5f;
                    ch->speedScale = min(1.0f, d);
                    res = 2;
                    break;
                linked:
                    res = 1;
                    break;
                }
                if (oa && oa->type == '-' && oa->u.walk.retryAt == -1)
                    continue;
                if (wp[j].x == tx && wp[j].z == tz) {
                    res = 1;
                    break;
                }
            } else if (wp[j].flags & 8) {
                if (wp[j].x == cx && wp[j].z == cz) {
                    if (!oa || oa->type != '-' || oa->u.walk.retryAt != -1) {
                        res = 5;
                        break;
                    }
                    for (k = 0; k < 16 && ch->contacts[k].ch; k++)
                        if (others[i]->contacts[k].ch == ch && others[i]->contacts[k].flags & 2) {
                            res = 1;
                            goto next;
                        }
                    dx = ch->obj->pos.x - CH_TRANSPORT(others[i])->obj->pos.x;
                    dy = ch->obj->pos.y - CH_TRANSPORT(others[i])->obj->pos.y;
                    dz = ch->obj->pos.z - CH_TRANSPORT(others[i])->obj->pos.z;
                    d = sqrt(dx * dx + dy * dy + dz * dz);
                    ch->speedScale = d = (float)(d * 0.01f) * 0.5f;
                    ch->speedScale = min(1.0f, d);
                    res = 2;
                } else if (wp[j].x == tx && wp[j].z == tz) {
                    res = 1;
                    break;
                }
            }
        next:
            ;
        }
    }
    if (res) {
        memset(ch->contacts, 0, 16 * sizeof(CHCONTACT));
        for (i = 0; i < n; i++) {
            ch->contacts[i].ch = others[i];
            ch->contacts[i].flags = res;
        }
        if (res & 1) {
            if (ch->speedScale < 0.5)
                ch->speedScale = 0.5f;
            for (i = 0; i < n; i++) {
                oa = CH_ACTIONS(others[i]);
                if (oa && oa->type == '-' && oa->u.walk.path && oa->u.walk.retryAt == -1) {
                    r = steps;
                    if (r > 3)
                        r = 3;
                    k = others[i]->transport ? oa->u.walk.cur - 2 : oa->u.walk.cur;
                    if (k < 1)
                        k = 1;
                    for (e = k + 2; k < e && k < oa->u.walk.pathLen; k++) {
                        gx = ((unsigned char *)oa->u.walk.path)[k * 2];
                        gz = ((unsigned char *)oa->u.walk.path)[k * 2 + 1];
                        if (k == oa->u.walk.pathLen - 1)
                            ch_SetGroundDiamond(gx, gz, scene, 0xd, r);
                        else
                            ch_SetGroundDiamond(gx, gz, scene, 1, r);
                    }
                }
            }
            for (i = 0; i < n; i++) {
                oa = CH_ACTIONS(others[i]);
                if (oa && oa->type == '-' && oa->u.walk.path && oa->u.walk.retryAt == -1) {
                    if (d3_GetHeightAt(h, &others[i]->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0, steps);
                    if (others[i]->transport && d3_GetHeightAt(h, &CH_TRANSPORT(others[i])->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0, steps * 2);
                } else {
                    if (d3_GetHeightAt(h, &others[i]->obj->pos, &gx, &gz, &y)) {
                        r = min(9, steps);
                        if (oa && oa->u.walk.retryAt != -1)
                            ch_SetGroundDiamond(gx, gz, scene, 0, r);
                        else
                            ch_SetGroundDiamond(gx, gz, scene, 0xd, r);
                    }
                    if (others[i]->transport && d3_GetHeightAt(h, &CH_TRANSPORT(others[i])->obj->pos, &gx, &gz, &y))
                        ch_SetGroundDiamond(gx, gz, scene, 0xd, steps * 2);
                }
            }
            a = CH_ACTIONS(ch);
            if (d3_GetHeightAt(h, &a->ch->obj->pos, &sx, &cx, &y)) {
                last = min(a->u.walk.cur + 16, a->u.walk.pathLen - 1);
                nrest = 0;
                q = rest;
                for (k = last + 1; k < a->u.walk.pathLen; k++) {
                    q[0] = ((unsigned char *)a->u.walk.path)[k * 2];
                    q[1] = ((unsigned char *)a->u.walk.path)[k * 2 + 1];
                    q += 2;
                    nrest++;
                }
                gx = ((unsigned char *)a->u.walk.path)[last * 2];
                gz = ((unsigned char *)a->u.walk.path)[last * 2 + 1];
                m = ch->scene == d3_Scenes || d3_Terrain ? 5 : 2;
                if (!ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, gx, gz, &cz, &i, steps * m,
                                     a->u.walk.flags | 1)) {
                    ch_RestoreGround((struct D3HMAP *)h);
                    return -1;
                }
                gx = cz;
                gz = i;
                ch_Path.xy = ch_PathBuf;
                ch_Path.max = 0x200;
                if (h->entries[h->size * cx + sx].type == 0xd) {
                    m = ch->scene == d3_Scenes || d3_Terrain ? 5 : 2;
                    if (ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, sx, cx, &cz, &i, steps * m,
                                        a->u.walk.flags | 1)) {
                        sx = cz;
                        cx = i;
                    }
                }
                if (path_Find(&ch_Path, (PMAP *)h, sx, cx, gx, gz, ch->pathType) == 0xffff) {
                    ch_RestoreGround((struct D3HMAP *)h);
                    return -1;
                }
                cnt = ch_Path.count;
                if (a->u.walk.path) {
                    mem_Free(a->u.walk.path);
                    a->u.walk.path = 0;
                }
                a->u.walk.path = mem_Alloc((cnt + nrest) * 2, "ch:GetNewPath");
                tz = ch_ThinPath(ch, &ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
                for (k = 0; k < nrest; k++) {
                    ((unsigned char *)a->u.walk.path)[(tz + k) * 2] = rest[k * 2];
                    ((unsigned char *)a->u.walk.path)[(tz + k) * 2 + 1] = rest[k * 2 + 1];
                }
                a->u.walk.pathLen = tz + nrest;
                a->u.walk.cur = 0;
                a->u.walk.prevCell = -1;
                gx = ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 2];
                gz = ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 1];
                d3sm_GetVertex(h, gx, gz, &a->u.walk.end);
                a->u.walk.end.x += ch_PathOffset.x;
                a->u.walk.end.y += ch_PathOffset.y;
                a->u.walk.end.z += ch_PathOffset.z;
                if (a->u.walk.pathLen >= 2
                    && ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 2]
                       == ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 4]
                    && ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 1]
                       == ((unsigned char *)a->u.walk.path)[a->u.walk.pathLen * 2 - 3])
                    a->u.walk.pathLen--;
                ch_RestoreGround((struct D3HMAP *)h);
                ch->contactSteps = 4;
                if (sx == gx && cx == gz)
                    return -2;
                return tz;
            }
        }
    }
    return 0;
}
/* 0x408490, walk actions: finds a path from the character to the target character ("ch:GetNewPath"?) */

int ch_PathToTarget(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3SM *h = (D3SM *)ch_GetHMap(ch);
    int x1, z1, x2, z2, ox, oz;
    float y;
    CHARACTER *t;

    if (!d3_GetHeightAt(h, &a->ch->obj->pos, &x1, &z1, &y))
        return 0;
    if (!d3_GetHeightAt(h, &a->u.walk.target->obj->pos, &x2, &z2, &y))
        return 0;
    if (z1 < 1)
        z1 = 1;
    else if (z1 > h->size - 2)
        z1 = h->size - 2;
    if (x1 < 1)
        x1 = 1;
    else if (x1 > h->size - 2)
        x1 = h->size - 2;
    d3sm_FindGround(h, x1, z1, &ox, &oz);
    x1 = ox;
    z1 = oz;
    ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, x2, z2, &ox, &oz, 4, 0);
    x2 = ox;
    z2 = oz;
    ch_Path.xy = ch_PathBuf;
    ch_Path.max = 0x200;
    if (path_Find(&ch_Path, (PMAP *)h, x1, z1, x2, z2, ch->pathType) == 0xffff)
        return 0;
    if (a->u.walk.path) {
        mem_Free(a->u.walk.path);
        a->u.walk.path = 0;
    }
    a->u.walk.path = mem_Alloc(ch_Path.count * 2, "ch_t:waypoints");
    if (!a->u.walk.path)
        __asm int 3
    a->u.walk.pathLen = ch_ThinPath(ch, &ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
    t = a->u.walk.target;
    a->u.walk.cur = 0;
    a->u.walk.prevCell = -1;
    a->u.walk.tpos.x = t->obj->pos.x;
    a->u.walk.tpos.y = t->obj->pos.y;
    a->u.walk.tpos.z = t->obj->pos.z;
    return 1;
}

/* ---- 0x408690..0x40a900 (worker ch5) ---- */

/* 0x408690, walk actions: drops the current animation and finds the path to args[0], args[1] in scene args[2] */
int ch_InitWalk(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    int x, z, t[2];             /* t: target cell x, z */
    float y;
    D3ANIMDATA *ad;
    char name[0x60];

    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        ad = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(ad, 1);
        ch->morph = 0;
    }
    d3t_ReleaseObjectTextures(ch->obj);
    ch->next = 0;
    ch->morph = 0;
    ch->slot = 0;
    ch->speedScale = 1.0f;
    a->ch->flags.all &= ~8;
    a->u.walk.path = 0;
    a->u.walk.started = 0;
    a->u.walk.retryAt = -1;
    memset(ch->contacts, 0, sizeof ch->contacts);
    t[0] = a->args[0];
    t[1] = a->args[1];
    if (d3t_SceneIndex(ch->scene) != a->args[2]) {
        ch_RemoveAction(a);
        return 0;
    }
    if (a->u.walk.flags & CH_WALK_TARGET)
        a->u.walk.target = ch_FindByName(a->u.walk.tname);
    if (!d3_GetHeightAt(h, &a->ch->obj->pos, &x, &z, &y)) {
        if (ch_MarkWalkErrors)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return 0;
    }
    if (x < 1)
        x = 1;
    else if (x > h->size - 2)
        x = h->size - 2;
    if (z < 1)
        z = 1;
    else if (z > h->size - 2)
        z = h->size - 2;
    if (ch->kind & 4)
        a->u.walk.flags &= ~CH_WALK_DEST;
    if (a->u.walk.flags & CH_WALK_DEST)
        ch_FindFreeCell(h, &ch->obj->pos, &a->u.walk.dest, t[0], t[1], &t[1], &t[0], 8, a->u.walk.flags);
    else
        ch_FindFreeCell(h, &ch->obj->pos, &ch->obj->pos, t[0], t[1], &t[1], &t[0], 8, a->u.walk.flags);
    ch_Path.xy = ch_PathBuf;
    ch_Path.max = 0x200;
    if (path_Find(&ch_Path, (PMAP *)h, x, z, t[1], t[0], ch->pathType) == 0xffff) {
        a->ch->pathState = 0;
        if (ch_MarkWalkErrors)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        if (a->u.walk.fail)
            a->u.walk.fail(a);
        ch_RemoveAction(a);
        return 0;
    }
    a->u.walk.path = mem_Alloc(ch_Path.count * 2, "ch_t:waypoints");
    a->u.walk.pathLen = ch_ThinPath(ch, &ch_Path, (CHPATHPT *)a->u.walk.path, (CHWALKDEF *)h);
    a->u.walk.cur = 0;
    a->u.walk.prevCell = -1;
    d3sm_GetVertex(h, ((unsigned char *)a->u.walk.path)[0], ((unsigned char *)a->u.walk.path)[1], &a->u.walk.end);
    a->u.walk.end.x += ch_PathOffset.x;
    a->u.walk.end.y += ch_PathOffset.y;
    a->u.walk.end.z += ch_PathOffset.z;
    if (a->u.walk.flags & CH_WALK_TARGET) {
        a->u.walk.tpos.x = CH_POS038(a->u.walk.target)->x;
        a->u.walk.tpos.y = CH_POS038(a->u.walk.target)->y;
        a->u.walk.tpos.z = CH_POS038(a->u.walk.target)->z;
    } else {
        d3sm_GetVertex(h, t[1], t[0], &a->u.walk.tpos);
        a->u.walk.end.x += ch_PathOffset.x;     /* sic: end, not tpos */
        a->u.walk.end.y += ch_PathOffset.y;
        a->u.walk.end.z += ch_PathOffset.z;
        if ((a->u.walk.flags & CH_WALK_DEST) && !d3_VecEqual(&a->u.walk.tpos, &a->u.walk.dest, 30.0f)) {
            a->u.walk.dest.x = a->u.walk.tpos.x;
            a->u.walk.dest.y = a->u.walk.tpos.y;
            a->u.walk.dest.z = a->u.walk.tpos.z;
        }
    }
    a->u.walk.started = 0;
    return 1;
}
/* 0x408ab0, plays a step sound for the ground type when the walk animation reaches a step frame */

void ch_WalkSound(CHACTION *a, int ground)
{
    CHARACTER *ch = a->ch;
    void *snd;
    char name[0x80];

    if (ch->slot && ch_SndLib && d3_DefaultScene == ch->scene && ch_CurrentGroup == ch->group
        && ch->slot->frame != a->u.walk.lastframe) {
        if (ch->slot->frame == 4 || ch->slot->frame == 12 || ch->slot->frame == 18
            || ch->slot->frame == 24) {
            if ((ch->kind & 1) && ch_StepSounds < 3 && d3t_SceneIndex(ch->scene)) {
                if (ch->scene->map) {
                    strcpy(name, "Normal_s");
                    if (ground == 6 || ground == 11)
                        strcpy(name, "Stein_s");
                    else if (ground == 3)
                        strcpy(name, "Erde_s");
                    else if (ground == 4)
                        strcpy(name, "Wiese_s");
                    else if (ground == 8)
                        strcpy(name, "Stein_s");
                    else if (ground == 10)
                        strcpy(name, "Pfuetze_s");
                    snd = sb_GetSample(ch_SndLib, name);
                } else
                    snd = sb_GetSample(ch_SndLib, ch_StepSample);
                if (snd) {
                    d3snd_PlayAt(snd, ch->obj, 40, 500.0f);
                    ch_StepSounds++;
                }
            }
            a->u.walk.lastframe = ch->slot->frame;
        }
    }
}
/* 0x408cb0, starts the walk animation; its speed follows the cell size of the height map */

int ch_StartWalkAnim(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    float f;
    CHSLOT *s;
    D3ANIMDATA *ad;
    char name[0x60];

    ch->next = 0;
    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        ad = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(ad, 1);
        ch->morph = 0;
    }
    d3t_ReleaseObjectTextures(ch->obj);
    if (!ch->obj->info)
        ch_InflateModel(ch);
    if (ch->transport)
        s = (CHSLOT *)ch_AttachAni(ch, "bewegung/karren_ziehen", 1);
    else
        s = (CHSLOT *)ch_AttachAni(ch, "bewegung/gehen", 1);
    if (!(ch->slot = (D3ANIMSLOT *)s)) {
        if (ch_MarkWalkErrors)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return 0;
    }
    s->f6d_10 = 0;
    s = CH_XSLOT(ch);
    s->b70 = 0;
    s->a70 = 1;
    if (ch->kind & 8)
        CH_XSLOT(ch)->speed = 30.0f;
    else if (ch->kind & 0x10)
        CH_XSLOT(ch)->speed = 60.0f;
    else
        CH_XSLOT(ch)->speed = 15.0f;
    f = 50.0f / h->cell.x;
    f = min(2.0f, f);
    CH_XSLOT(ch)->speed *= f;
    CH_XSLOT(ch)->play = 1;
    CH_XSLOT(ch)->f74_8 = 0;
    CH_XSLOT(ch)->f74_4 = 1;
    CH_XSLOT(ch)->f6d_20 = 0;
    a->u.walk.started = 1;
    return 1;
}
/* 0x408e60, walk actions: avoids other characters, then takes the next path cell; 0 when the walk ends */

int ch_NextWaypoint(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    int n, r;
    float speed, f, dx, dy, dz;
    D3OBJECT *o;
    CHARACTER *others[16];

    if (ch->contactSteps < 1 && (ch->kind & 1) && (!ch->slot || !CH_XSLOT(ch)->f74_8)
        && (d3_DefaultScene == ch->scene || ch->scene == d3_Scenes)) {
        if (n = ch_FindNear300(ch, others)) {
            r = ch_GetNewPath(ch, n, others);
            if (r > 0) {
                if (ch->slot) {
                    if (ch->kind & 8)
                        speed = 30.0f;
                    else if (ch->kind & 0x10)
                        speed = 30.0f;
                    else
                        speed = 15.0f;
                    if (ch->scene->map)
                        speed *= 3.0;
                    f = 50.0f / h->cell.x;
                    CH_XSLOT(ch)->speed = speed * min(2.0f, f);
                    CH_XSLOT(ch)->play = 1;
                    CH_XSLOT(ch)->f74_8 = 0;
                    CH_XSLOT(ch)->f74_4 = 1;
                    CH_XSLOT(ch)->f6d_20 = 0;
                    o = a->ch->obj;
                    dx = o->pos.x - a->u.walk.end.x;
                    dy = o->pos.y - a->u.walk.end.y;
                    dz = o->pos.z - a->u.walk.end.z;
                    f = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    a->u.walk.dist = f;
                }
            } else if (r == -1) {
                if (ch->slot)
                    CH_XSLOT(ch)->playSpeed = 0.01f;
                a->u.walk.retryAt = ch_Time;
                return 0;
            } else if (r == -2) {
                ch_StopAnim(ch);
                if (ch_MarkWalkErrors)
                    a->ch->flags.bytes.lo |= 8;
                if (a->u.walk.path) {
                    mem_Free(a->u.walk.path);
                    a->u.walk.path = 0;
                }
                ch_RemoveAction(a);
                return 0;
            }
        }
    }
    if (!a->u.walk.cur || (ch->slot && CH_XSLOT(ch)->f74_8)) {
        if ((a->u.walk.flags & CH_WALK_TARGET) && a->u.walk.cur % 4 == 0
            && !d3_VecEqual(&a->u.walk.tpos, &a->u.walk.target->obj->pos, 70.0f))
            ch_PathToTarget(a);
        a->u.walk.cur++;
        if (ch->contactSteps > 0 && !--ch->contactSteps)
            memset(ch->contacts, 0, sizeof ch->contacts);
        if (a->u.walk.cur >= a->u.walk.pathLen) {
            if (a->u.walk.path) {
                mem_Free(a->u.walk.path);
                a->u.walk.path = 0;
            }
            if (a->u.walk.flags & CH_WALK_DEST) {
                if (ch->scene == d3_Scenes && a->u.walk.retries > 4)
                    a->u.walk.flags &= ~CH_WALK_DEST;
                if (!d3_VecEqual(&ch->obj->pos, &a->u.walk.dest, 5.0f)) {
                    a->u.walk.end.x = a->u.walk.dest.x;
                    a->u.walk.end.y = a->u.walk.dest.y;
                    a->u.walk.end.z = a->u.walk.dest.z;
                    CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
                    CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
                    CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
                    o = a->ch->obj;
                    dx = o->pos.x - a->u.walk.end.x;
                    dy = o->pos.y - a->u.walk.end.y;
                    dz = o->pos.z - a->u.walk.end.z;
                    a->u.walk.angle = -1.0f;
                    f = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    a->u.walk.dist = f;
                    CH_XSLOT(ch)->speed = 5.0f;
                    CH_XSLOT(ch)->play = 1;
                    CH_XSLOT(ch)->f74_8 = 0;
                    CH_XSLOT(ch)->f6d_20 = 0;
                    a->u.walk.retries++;
                    return 0;
                }
                d3_SetObjectPos(ch->obj, &a->u.walk.dest);
                ch_PlaceOnGround(ch, 1);
            }
            ch_StopAnim(ch);
            if (ch_MarkWalkErrors)
                a->ch->flags.bytes.lo |= 8;
            ch_RemoveAction(a);
            return 0;
        }
        d3sm_GetVertex(h, ((unsigned char *)a->u.walk.path)[a->u.walk.cur * 2],
                       ((unsigned char *)a->u.walk.path)[a->u.walk.cur * 2 + 1], &a->u.walk.end);
        a->u.walk.end.x += ch_PathOffset.x;
        a->u.walk.end.y += ch_PathOffset.y;
        a->u.walk.end.z += ch_PathOffset.z;
        a->u.walk.angle = d3_YawTo(ch->obj, &a->u.walk.end);
        o = a->ch->obj;
        dx = o->pos.x - a->u.walk.end.x;
        dy = o->pos.y - a->u.walk.end.y;
        dz = o->pos.z - a->u.walk.end.z;
        f = (float)sqrt(dx * dx + dy * dy + dz * dz);
        a->u.walk.dist = f;
        if (ch->slot) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1 && a->u.walk.pathLen > 1) {
                if (ch->kind & 8)
                    speed = 24.0f;
                else if (ch->kind & 0x10)
                    speed = 64.0f;
                else
                    speed = 8.0f;
            } else {
                if (ch->kind & 8)
                    speed = 45.0f;
                else if (ch->kind & 0x10)
                    speed = 75.0f;
                else
                    speed = 15.0f;
            }
            if (ch->scene->map)
                speed *= 3.0;
            CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
            CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
            CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
            CH_XSLOT(ch)->speed = speed;
            CH_XSLOT(ch)->play = 1;
            CH_XSLOT(ch)->f74_8 = 0;
            CH_XSLOT(ch)->f6d_20 = 0;
        }
    }
    return 1;
}
/* 0x4093d0, walk actions: turns the character towards the next path cell; a->u.walk.angle < 0 left,
   > 0 right; 0: checks that the character did not walk past the cell */

void ch_WalkRotate(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3OBJECT *o;
    float ang, r, step;
    double rot;
    D3VECTOR4 d, p;
    char buf[0x100];

    if (a->u.walk.angle < 0.0f) {
        o = ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (!(ang <= 0.04 && ang >= -0.04)) {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 1.4f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                ch->speedScale = 0.4f;
            if (fabs(ang) > 1.5707964f)
                step *= 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                step *= 0.14f;
            else if (fabs(ang) > 0.5235988f)
                step *= 0.06f;
            else
                step *= 0.04f;
            r = rot - step;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r < ang + o->rotation.y) {
                r = ang + o->rotation.y;
                a->u.walk.angle = 0.0f;
            }
        } else {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else if (a->u.walk.angle > 0.0f) {
        o = ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (!(ang <= 0.04 && ang >= -0.04)) {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 0.8f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                ch->speedScale = 0.4f;
            if (fabs(ang) > 1.5707964f)
                step *= 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                step *= 0.14f;
            else if (fabs(ang) > 0.5235988f)
                step *= 0.06f;
            else
                step *= 0.04f;
            r = rot + step;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r > ang + o->rotation.y) {
                r = ang + o->rotation.y;
                a->u.walk.angle = 0.0f;
            }
        } else {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else {
        o = ch->obj;
        p.x = o->pos.x;
        p.y = o->pos.y;
        p.z = o->pos.z;
        d.x = p.x - a->u.walk.end.x;
        d.y = p.y - a->u.walk.end.y;
        d.z = p.z - a->u.walk.end.z;
        if ((float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) > a->u.walk.dist * 1.4) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1)
                ch_SetPos(ch, &a->u.walk.end, 0);
            if (ch->slot)
                CH_XSLOT(ch)->f74_8 = 1;
            sprintf(buf, "ch_WalkOnPathRotation(): Have missed my treshhold... %s [%f %f]", a->ch->obj,
                    a->u.walk.end.x, a->u.walk.end.z);
        }
    }
}
/* 0x4097d0, action callback 0x2d: walks along the path to the cell args[0], args[1] in scene args[2]
   @deep Read in full against 0x4097d0..0x409bce; all statements and calls line up (ours 319 vs 321),
   D3OBJ_UNK200 replaced by D3OBJECT.owner. Left: one redundant test the original keeps in the
   walk-animation start: after 'test byte [eax+0x6d],0x20' fails it re-tests the slot ('+ jne 0x4099d5 /
   cmp eax,ebx / jne 0x4099e7' where ours has '- je 0x4099e3'), i.e. the third term of !slot ||
   slot->f6d_20 || !slot is not threaded away in the original; the source keeps the three terms, and the
   spellings tried (!CH_XSLOT(ch) first or third, slot->flags.b[1] & 0x20, !a->ch->slot: 23 worse) all
   compile to the threaded form; the rest is the 4-byte shift of every later jump target. */

void ch_ActWalk(CHACTION *a)
{
    D3SM *h = (D3SM *)ch_GetHMap(a->ch);
    CHARACTER *ch = a->ch;
    int ground;
    float f;
    char name[0x100];

    if (a->u.walk.flags & CH_WALK_ARRIVED) {
        if (!ch->morph || ((CHSLOT *)ch->morph)->f6d_20)
            ch_RemoveAction(a);
        return;
    }
    if (a->stop) {
        sprintf(name, "%s_%s", "bewegung/gehen", ch->body);
        vfs_ToSlashes(name);
        if (ch->slot)
            d3_ReleaseTexture(a->ch->obj->material->animCtl, (char *)CH_XSLOT(ch)->data);
        ch->slot = 0;
        ch_FollowGround(a);
        if (ch_MarkWalkErrors)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return;
    }
    if (a->u.walk.retryAt != -1) {
        if (ch_Time < a->u.walk.retryAt + 225)
            return;
        if (ch->slot)
            CH_XSLOT(ch)->playSpeed = 1.0f;
        a->u.walk.retryAt = -1;
    }
    if (a->u.walk.flags & CH_WALK_RESTART) {
        a->u.walk.flags &= ~CH_WALK_RESTART;
        a->count = 0;
    }
    if (!a->count) {
        if (ch->scene->dirty) {
            ch_RemoveAction(a);
            return;
        }
        if (!ch_InitWalk(a, h))
            return;
    }
    if (a->u.walk.flags & CH_WALK_STOPANIM) {
        if (ch->slot && CH_XSLOT(ch)->f6d_20) {
            d3_ReleaseTexture(ch->obj->material->animCtl, (char *)CH_XSLOT(ch)->data);
            d3anim_FreePath(ch->obj);
            ch->turnAnim = 0;
            a->u.walk.flags &= ~CH_WALK_STOPANIM;
        } else if (ch->slot)
            return;
        else
            a->u.walk.flags &= ~CH_WALK_STOPANIM;
    }
    if (!a->u.walk.started && (!ch->slot || CH_XSLOT(ch)->f6d_20 || !ch->slot)) {
        if (!ch_StartWalkAnim(a, h))
            return;
    }
    if (!a->u.walk.started && !ch->slot) {
        if (ch_MarkWalkErrors)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return;
    }
    if (!a->u.walk.started && ch->slot)
        return;
    ground = ch_FollowGround(a);
    ch_WalkSound(a, ground);
    if (ch->speedScale < 1.0)
        ch->speedScale += 0.01f;
    if (a->u.walk.path && !ch_NextWaypoint(a, h))
        return;
    if (!a->u.walk.path && (a->u.walk.flags & CH_WALK_DEST) && ch->slot && CH_XSLOT(ch)->f74_8) {
        ch_StopAnim(ch);
        if (ch_MarkWalkErrors)
            a->ch->flags.all |= 8;
        a->u.walk.flags |= CH_WALK_ARRIVED;
        d3_SetObjectPos(ch->obj, &a->u.walk.dest);
        ch_PlaceOnGround(ch, d3_CurrentScene != 0);
        return;
    }
    ch_WalkRotate(a);
    if (ground && ground != 13 && ch->slot) {
        f = 0.7f;
        if (ch->group == -1 && ch->obj->owner)
            f = cm_SpeedFactor;
        if (ch->kind & 8) {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = ch->walkSpeed * ch->speedScale * f * 1.9f;
            else
                CH_XSLOT(ch)->playSpeed = ch->walkSpeed * ch->speedScale * f * 1.7f;
        } else {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = ch->walkSpeed * ch->speedScale * f * 2.5f;
            else
                CH_XSLOT(ch)->playSpeed = ch->walkSpeed * ch->speedScale * f * 2.2f;
        }
    }
}
/* 0x409bd0, action callback 0x3a: walks along the cells of a->u.walk.path (0x100 cells, filled by the caller;
   empty cells are skipped) to the cell args[0], args[1] in scene args[2]
   @deep Read in full against 0x409bd0..0x40a8fa (the retry wait, the first-step setup, the walk-animation
   start, the morph release, the cell steps with the gap skip, the turn steering of both directions, the
   missed-threshold check, the play speed). Meaning fix: both turn branches scale by 0.19999999f (the
   original's 0x5eb860 is 0x3e4ccccc, not 0.2f); D3OBJ_UNK200 replaced by D3OBJECT.owner. All statements
   line up (ours 963 vs 965). Left: (1) the same redundant slot test as ch_ActWalk after 'test byte
   [eax+0x6d],0x20' at +0x1df ('+ jne / cmp eax,ebx / jne' vs '- je'); (2) the gap-skip loop (+0x632): the
   original keeps n in edx and recomputes min(0x100, n + 32) at the loop head each iteration (non-rotated,
   'jmp 0x40a224'), ours hoists it and spills n to [esp+0x18], hence our frame 0x1ac vs 0x1a8 and i in eax
   vs edi; a for(;;) with the bound tested by break compiles worse (58 -> 96). */

void ch_ActWalkCells(CHACTION *a)
{
    D3SM *h = (D3SM *)ch_GetHMap(a->ch);
    CHARACTER *ch = a->ch;
    int x, z, i, n, ground;
    CHPATHPT *pts, *pt;
    CHSLOT *s;
    D3OBJECT *o;
    float speed, f, ang, r, step, dx, dy, dz;
    double rot;
    D3VECTOR4 d, p;
    void *snd;
    D3ANIMDATA *ad;
    char name[0x100];
    char name2[0x80];

    if (a->u.walk.retryAt != -1) {
        if (ch_Time < a->u.walk.retryAt + 200)
            return;
        if (ch->slot)
            CH_XSLOT(ch)->playSpeed = 1.0f;
        a->u.walk.retryAt = -1;
    }
    if (!a->count) {
        if (ch->morph) {
            sprintf(name, "morph_%i", ch);
            d3_ReleaseTexture(ch->obj->material->animCtl, name);
            ad = (D3ANIMDATA *)d3_FindAnim(name);
            d3anim_Release(ad, 1);
            ch->morph = 0;
        }
        d3t_ReleaseObjectTextures(ch->obj);
        ch->speedScale = 1.0f;
        ch->next = 0;
        ch->morph = 0;
        ch->slot = 0;
        a->ch->flags.all &= ~8;
        a->u.walk.path = 0;
        memset(ch->contacts, 0, sizeof ch->contacts);
        x = a->args[0];
        z = a->args[1];
        if (d3t_SceneIndex(ch->scene) != a->args[2]) {
            ch_RemoveAction(a);
            return;
        }
        a->u.walk.path = mem_Alloc(0x200, "ch_t:waypoints");
        if (!a->u.walk.path)
            __asm int 3
        a->u.walk.pathLen = 0x100;
        a->u.walk.cur = 0;
        a->u.walk.prevCell = -1;
        for (i = 0; i < 0x200; i += 2) {
            ((unsigned char *)a->u.walk.path)[i] = 0;
            ((unsigned char *)a->u.walk.path)[i + 1] = 0;
        }
        d3sm_GetVertex(h, ((unsigned char *)a->u.walk.path)[0], ((unsigned char *)a->u.walk.path)[1], &a->u.walk.end);
        d3sm_GetVertex(h, x, z, &a->u.walk.end);
        a->u.walk.started = 0;
        return;
    }
    if (!a->u.walk.started && (!ch->slot || CH_XSLOT(ch)->f6d_20 || !ch->slot)) {
        ch->next = 0;
        if (ch->morph) {
            sprintf(name, "morph_%i", ch);
            d3_ReleaseTexture(ch->obj->material->animCtl, name);
            ad = (D3ANIMDATA *)d3_FindAnim(name);
            d3anim_Release(ad, 1);
            ch->morph = 0;
        }
        d3t_ReleaseObjectTextures(ch->obj);
        if (!ch->obj->info)
            ch_InflateModel(ch);
        if (ch->transport)
            s = (CHSLOT *)ch_AttachAni(ch, "bewegung/karren_ziehen", 1);
        else
            s = (CHSLOT *)ch_AttachAni(ch, "bewegung/gehen", 1);
        if (!(ch->slot = (D3ANIMSLOT *)s)) {
            if (ch_MarkWalkErrors)
                a->ch->flags.bytes.lo |= 8;
            if (a->u.walk.path) {
                mem_Free(a->u.walk.path);
                a->u.walk.path = 0;
            }
            ch_RemoveAction(a);
            return;
        }
        s->f6d_10 = 0;
        s = CH_XSLOT(ch);
        s->b70 = 0;
        s->a70 = 1;
        CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
        CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
        CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
        if (ch->kind & 8)
            CH_XSLOT(ch)->speed = 30.0f;
        else if (ch->kind & 0x10)
            CH_XSLOT(ch)->speed = 60.0f;
        else
            CH_XSLOT(ch)->speed = 15.0f;
        CH_XSLOT(ch)->play = 1;
        CH_XSLOT(ch)->f74_8 = 0;
        CH_XSLOT(ch)->f74_4 = 1;
        CH_XSLOT(ch)->f6d_20 = 0;
        a->u.walk.started = 1;
    }
    if (a->stop) {
        sprintf(name2, "%s_%s", "bewegung/gehen", a->ch->body);
        vfs_ToSlashes(name2);
        if (ch->slot)
            d3_ReleaseTexture(a->ch->obj->material->animCtl, (char *)CH_XSLOT(ch)->data);
        ch->slot = 0;
        ch_FollowGround(a);
        if (ch_MarkWalkErrors)
            a->ch->flags.all |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return;
    }
    if (!a->u.walk.started && !ch->slot) {
        if (ch_MarkWalkErrors)
            a->ch->flags.all |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return;
    }
    if (!a->u.walk.started && ch->slot)
        return;
    ground = ch_FollowGround(a);
    if (ch->slot && ch_SndLib && d3_DefaultScene == ch->scene && ch_CurrentGroup == ch->group
        && ch->slot->frame != a->u.walk.lastframe) {
        if (ch->slot->frame == 4 || ch->slot->frame == 12 || ch->slot->frame == 18
            || ch->slot->frame == 24) {
            if ((ch->kind & 1) && ch_StepSounds < 3 && d3t_SceneIndex(ch->scene)) {
                if (ch->scene->map) {
                    strcpy(name, "Normal_s");
                    if (ground == 6 || ground == 11)
                        strcpy(name, "Stein_s");
                    else if (ground == 3)
                        strcpy(name, "Erde_s");
                    else if (ground == 4)
                        strcpy(name, "Wiese_s");
                    else if (ground == 8)
                        strcpy(name, "Stein_s");
                    else if (ground == 10)
                        strcpy(name, "Pfuetze_s");
                    snd = sb_GetSample(ch_SndLib, name);
                } else
                    snd = sb_GetSample(ch_SndLib, ch_StepSample);
                if (snd) {
                    d3snd_PlayAt(snd, ch->obj, 40, 500.0f);
                    ch_StepSounds++;
                }
            }
            a->u.walk.lastframe = ch->slot->frame;
        }
    }
    if (ch->speedScale < 1.0)
        ch->speedScale += 0.01f;
    if (!a->u.walk.cur || (ch->slot && CH_XSLOT(ch)->f74_8)) {
        pts = (CHPATHPT *)a->u.walk.path;
        i = a->u.walk.cur + 1;
        pt = &pts[i];
        if (!pt->x && !pt->z && i < a->u.walk.pathLen) {
            for (n = i; i < min(0x100, n + 32); i++, pt++) {
                if (pt->x || pt->z) {
                    a->u.walk.cur = i - 1;
                    goto next;
                }
            }
            if (i != n) {
                CH_XSLOT(ch)->play = 0;
                CH_XSLOT(ch)->f74_8 = 1;
                CH_XSLOT(ch)->f6d_20 = 1;
                return;
            }
        }
next:
        a->u.walk.cur++;
        if (a->u.walk.cur >= a->u.walk.pathLen) {
            ch_StopAnim(ch);
            if (ch_MarkWalkErrors)
                a->ch->flags.bytes.lo |= 8;
            if (a->u.walk.path) {
                mem_Free(a->u.walk.path);
                a->u.walk.path = 0;
            }
            ch_RemoveAction(a);
            return;
        }
        d3sm_GetVertex(h, pts[a->u.walk.cur].x, pts[a->u.walk.cur].z, &a->u.walk.end);
        a->u.walk.angle = d3_YawTo(ch->obj, &a->u.walk.end);
        o = a->ch->obj;
        dx = o->pos.x - a->u.walk.end.x;
        dy = o->pos.y - a->u.walk.end.y;
        dz = o->pos.z - a->u.walk.end.z;
        f = (float)sqrt(dx * dx + dy * dy + dz * dz);
        a->u.walk.dist = f;
        if (ch->slot) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1 && a->u.walk.pathLen > 1) {
                if (ch->kind & 8)
                    speed = 24.0f;
                else if (ch->kind & 0x10)
                    speed = 45.0f;
                else
                    speed = 8.0f;
            } else {
                if (ch->kind & 8)
                    speed = 45.0f;
                else if (ch->kind & 0x10)
                    speed = 60.0f;
                else
                    speed = 15.0f;
            }
            if (ch->scene->map)
                speed *= 3.0;
            CH_XSLOT(ch)->pos.x = a->u.walk.end.x;
            CH_XSLOT(ch)->pos.y = a->u.walk.end.y;
            CH_XSLOT(ch)->pos.z = a->u.walk.end.z;
            CH_XSLOT(ch)->speed = speed;
            CH_XSLOT(ch)->play = 1;
            CH_XSLOT(ch)->f74_8 = 0;
            CH_XSLOT(ch)->f6d_20 = 0;
        }
    }
    if (a->u.walk.angle < 0.0f) {
        o = a->ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (!(ang <= 0.04 && ang >= -0.04)) {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 1.4f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                r = rot - step * 0.32f;
            else if (fabs(ang) > 1.5707964f)
                r = rot - step * 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                r = rot - step * 0.14f;
            else if (fabs(ang) > 0.5235988f)
                r = rot - step * 0.08f;
            else
                r = rot - step * 0.04f;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r < ang + o->rotation.y)
                r = ang + o->rotation.y;
        } else {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else if (a->u.walk.angle > 0.0f) {
        o = a->ch->obj;
        ang = d3_YawTo(o, &a->u.walk.end);
        rot = o->rotation.y;
        a->u.walk.angle = ang;
        if (ang <= 0.04 && ang >= -0.04) {
            r = ang + o->rotation.y;
            a->u.walk.angle = 0.0f;
        } else {
            step = (ch_Time - ch_LastUpdate) / 1.1f;
            if (ch->kind & 8)
                step *= 0.4f;
            else if (ch->kind & 0x10)
                step *= 1.4f;
            if (fabs(ang) > 2.0943952f && !(ch->kind & 8))
                r = rot - step * 0.32f;
            else if (fabs(ang) > 1.5707964f)
                r = rot + step * 0.19999999f;
            else if (fabs(ang) > 1.0471976f)
                r = rot + step * 0.14f;
            else if (fabs(ang) > 0.5235988f)
                r = rot + step * 0.08f;
            else
                r = rot + step * 0.04f;
            r = (float)fmod(r, 6.2831855f);
            if (r < 0.0)
                r += 6.2831855f;
            if (r > ang + o->rotation.y)
                r = ang + o->rotation.y;
        }
        d3_SetObjectRotXYZ(o, o->rotation.x, r, o->rotation.z);
    } else {
        o = a->ch->obj;
        p.x = o->pos.x;
        p.y = o->pos.y;
        p.z = o->pos.z;
        d.x = p.x - a->u.walk.end.x;
        d.y = p.y - a->u.walk.end.y;
        d.z = p.z - a->u.walk.end.z;
        if ((float)sqrt(d.x * d.x + d.y * d.y + d.z * d.z) > a->u.walk.dist * 1.2) {
            if (a->u.walk.cur >= a->u.walk.pathLen - 1)
                ch_SetPos(a->ch, &a->u.walk.end, 0);
            if (ch->slot)
                CH_XSLOT(ch)->f74_8 = 1;
            sprintf(name2, "ch_WalkOnPath(): Have missed my treshhold... %s [%f %f]", a->ch->obj,
                    a->u.walk.end.x, a->u.walk.end.z);
        }
    }
    if (ground && ground != 13 && ch->slot) {
        f = 0.7f;
        if (ch->group == -1 && ch->obj->owner) {
            f = cm_SpeedFactor;
            if (!ch->transport)
                f += 0.2f;
        }
        if (ch->kind & 8) {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = (f * ch->walkSpeed) * ch->speedScale * 1.9f;
            else
                CH_XSLOT(ch)->playSpeed = (f * ch->walkSpeed) * ch->speedScale * 1.7f;
        } else {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = (f * ch->walkSpeed) * ch->speedScale * 2.5f;
            else
                CH_XSLOT(ch)->playSpeed = (f * ch->walkSpeed) * ch->speedScale * 2.2f;
        }
    }
}

/* ---- 0x40a900..0x40b8a0 (worker ch6) ---- */
/* The TU ends at 0x40b580; the script builtins after it are in evtc.c. */

/* 0x40a900, queues a walk to the object's position */
CHACTION *ch_WalkToObject(CHARACTER *ch, D3OBJECT *obj)
{
    CHACTION *a = 0;
    struct D3HMAP *h;
    int x, z;
    float y;
    D3VECTOR4 pos;

    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    h = obj->scene->hmap;
    if (!h)
        h = ch_GetSceneHMap(obj->scene);
    if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
        a = ch_AddAction(ch, 0x2d, x, z, d3t_SceneIndex(obj->scene));
        if (a) {
            a->u.walk.flags |= CH_WALK_DEST;
            a->u.walk.dest.x = pos.x;
            a->u.walk.dest.y = pos.y;
            a->u.walk.dest.z = pos.z;
        }
    }
    return a;
}
/* 0x40a9c0, action callback 0x39: walks to a random dummy object if the character is in scene args[0] */

void ch_ActWalk2RndDummy(CHACTION *a)
{
    struct D3HMAP *h;
    int cur, scene;
    D3OBJECT *obj;
    CHACTION *w;
    int x, z;
    float y;
    D3VECTOR4 pos;

    if (!a->count) {
        h = ch_GetHMap(a->ch);
        cur = d3_CurrentScene;
        scene = d3t_SceneIndex(a->ch->scene);
        if (scene != a->args[0]) {
            ch_RemoveAction(a);
            return;
        }
        d3if_SelectScene(scene, 1);
        obj = ch_FindDummy();
        if (obj && h) {
            d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
            if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
                w = ch_InsertAction(a->ch, a, 0x2d, x, z, scene);
                w->u.walk.flags |= CH_WALK_DEST;
                w->u.walk.dest.x = pos.x;
                w->u.walk.dest.y = pos.y;
                w->u.walk.dest.z = pos.z;
                strcpy(CHACT_NAME70(w), "Walk2RndDummy");
            }
        }
        ch_RemoveAction(a);
        d3if_SelectScene(cur, 1);
    }
}
/* 0x40aaf0, queues a walk to the object, then a turn to the object's direction */

void ch_WalkToObjectTurn(CHARACTER *ch, D3OBJECT *obj)
{
    struct D3HMAP *h;
    CHACTION *a;
    int x, z;
    float y, ang;
    D3VECTOR4 pos, dir;

    h = ch_GetHMap(ch);
    d3_PointToWorld(obj, &obj->pos, (D3VECTOR *)&pos);
    d3_DirToWorld(obj, &d3_ZAxis, (D3VECTOR *)&dir);
    ang = d3_VecAngle(&d3_ZAxis, (D3VECTOR *)&dir);
    if (d3_GetHeightAt((D3SM *)h, (D3VECTOR *)&pos, &x, &z, &y)) {
        a = ch_AddAction(ch, 0x2d, x, z, d3t_SceneIndex(obj->scene));
        a->u.walk.flags |= CH_WALK_DEST;
        a->u.walk.dest.x = pos.x;
        a->u.walk.dest.y = pos.y;
        a->u.walk.dest.z = pos.z;
        ch_AddAction(ch, 7, (int)(-ang * 180.0 / 3.1415927f), 1);
    }
}
/* 0x40abd0, action callback 0x37: shows (args[0] != 0) or hides the character */

void ch_ActSetVisible(CHACTION *a)
{
    if (!a->count) {
        ch_SetVisible(a->ch, a->args[0]);
        ch_RemoveAction(a);
    }
}
/* 0x40ac00, action callback 0x38: fades the character in (args[0] != 0) or out over 50 time units */

void ch_ActFade(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3TEXFLAGS f;
    float t;
    CHARACTER *others[32];
    D3TEXFLAGS g;

    if (!a->count) {
        if (!ch->obj->info)
            goto done;
        if (ch_CurrentGroup != ch->group || ch_CurrentScriptGroup != ch->scriptGroup) {
            if (ch->group != -1)
                ch_SetVisible(ch, 0);
            else
                ch_SetVisible(ch, a->args[0]);
            goto done;
        }
        ch_SetVisible(ch, 1);
        memset(&g, 0, sizeof(g));
        g.b[2] &= 0xfe;
        g.b[0] = 0xff;
        d3_SetObjectAlpha(ch->obj, ch->obj->info, g);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, g);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, g);
        CHACT_FSTART(a) = (float)(ch_Time - 1);
        ch->flags.all = ch->flags.all & ~CH_NOFADE | 0x40;
    }
    t = a->args[0] ? ((float)ch_Time - CHACT_FSTART(a)) * 0.02f : ai_FloatOne - ((float)ch_Time - CHACT_FSTART(a)) * 0.02f;
    if (!(t > ai_FloatZero))
        t = 0.0f;
    else if (!(t < ai_FloatOne))
        t = 1.0f;
    if (ch->scene == d3_DefaultScene && a->args[0] && !(ch->flags.all & CH_NOFADE) && t < 0.1
        && ch_FindNear(ch, others, 30.0f)) {
        CHACT_FSTART(a) = (float)(ch_Time - 1);
        others[0]->flags.all |= CH_NOFADE;
        ch_InsertAction(ch, a, 0x3b, 75);
        return;
    }
    t *= 255.0f;
    memset(&f, 0, sizeof(f));
    f.b[2] &= 0xfe;
    f.b[0] = (unsigned char)t;
    if (t != 255.0f && t != ai_FloatZero) {
        d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, f);
    }
    if (t == ai_FloatZero || t == 255.0f) {
        ch->flags.fading = 0;
        f.b[2] &= 0xfe;
        f.b[0] = 0xff;
        d3_SetObjectAlpha(ch->obj, ch->obj->info, f);
        if (ch->transport)
            d3_SetObjectAlpha(*ch->transport, (*ch->transport)->info, f);
        if (ch->lowPolyBody)
            d3_SetObjectAlpha(ch->lowPolyBody, ch->lowPolyBody->info, f);
        if (t == ai_FloatZero)
            ch_SetVisible(ch, 0);
        else if (t == 255.0f)
            ch_SetVisible(ch, 1);
    done:
        ch_RemoveAction(a);
    }
}
/* 0x40af50, action callback 0x3b: waits args[0] milliseconds */

void ch_ActWait(CHACTION *a)
{
    if (!a->count)
        a->args[1] = d3_Time;
    if (d3_Time > (unsigned long)(a->args[1] + a->args[0]) || a->stop)
        ch_RemoveAction(a);
}
/* 0x40af90, drops the character's current group and its script group (-1: none) and the two saved
   scenes; main_StartGame calls it when it starts a game */

void ch_Reset(void)
{
    ch_CurrentScriptGroup = ch_CurrentGroup = -1;
    ch_Saved1 = ch_Saved0 = 0;
}
/* 0x40afb0, allocates the action queue and declares the actions */

int ch_Open(void)
{
    ch_Queue = mem_Alloc(CH_QUEUE * sizeof(CHACTION), "ch:Open:ActionQueue");
    if (!ch_Queue)
        return 0;
    memset(ch_ActDefs, 0, sizeof(ch_ActDefs));
    memset(ch_List, 0, sizeof(ch_List));
    ch_DeclareAction(7, (CHACTFUNC)ch_ActTurn, 1, "bewegung/dreh_90_rechts", 2);
    ch_DeclareAction(0, (CHACTFUNC)ch_ActAnimOnce, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x17, (CHACTFUNC)ch_ActPlay, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x2d, (CHACTFUNC)ch_ActWalk, 1, "bewegung/gehen", 3);
    ch_DeclareAction(0x3a, (CHACTFUNC)ch_ActWalkCells, 1, "bewegung/gehen", 3);
    ch_DeclareAction(0x39, (CHACTFUNC)ch_ActWalk2RndDummy, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x2e, (CHACTFUNC)ch_ActPlay, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x2f, (CHACTFUNC)ch_ActSitDown, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x30, (CHACTFUNC)ch_ActStandUp, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x31, (CHACTFUNC)ch_ActTakingObject, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x32, (CHACTFUNC)ch_ActDroppingObject, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x33, (CHACTFUNC)ch_ActMove2Universe, 1, ch_EmptyName, 3);
    ch_DeclareAction(0x35, (CHACTFUNC)ch_ActTurnTo, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x36, (CHACTFUNC)ch_ActPlayOnce, 1, ch_EmptyName, 0);
    ch_DeclareAction(0x37, (CHACTFUNC)ch_ActSetVisible, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x38, (CHACTFUNC)ch_ActFade, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x3b, (CHACTFUNC)ch_ActWait, 1, ch_EmptyName, 1);
    ch_DeclareAction(0x34, (CHACTFUNC)ch_ActUsingGate, 1, ch_EmptyName, 5);
    memset(ch_Fades, 0, sizeof(ch_Fades));
    ch_PathMap = ch_MapAlloc(60.0f);
    return 1;
}
/* 0x40b1a0, frees the walk paths, the characters and the action queue */

void ch_Close(void)
{
    int i;

    if (ch_PathMap)
        ch_MapFree(ch_PathMap);
    if (ch_Queue) {
        for (i = 0; i < CH_QUEUE; i++) {
            if (ch_Queue[i].type == '-' && ch_Queue[i].u.walk.path) {
                void **p = &ch_Queue[i].u.walk.path;

                mem_Free(*p);
                *p = 0;
            }
        }
    }
    for (i = 0; i < CH_MAX; i++)
        if (ch_List[i])
            ch_Delete(ch_List[i]);
    if (ch_Queue) {
        mem_Free(ch_Queue);
        ch_Queue = 0;
    }
}
/* 0x40b240, deletes every live character and empties ch_List (ls_LoadGame clears the old game with it) */

void ch_DeleteAll(void)
{
    int i;

    for (i = 0; i < CH_MAX; i++) {
        if (ch_List[i]) {
            ch_Delete(ch_List[i]);
            ch_List[i] = 0;
        }
    }
}
/* 0x40b270, a cleared queue entry appended to the character's queue */

CHACTION *ch_NewAction(CHARACTER *ch)
{
    CHACTION *a, *p;

    a = ch_GetFreeQueueEntry();
    if (!a) {
        err_Message("ch_InsertAction(): Could not ch_GetFreeQueueEntry()");
        return 0;
    }
    memset(a, 0, sizeof(*a));
    if (!CH_ACTIONS(ch))
        CH_ACTIONS(ch) = a;
    else {
        for (p = CH_ACTIONS(ch); p->next; p = p->next)
            ;
        p->next = a;
        a->prev = p;
    }
    a->ch = ch;
    a->count = 0;
    a->func = 0;
    a->next = 0;
    a->started = 0;
    ch_CheckQueue(ch, a);
    return a;
}
/* 0x40b2f0, appends action type to the character's queue; the action's arguments follow */

CHACTION *ch_AddAction(CHARACTER *ch, unsigned int type, ...)
{
    CHACTION *a;
    int i;
    va_list args;

    if (!ch)
        return 0;
    if (!ch_ActDefs[type].func)
        type = 0;
    a = ch_NewAction(ch);
    if (!a)
        return 0;
    a->func = ch_ActDefs[type].func;
    a->type = type;
    a->ch = ch;
    a->count = 0;
    va_start(args, type);
    for (i = 0; i < ch_ActDefs[a->type].nargs; i++)
        a->args[i] = va_arg(args, int);
    a->next = 0;
    a->started = 0;
    if (type == 0x33) {
        if (a->args[1] != -1 && a->args[2] == -1)
            __asm int 3
        if (!a->args[0] && a->args[1] != -1 || a->args[0] && a->args[1] == -1) {
            ch_RemoveAction(a);
            return 0;
        }
    }
    ch_CheckQueue(ch, a);
    if (a->func == (CHACTFUNC)ch_ActAnimOnce)
        a->args[0] = a->type;
    return a;
}
/* 0x40b400, like ch_AddAction, with func2 called at the animation frame "frame"; no queue check */

CHACTION *ch_AddActionCB(CHARACTER *ch, CHACTFUNC func2, unsigned char frame, unsigned int type, ...)
{
    CHACTION *a;
    int i;
    va_list args;

    if (!ch)
        return 0;
    if (!ch_ActDefs[type].func)
        type = 0;
    a = ch_NewAction(ch);
    if (!a)
        return 0;
    a->func = ch_ActDefs[type].func;
    a->type = type;
    a->ch = ch;
    a->count = 0;
    va_start(args, type);
    for (i = 0; i < ch_ActDefs[a->type].nargs; i++)
        a->args[i] = va_arg(args, int);
    a->next = 0;
    a->started = 0;
    a->func2 = func2;
    a->func2Frame = frame;
    if (a->func == (CHACTFUNC)ch_ActAnimOnce)
        a->args[0] = a->type;
    return a;
}
/* 0x40b4d0, drops the frame callbacks of all actions of characters in the scene */

void ch_ClearSceneCallbacks(D3SCENE *scene)
{
    int i;

    for (i = 0; i < CH_QUEUE; i++) {
        if (ch_Queue[i].type && ch_Queue[i].func && ch_Queue[i].ch && ch_Queue[i].ch->scene == scene
            && ch_Queue[i].func2)
            ch_Queue[i].func2 = 0;
    }
}
/* 0x40b530, attaches the sitting animation at its last frame */

void ch_SetSitAnim(CHARACTER *ch, char *name)
{
    D3ANIMSLOT *s;

    ch->sitSlot = s = (D3ANIMSLOT *)ch_AttachAni(ch, name, 1);
    ch->slot = 0;
    if (s) {
        d3anim_SetFrame(ch->obj, s, -1);
        ch->sitSlot->flags.b[0] = 1;
        ch->sitSlot->rootFlags &= ~3;   /* the sit animation must not move the character */
        ch->sitSlot->flags.b[1] |= 0x10;
    }
}
