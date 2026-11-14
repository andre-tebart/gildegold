/* d3_anim.c -- original file name unknown; the allocation tags are "d3_anim:CalculateAnimNormals...".
   Vertex animations: a cache of loaded animations (LRU list, purged above d3anim_CacheMax bytes) and
   up to three animation slots per animated object, root motion, bone posing and object path animations.
   Extent in GildeGold.exe: 0x4315c0 (after vfs_Exit) .. 0x4359f0, where d3_d3d.c starts.
   Functions are kept in their original order. Names are ours.
   Open: D3MATERIAL+0xf4 is the animation control block (read here through the local D3ANIMMATV view); typing it
   in d3.h moves d3_Unk4550e0 by 8 bytes. D3OBJECT.path (0x1d0) is typed in d3.h and d3anim_CreatePath and d3anim_FreePath use it,
   the other path users still go through the D3_OBJPATH raw cast.
   unit of rea 8cef7d5b (src/game/d3_anim.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "d3_anim.h"
#include "d3_engine.h"
#include "d3_light.h"
#include "d3_vec.h"
#include "mem.h"
#include "ts_texture.h"
#include "d3_pose.h"

extern D3ANIMDATA d3anim_Head;              /* 0xfc2580, list head; d3anim_Head.next at 0xfc26e0 */
extern D3ANIMDATA d3anim_Tail;              /* 0xfe7440, list end */
unsigned int d3anim_CacheMax = 4194304;  /* 0x5f927c, bytes */
extern int d3_Busy;                     /* 0x633b04 */
extern const float ai_FloatOne;         /* 0x5eb780, the shared 1.0f (bio.c) */

/* 0x4315c0, applies the root motion between two key frames: adds the frames' move delta (flags bit 0), in
   object space, to the object position, and the turn delta (flags bit 1) to its rotation. No return value */
void d3anim_ApplyRootMotion(D3OBJECT *obj, D3ANIMSLOT *s, int from, int to, int flags)
{
    D3VECTOR4 d, v;

    if ((flags & 1) == 1) {
        d.x = s->data->frames[to].move.x - s->data->frames[from].move.x;
        d.y = s->data->frames[to].move.y - s->data->frames[from].move.y;
        d.z = s->data->frames[to].move.z - s->data->frames[from].move.z;
        v.x = d.x * obj->matrix._11 + d.y * obj->matrix._21 + d.z * obj->matrix._31;
        v.y = d.x * obj->matrix._12 + d.y * obj->matrix._22 + d.z * obj->matrix._32;
        v.z = d.x * obj->matrix._13 + d.y * obj->matrix._23 + d.z * obj->matrix._33;
        v.x += obj->pos.x;
        v.y += obj->pos.y;
        v.z += obj->pos.z;
        d3_SetObjectPos(obj, (D3VECTOR *)&v);
    }
    if ((flags & 2) == 2) {
        d.x = s->data->frames[to].turn.x - s->data->frames[from].turn.x;
        d.y = s->data->frames[to].turn.y - s->data->frames[from].turn.y;
        d.z = s->data->frames[to].turn.z - s->data->frames[from].turn.z;
        d.x += obj->rotation.x;
        d.y += obj->rotation.y;
        d.z += obj->rotation.z;
        d3_SetObjectRot(obj, (D3VECTOR *)&d);
    }
}
/* 0x431750, applies the root-motion translation from the slot's last position (lastFrame/lastSub) up to
   frame/sub (flags bit 0), then stores that position in the slot. No return value */

void d3anim_ApplyMotionTo(D3OBJECT *obj, D3ANIMSLOT *s, int sub, int frame, int flags)
{
    D3ANIMDATA *data = s->data;
    int key = s->lastFrame;
    int sub0 = s->lastSub;
    int n1 = data->frames[frame].count;
    int n0 = data->frames[key].count;
    D3VECTOR4 d, sum;
    float f;
    int i;

    if (sub != sub0 || frame != key) {
        if ((flags & 1) == 1) {
            if (key < frame) {
                f = 1.0f - (float)sub0 / n0;
                d.x = data->frames[key + 1].move.x - data->frames[key].move.x;
                d.y = data->frames[key + 1].move.y - data->frames[key].move.y;
                d.z = data->frames[key + 1].move.z - data->frames[key].move.z;
                sum.x = d.x * f;
                sum.y = d.y * f;
                sum.z = d.z * f;
            } else {
                sum.x = 0.0f;
                sum.y = 0.0f;
                sum.z = 0.0f;
            }
            for (i = key + 1; i < frame; i++) {
                d.x = data->frames[i + 1].move.x - data->frames[i].move.x;
                d.y = data->frames[i + 1].move.y - data->frames[i].move.y;
                d.z = data->frames[i + 1].move.z - data->frames[i].move.z;
                sum.x += d.x;
                sum.y += d.y;
                sum.z += d.z;
            }
            if (key == frame)
                f = (float)(sub - sub0) / n1;
            else
                f = (float)sub / n1;
            d.x = data->frames[frame + 1].move.x - data->frames[frame].move.x;
            d.y = data->frames[frame + 1].move.y - data->frames[frame].move.y;
            d.z = data->frames[frame + 1].move.z - data->frames[frame].move.z;
            d.x *= f;
            d.y *= f;
            d.z *= f;
            sum.x += d.x;
            sum.y += d.y;
            sum.z += d.z;
            d.x = sum.x * obj->matrix._11 + sum.y * obj->matrix._21 + sum.z * obj->matrix._31;
            d.y = sum.x * obj->matrix._12 + sum.y * obj->matrix._22 + sum.z * obj->matrix._32;
            d.z = sum.x * obj->matrix._13 + sum.y * obj->matrix._23 + sum.z * obj->matrix._33;
            d.x += obj->pos.x;
            d.y += obj->pos.y;
            d.z += obj->pos.z;
            d3_SetObjectPos(obj, (D3VECTOR *)&d);
        }
        s->lastFrame = frame;
        s->lastSub = sub;
    }
}
/* 0x4319e0, maps the bones of the object's skeleton animations to its child objects (by name). */

char d3anim_MapBones(D3OBJECT *obj)
{
    D3BONESLOT *bs;
    D3BONEANIM *anim;
    D3OBJECT *c;
    D3MATERIAL *mat;
    int i, k, n;

    if (obj && (mat = obj->material)) {
        for (i = 0; i < 3; i++) {
            bs = (D3BONESLOT *)&mat->tex[i];
            if (anim = bs->anim) {
                bs->mapAll = -1;
                n = 0;
                for (k = 0; n < 4; k++) {
                    if (k >= anim->nbones)
                        break;
                    for (c = obj->child; c; c = c->next)
                        if (!strcmp(D3_MESH(c)->boneName, anim->names[k]))
                            bs->map[n++] = k;
                }
            }
        }
    }
    return 0;
}

typedef struct D3BONEACC {          /* 0xa4 bytes, a bone blended over the running skeleton animations */
    char name[0x40];
    int n;                          /* 0x40, animations added */
    D3VECTOR4 pos;                  /* 0x44 */
    D3VECTOR4 base;                 /* 0x54, rest position */
    D3VECTOR4 m[4];                 /* 0x64, rotation matrix (rows) */
} D3BONEACC;

/* 0x431ae0, poses the child objects (bones) from the skeleton animations of the object's slots, weighted;
   without a running animation, the bones get the mesh's rest pose. Per bone the first slot sets the accumulator
   (base = rest position, pos = base + weight * (lerped pos - base), m = lerped rotation rows), later slots add
   weight * (lerped pos - pos) and their rows; the rows are averaged before the angles are taken. The weight is
   written parenthesised, (s->weight) * v.x: VC6 orders the two fmul operands by it (weight loaded first, as in the
   original - plausibly a macro argument there). The 1.0f of the average is the pooled literal: written as
   ai_FloatOne (the same address) VC6 orders the code differently. */
void d3anim_PoseBones(D3OBJECT *obj)
{
    D3BONEACC acc[4];
    D3VECTOR4 m1[4], m0[4];
    D3ANIMSLOT *s;
    D3ANIMDATA *data;
    D3ANIMMESH *mesh;
    D3OBJECT *c;
    char *name;
    float t;
    int n, i, k, b, bone, r;

    if (obj && obj->material && obj->material->ops) {
        n = 0;
        if (((D3ANIMCTL *)obj->material->animCtl)->active) {
            D3_ZERO(acc, sizeof(acc));
            for (k = 0; k < 3; k++) {
                s = &((D3ANIMCTL *)obj->material->animCtl)->slots[k];
                if (data = s->data) {
                    D3VECTOR4 v;

                    for (b = 0; b < 4; b++) {
                        if (s->map[b] < 0xff) {
                            bone = s->map[b];
                            name = (char *)data + (bone + 1) * 0x40;
                            for (i = 0; i < n; i++)
                                if (!strcmp(acc[i].name, name))
                                    break;
                            if (i >= n) {
                                if (n < 4) {
                                    D3ANIMMESH *skel = ((D3ANIMCTL *)obj->material->animCtl)->mesh;

                                    strcpy(acc[n].name, name);
                                    for (i = 0; i < skel->nbones; i++) {
                                        if (!strcmp(skel->bones[i].name, name)) {
                                            acc[n].base.x = skel->bones[i].pos.x;
                                            acc[n].base.y = skel->bones[i].pos.y;
                                            acc[n].base.z = skel->bones[i].pos.z;
                                            break;
                                        }
                                    }
                                    t = (float)s->subFrame / data->frames[s->frame].count;
                                    d3_Lerp3((D3VECTOR4 *)&data->frames[s->frame].bone[bone].pos,
                                                 (D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].pos, t, &v);
                                    v.x -= acc[n].base.x;
                                    v.y -= acc[n].base.y;
                                    v.z -= acc[n].base.z;
                                    acc[n].pos.x = ((s->weight) * v.x);
                                    acc[n].pos.y = ((s->weight) * v.y);
                                    acc[n].pos.z = ((s->weight) * v.z);
                                    acc[n].pos.x = acc[n].pos.x + acc[n].base.x;
                                    acc[n].pos.y = acc[n].pos.y + acc[n].base.y;
                                    acc[n].pos.z = acc[n].pos.z + acc[n].base.z;
                                    d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->frame].bone[bone].rot, (D3MATRIX *)m0);
                                    d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].rot, (D3MATRIX *)m1);
                                    for (r = 0; r < 3; r++)
                                        d3_Lerp3(&m0[r], &m1[r], t, &acc[n].m[r]);
                                    acc[n].n = 1;
                                    n++;
                                }
                            } else {
                                t = (float)s->subFrame / data->frames[s->frame].count;
                                d3_Lerp3((D3VECTOR4 *)&data->frames[s->frame].bone[bone].pos,
                                             (D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].pos, t, &v);
                                v.x -= acc[i].pos.x;
                                v.y -= acc[i].pos.y;
                                v.z -= acc[i].pos.z;
                                acc[i].pos.x += (s->weight) * v.x;
                                acc[i].pos.y += (s->weight) * v.y;
                                acc[i].pos.z += (s->weight) * v.z;
                                d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->frame].bone[bone].rot, (D3MATRIX *)m0);
                                d3_MatFromAngles((D3VECTOR4 *)&data->frames[s->nextFrame].bone[bone].rot, (D3MATRIX *)m1);
                                for (r = 0; r < 3; r++) {
                                    d3_Lerp3(&m0[r], &m1[r], t, &v);
                                    acc[i].m[r].x += v.x;
                                    acc[i].m[r].y += v.y;
                                    acc[i].m[r].z += v.z;
                                }
                                acc[i].n++;
                            }
                        }
                    }
                }
            }
        }
        if (!n) {
            mesh = ((D3ANIMCTL *)obj->material->animCtl)->mesh;
            for (c = obj->child; c; c = c->next) {
                if (c->material && D3_MESH(c)->boneName[0]) {
                    for (i = 0; i < mesh->nbones; i++)
                        if (!strcmp(mesh->bones[i].name, D3_MESH(c)->boneName))
                            break;
                    if (i < mesh->nbones) {
                        d3_SetObjectPos(c, &mesh->bones[i].pos);
                        d3_SetObjectRot(c, &mesh->bones[i].rot);
                    }
                }
            }
        } else {
            float s;
            D3VECTOR rot;

            for (i = 0; i < n; i++) {
                s = 1.0f / acc[i].n;
                for (r = 0; r < 3; r++) {
                    acc[i].m[r].x *= s;
                    acc[i].m[r].y *= s;
                    acc[i].m[r].z *= s;
                }
                d3_MatToAngles(acc[i].m, (D3VECTOR4 *)&rot);
                for (c = obj->child; c; c = c->next) {
                    if (c->material && !strcmp(acc[i].name, D3_MESH(c)->boneName)) {
                        d3_SetObjectPos(c, (D3VECTOR *)&acc[i].pos);
                        d3_SetObjectRot(c, &rot);
                    }
                }
            }
        }
    }
}
/* 0x432190, the object's position at frame/sub of the slot's animation (root motion, without moving it): relative
   animations sum the frames' move deltas from the slot's last position (lastFrame / lastSub) up to frame / sub and
   rotate the sum by the object's matrix, the others interpolate between the slot's frame and nextFrame; posOffset
   is added to both. The else branch indexes data->frames[s->frame] at each use (no frame pointer locals).
   @original sum is not set when key >= frame (no else as in d3anim_ApplyMotionTo): for key == frame with another
   sub the sum reads uninitialised stack. */

void d3anim_RootPosAt(D3OBJECT *obj, D3ANIMSLOT *s, int frame, int sub, D3VECTOR *out)
{
    D3ANIMDATA *data = s->data;
    D3VECTOR4 d, sum;
    float t;
    int key, sub0, n0, n1, i;

    out->x = 0;
    out->y = 0;
    out->z = 0;
    if (data->relative) {
        n1 = data->frames[frame].count;
        key = s->lastFrame;
        sub0 = s->lastSub;
        n0 = data->frames[key].count;
        if (sub == sub0 && key == frame) {
            out->x = obj->posOffset.x + obj->pos.x;
            out->y = obj->posOffset.y + obj->pos.y;
            out->z = obj->posOffset.z + obj->pos.z;
            return;
        }
        if (key < frame) {
            t = ai_FloatOne - (float)sub0 / n0;
            d.x = data->frames[key + 1].move.x - data->frames[key].move.x;
            d.y = data->frames[key + 1].move.y - data->frames[key].move.y;
            d.z = data->frames[key + 1].move.z - data->frames[key].move.z;
            sum.x = d.x * t;
            sum.y = d.y * t;
            sum.z = d.z * t;
        }
        for (i = key + 1; i < frame; i++) {
            d.x = data->frames[i + 1].move.x - data->frames[i].move.x;
            d.y = data->frames[i + 1].move.y - data->frames[i].move.y;
            d.z = data->frames[i + 1].move.z - data->frames[i].move.z;
            sum.x += d.x;
            sum.y += d.y;
            sum.z += d.z;
        }
        if (key == frame) {
            sub -= sub0;
            t = (float)sub / n1;
        } else
            t = (float)sub / n1;
        d.x = data->frames[frame + 1].move.x - data->frames[frame].move.x;
        d.y = data->frames[frame + 1].move.y - data->frames[frame].move.y;
        d.z = data->frames[frame + 1].move.z - data->frames[frame].move.z;
        d.x *= t;
        d.y *= t;
        d.z *= t;
        sum.x += d.x;
        sum.y += d.y;
        sum.z += d.z;
        d.x = sum.x * obj->matrix._11 + sum.y * obj->matrix._21 + sum.z * obj->matrix._31;
        d.y = sum.x * obj->matrix._12 + sum.y * obj->matrix._22 + sum.z * obj->matrix._32;
        d.z = sum.x * obj->matrix._13 + sum.y * obj->matrix._23 + sum.z * obj->matrix._33;
        out->x = d.x + obj->pos.x;
        out->y = d.y + obj->pos.y;
        out->z = d.z + obj->pos.z;
    } else {
        d.x = data->frames[s->frame].move.x;
        d.y = data->frames[s->frame].move.y;
        d.z = data->frames[s->frame].move.z;
        sum.x = data->frames[s->nextFrame].move.x - d.x;
        sum.y = data->frames[s->nextFrame].move.y - d.y;
        sum.z = data->frames[s->nextFrame].move.z - d.z;
        t = (float)s->subFrame / data->frames[s->frame].count;
        d.x = sum.x * t + d.x;
        d.y = sum.y * t + d.y;
        d.z = sum.z * t + d.z;
        sum.x = d.x * obj->matrix._11 + d.y * obj->matrix._21 + d.z * obj->matrix._31;
        sum.y = d.x * obj->matrix._12 + d.y * obj->matrix._22 + d.z * obj->matrix._32;
        sum.z = d.x * obj->matrix._13 + d.y * obj->matrix._23 + d.z * obj->matrix._33;
        out->x = sum.x + obj->pos.x;
        out->y = sum.y + obj->pos.y;
        out->z = sum.z + obj->pos.z;
    }
    out->x = obj->posOffset.x + out->x;
    out->y = obj->posOffset.y + out->y;
    out->z = obj->posOffset.z + out->z;
}
/* 0x4324e0, the next frame after frame (reverse, loop and ping-pong flags in f.b[1]). */

int d3anim_NextFrame(D3ANIMFLAGS f, int frame, int a, int b, int c)
{
    if (f.b[1] & 2) {
        if (frame > a)
            return frame - 1;
        return a + 1;
    }
    if (frame >= b) {
        if (f.b[1] & 0x10) {
            if (frame + 1 <= c - 1)
                return frame + 1;
            return c - 1;
        }
        if (f.b[1] & 1)
            return frame - 1;
        return a;
    }
    return frame + 1;
}
/* 0x432530, spline tangents at k1 from the mean slope of the segments k0-k1 and k1-k2. */

void d3path_Tangents(D3PATHKEY *k0, D3PATHKEY *k1, D3PATHKEY *k2)
{
    D3VECTOR4 a, b;
    float t, f;

    a.x = k1->pos.x - k0->pos.x;
    a.y = k1->pos.y - k0->pos.y;
    a.z = k1->pos.z - k0->pos.z;
    f = 1.0f / k0->time;
    a.x *= f;
    a.y *= f;
    a.z *= f;
    b.x = k2->pos.x - k1->pos.x;
    b.y = k2->pos.y - k1->pos.y;
    b.z = k2->pos.z - k1->pos.z;
    t = (float)k1->time;
    f = 1.0f / t;
    b.x *= f;
    b.y *= f;
    b.z *= f;
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
    a.x *= 0.5f;
    a.y *= 0.5f;
    a.z *= 0.5f;
    k1->posin.x = t * a.x;
    k1->posin.y = t * a.y;
    k1->posin.z = t * a.z;
    f = (float)k0->time;
    k0->posout.x = f * a.x;
    k0->posout.y = f * a.y;
    k0->posout.z = f * a.z;

    a.x = k1->rot.x - k0->rot.x;
    a.y = k1->rot.y - k0->rot.y;
    a.z = k1->rot.z - k0->rot.z;
    f = 1.0f / f;
    a.x *= f;
    a.y *= f;
    a.z *= f;
    b.x = k2->rot.x - k1->rot.x;
    b.y = k2->rot.y - k1->rot.y;
    b.z = k2->rot.z - k1->rot.z;
    t = (float)k1->time;
    f = 1.0f / t;
    b.x *= f;
    b.y *= f;
    b.z *= f;
    a.x = b.x + a.x;
    a.y = b.y + a.y;
    a.z = b.z + a.z;
    a.x *= 0.5f;
    a.y *= 0.5f;
    a.z *= 0.5f;
    k1->rotin.x = t * a.x;
    k1->rotin.y = t * a.y;
    k1->rotin.z = t * a.z;
    f = (float)k0->time;
    k0->rotout.x = f * a.x;
    k0->rotout.y = f * a.y;
    k0->rotout.z = f * a.z;
}

/* The material of an animated mesh (D3MATERIAL view): up to five animation controls of 0x198 bytes from 0xf4. */
typedef struct D3ANIMMATV {
    char pad000[0xf4];
    D3ANIMCTL ctl[5];               /* 0xf4..0x8ec */
    char pad8ec[0x80];
    unsigned char nctl;             /* 0x96c (the original reads the byte there, not after 0xe0 bytes of pad) */
    unsigned char flags;            /* 0x96d, 0x40 = no vertex animation */
} D3ANIMMATV;

#define FRAME_AT(d, i) ((D3ANIMFRAME *)((char *)(d)->frames + (i) * 0xc0))

float d3_Hermite(float p0, float p1, float m0, float m1, float t);                      /* 0x453e20 */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);                                /* 0x458810 */
char tex_Animate(D3OBJECT *obj, unsigned int time);                                     /* 0x460b90 */
void d3_RelightObjectFrame(D3OBJECT *obj, D3ANIMFRAME *f);                                       /* 0x4535b0, d3_light.h's canonical prototype; d3_anim.c keeps this local copy, the include is WORSE for d3anim_CreatePath / d3anim_CreateMorphAnim (+2 instructions each) */
struct D3ANIMSLOT * d3anim_GetMainSlot(struct D3ANIMCTL *ctl);                                         /* 0x435900 */

/* the shared 1.0f both paths' key fractions are subtracted from (the original loads it from memory) */

/* 0x432750, the per-object animation step (d3_ForEachObject callback, ctx = d3_Time, | 0x80000000 in the
   second pass): steps the three vertex animation slots of every active control of the mesh material, then
   the object's path animation (hermite through the keys).
   @stuck match-deep 2026-11-14: frame 0x68 -> 0x9c (the original 0xbc) with every vector a block-local
   D3VECTOR4 (RootPosAt out, the attach block's v/w/out, the path's v/w/tin/tout; the original's holes
   after each 3-float group are the w slots); limit = data->nframes - 1 with nframes read after the if;
   fadeEnd <= now; meaning fix of the end-tangent clear (nkeys, not key/next). 1331 -> 1144 ins. Tried with
   no gain: late init of over/over2/shown, (ctx >> 31) & 1, tin/tout or the done block's v/w in their own
   blocks (same or worse). Read the whole path state machine against the original: same meaning. Left: the
   original reloads P->key / &keys[key] per statement in the linear and hermite path blocks (ours CSEs the
   address: line 760 region, 30 ins), the original keeps obj in memory (arg slot) and s in esi for the slot
   loop where ours has obj in ebx, i/j in slots [esp+0x74]/[esp+0x70] with a zero in edi, 0x20 bytes of
   frame still missing (unpaired original slots [esp+0x8c..0x94], [esp+0x9c], [esp+0xa0], [esp+0xc4]), ours
   1518 vs 1578 instructions (tiebreak: not measured (dcp headers sample d3anim_StepObject)) prior: read
   the whole 1578-instruction function against the original: one call site fewer (the merged NextFrame
   explained below), the same store and reference set (refs clean), the same 15-entry switch table. The
   grade is dominated by the frame: ours 0x68 against the original 0xbc, 84 bytes more locals and spills on
   the original side; the local layouts differ beyond a uniform shift, so the frame alignment cannot mask
   them and the 1074 code / 492 slots hunks are the same instructions reading other stack slots (e.g. the
   fade fraction temporaries at [esp+0x3c]/[esp+0x9c], the key pointer at [esp+0x18] vs [esp+0x70], the
   attach vector at [esp+0x64..] vs [esp+0x30..]). Q4 is settled from the reconstructed body: D3OBJECT+0x44
   is D3_L(o)->time (the light view, d3_light.h), the D3ANIMSLOT float slots are the attach position 0x20
   and the stop position 0x4c / radius 0x5c, the 2-bit field at 0x70 is the s... */
char d3anim_StepObject(D3OBJECT *obj, void *ctx)
{
    unsigned int now = (unsigned int)ctx & 0x7fffffff;
    char second = (unsigned int)ctx >> 31;
    unsigned int dt = now - ((D3ANIMOBJ *)obj)->time;
    D3ANIMCTL *c;
    D3ANIMSLOT *s;
    D3ANIMDATA *data;
    D3ANIMFRAME *shown = 0;
    D3PATH *P;
    D3PATHKEY *keys, *k0, *k1;
    int i, j, n, last, limit, nframes, nk, half;
    char done, over = 0, over2 = 0, released;
    float acc, t, u;
    unsigned char f1;

    ((D3ANIMOBJ *)obj)->time = now;
    if (dt == 0)
        return 1;
    if (obj->material) {
        if (!second && obj->info && !(((D3ANIMMATV *)obj->material)->flags & 0x40) && obj->type == 4)
            tex_Animate(obj, now);
        for (i = 0; i < ((D3ANIMMATV *)obj->material)->nctl; i++) {
            if (!((D3ANIMMATV *)obj->material)->ctl[i].active)
                continue;
            for (j = 0; j < 3; j++) {
                s = &((D3ANIMMATV *)obj->material)->ctl[i].slots[j];
                data = s->data;
                if (!data || !(s->flags.b[8] & 2))
                    continue;
                if (now - D3_L(obj)->time > (unsigned int)s->timeout)
                    over = 1;
                last = data->first;
                if (s->flags.b[1] & 0x10) {
                    limit = data->nframes - 1;
                    if (!data->relative)
                        s->lastFrame = 0;
                } else
                    limit = data->last;
                nframes = data->nframes;
                s->flags.b[1] &= ~0x20;
                data->lastuse = d3_FrameCount;
                d3_UpdateObject(obj, 1);
                done = 0;
                if (s->fadeStart != s->fadeEnd) {
                    if ((unsigned int)s->fadeEnd <= now) {
                        s->fadeEnd = 0;
                        s->fadeStart = 0;
                        s->weight = s->fadeTo;
                        if (s->weight == 0.0)
                            done = 1;
                    } else
                        s->weight = (float)(now - s->fadeStart) / (s->fadeEnd - s->fadeStart) * (s->fadeTo - s->fadeFrom) + s->fadeFrom;
                }
                acc = ((s->flags.b[8] >> 1) & 1) * s->speed * dt + s->frameTime;
                s->frameTime = acc;
                if (s->flags.b[1] & 2) {
                    n = (int)acc;
                    s->subFrame -= n;
                } else {
                    n = (int)acc;
                    s->subFrame += n;
                }
                s->frameTime = acc - n;
normalize:
                if (s->flags.b[1] & 2) {
                    n = s->subFrame;
                    if (n < 0) {
                        if (s->nextFrame <= last) {
                            s->flags.b[8] &= ~0x10;
                            if (s->flags.b[1] & 1) {
                                s->subFrame = -n;
                                s->frame = last;
                                s->flags.b[1] &= ~2;
                                s->nextFrame = d3anim_NextFrame(s->flags, last, last, limit, nframes);
                                if (s->flags.b[0] > 0) {
                                    s->flags.b[0]--;
                                    if (s->flags.b[0] == 1)
                                        s->flags.b[1] |= 0x10;
                                }
                            } else {
                                if (s->flags.b[0] > 0) {
                                    s->flags.b[0]--;
                                    if (s->flags.b[0] == 1)
                                        s->flags.b[1] |= 0x10;
                                }
                                s->frame = limit;
                                s->subFrame = FRAME_AT(data, limit)->count + n;
                                s->nextFrame = d3anim_NextFrame(s->flags, limit, last, limit, nframes);
                            }
                        } else {
                            s->frame--;
                            s->nextFrame = d3anim_NextFrame(s->flags, s->frame, last, limit, nframes);
                            s->subFrame = FRAME_AT(data, s->nextFrame)->count + n;
                        }
                    }
                } else {
                    n = FRAME_AT(data, s->frame)->count;
                    if (s->subFrame >= n) {
                        if (s->frame + 1 < limit) {
                            s->frame++;
                            s->subFrame -= n;
                        } else if (s->flags.b[1] & 0x10) {
                            done = 1;
                            s->subFrame = n - 1;
                        } else if (s->flags.b[1] & 1) {
                            s->flags.b[1] |= 2;
                            s->frame = s->nextFrame;
                            s->subFrame = n - ((s->flags.b[8] >> 1) & 1) - 1;
                            if (s->flags.b[0] > 0) {
                                s->flags.b[0]--;
                                if (s->flags.b[0] == 1)
                                    s->flags.b[1] |= 0x10;
                            }
                        } else {
                            s->flags.b[8] &= ~0x10;
                            if (s->flags.b[0] > 0) {
                                s->flags.b[0]--;
                                if (s->flags.b[0] == 1)
                                    s->flags.b[1] |= 0x10;
                            }
                            s->subFrame -= n;
                            s->frame = last;
                            if (!data->relative)
                                d3anim_ApplyRootMotion(obj, s, s->lastFrame, limit, s->turn);
                        }
                        s->nextFrame = d3anim_NextFrame(s->flags, s->frame, last, limit, nframes);
                    }
                }
                if (s->flags.b[8] & 4) {
                    D3VECTOR4 out;
                    n = FRAME_AT(data, s->frame)->count;
                    half = n / 2;
                    if (s->subFrame >= 0 && s->subFrame < n)
                        half = s->subFrame;
                    d3anim_RootPosAt(obj, s, s->frame, half, (D3VECTOR *)&out);
                    if (fabs(out.x - s->stopPos.x) < s->stopRadius && fabs(out.z - s->stopPos.z) < s->stopRadius) {
                        s->subFrame = half;
                        s->flags.b[8] |= 8;
                        done = 1;
                    }
                }
                if (!done) {
                    if (s->subFrame < 0)
                        goto normalize;
                    if (s->subFrame >= FRAME_AT(data, s->frame)->count && !(s->flags.b[1] & 2))
                        goto normalize;
                    if (s->subFrame >= FRAME_AT(data, s->nextFrame)->count && (s->flags.b[1] & 2))
                        goto normalize;
                }
                if (data->relative)
                    d3anim_ApplyMotionTo(obj, s, s->subFrame, s->frame, s->turn);
                if (s->attachFirst != -1 && s->attachLast != -1 && s->attachBone != -1 && s->attached) {
                    if (s->frame >= s->attachFirst) {
                        if (s->frame >= s->attachLast) {
                            if (s->flags.b[8] & 0x40) {
                                s->flags.b[8] &= ~0x40;
                                s->attached = 0;
                                s->attachFirst = s->attachLast = s->attachBone = -1;
                            }
                        } else {
                            D3OBJECT *att = (D3OBJECT *)s->attached;
                            D3VECTOR4 v, w, out;

                            if (!(s->flags.b[8] & 0x40)) {
                                s->flags.b[8] |= 0x40;
                                s->attachPos.x = att->pos.x;
                                s->attachPos.y = att->pos.y;
                                s->attachPos.z = att->pos.z;
                            }
                            t = (float)s->subFrame / FRAME_AT(data, s->frame)->count;
                            d3_Lerp3((D3VECTOR4 *)&FRAME_AT(data, s->frame)->bone[s->attachBone].pos,
                                         (D3VECTOR4 *)&FRAME_AT(data, s->nextFrame)->bone[s->attachBone].pos, t, (D3VECTOR4 *)&v);
                            w.x = FRAME_AT(data, s->attachFirst)->bone[s->attachBone].pos.x - v.x;
                            w.y = FRAME_AT(data, s->attachFirst)->bone[s->attachBone].pos.y - v.y;
                            w.z = FRAME_AT(data, s->attachFirst)->bone[s->attachBone].pos.z - v.z;
                            d3_DirToWorld(obj, (float *)&w, (float *)&v);
                            out.x = v.z * att->matrix._31 + v.y * att->matrix._21 + v.x * att->matrix._11;
                            out.y = v.z * att->matrix._32 + v.y * att->matrix._22 + v.x * att->matrix._12;
                            out.z = v.z * att->matrix._33 + v.y * att->matrix._23 + v.x * att->matrix._13;
                            out.x += s->attachPos.x;
                            out.y += s->attachPos.y;
                            out.z += s->attachPos.z;
                            d3_SetObjectPos(att, (D3VECTOR *)&out);
                        }
                    }
                }
                released = 0;
                if (done) {
                    s->flags.b[8] &= ~2;
                    if (!(s->flags.b[1] & 8)) {
                        released = 1;
                        if (!data->relative)
                            d3anim_ApplyRootMotion(obj, s, s->lastFrame, limit, s->turn);
                        d3_ReleaseTexture(&((D3ANIMMATV *)obj->material)->ctl[i], (char *)data);
                    } else
                        s->flags.b[1] |= 0x20;
                }
                if (over && !released)
                    shown = FRAME_AT(s->data, s->frame);
            }
        }
    }
    d3anim_PoseBones(obj);
    P = obj->path;
    if (P && (P->flags.b[8] & 2)) {
        D3VECTOR4 v, w, tin, tout;
        nframes = P->nkeys;
        P->flags.b[1] &= ~0x20;
        if (P->flags.b[1] & 2) {
            u = (float)P->time / P->keys[P->next].time;
            t = ai_FloatOne - u;
        } else {
            t = (float)P->time / P->keys[P->key].time;
            u = ai_FloatOne - t;
        }
        if (P->flags.b[1] & 4) {
            if (P->flags.b[1] & 2) {
                tin.x = -P->keys[P->next].posin.x; tin.y = -P->keys[P->next].posin.y; tin.z = -P->keys[P->next].posin.z;
                tout.x = -P->keys[P->next].posout.x; tout.y = -P->keys[P->next].posout.y; tout.z = -P->keys[P->next].posout.z;
                v.x = d3_Hermite(P->keys[P->key].pos.x, P->keys[P->next].pos.x, tout.x, tin.x, t);
                v.y = d3_Hermite(P->keys[P->key].pos.y, P->keys[P->next].pos.y, tout.y, tin.y, t);
                v.z = d3_Hermite(P->keys[P->key].pos.z, P->keys[P->next].pos.z, tout.z, tin.z, t);
                v.x += P->pos.x;
                v.y += P->pos.y;
                v.z += P->pos.z;
                d3_SetObjectPos(obj, (D3VECTOR *)&v);
                if (obj->type < 5) {
                    tin.x = -P->keys[P->next].rotin.x; tin.y = -P->keys[P->next].rotin.y; tin.z = -P->keys[P->next].rotin.z;
                    tout.x = -P->keys[P->next].rotout.x; tout.y = -P->keys[P->next].rotout.y; tout.z = -P->keys[P->next].rotout.z;
                    w.x = d3_Hermite(P->keys[P->key].rot.x, P->keys[P->next].rot.x, tout.x, tin.x, t);
                    w.y = d3_Hermite(P->keys[P->key].rot.y, P->keys[P->next].rot.y, tout.y, tin.y, t);
                    w.z = d3_Hermite(P->keys[P->key].rot.z, P->keys[P->next].rot.z, tout.z, tin.z, t);
                    w.x += P->rot.x;
                    w.y += P->rot.y;
                    w.z += P->rot.z;
                    d3_SetObjectRot(obj, (D3VECTOR *)&w);
                }
            } else {
                v.x = d3_Hermite(P->keys[P->key].pos.x, P->keys[P->next].pos.x, P->keys[P->key].posin.x, P->keys[P->key].posout.x, t);
                v.y = d3_Hermite(P->keys[P->key].pos.y, P->keys[P->next].pos.y, P->keys[P->key].posin.y, P->keys[P->key].posout.y, t);
                v.z = d3_Hermite(P->keys[P->key].pos.z, P->keys[P->next].pos.z, P->keys[P->key].posin.z, P->keys[P->key].posout.z, t);
                v.x += P->pos.x;
                v.y += P->pos.y;
                v.z += P->pos.z;
                d3_SetObjectPos(obj, (D3VECTOR *)&v);
                if (obj->type < 5) {
                    w.x = d3_Hermite(P->keys[P->key].rot.x, P->keys[P->next].rot.x, P->keys[P->key].rotin.x, P->keys[P->key].rotout.x, t);
                    w.y = d3_Hermite(P->keys[P->key].rot.y, P->keys[P->next].rot.y, P->keys[P->key].rotin.y, P->keys[P->key].rotout.y, t);
                    w.z = d3_Hermite(P->keys[P->key].rot.z, P->keys[P->next].rot.z, P->keys[P->key].rotin.z, P->keys[P->key].rotout.z, t);
                    w.x += P->rot.x;
                    w.y += P->rot.y;
                    w.z += P->rot.z;
                    d3_SetObjectRot(obj, (D3VECTOR *)&w);
                }
            }
        } else {
            v.x = u * P->keys[P->key].pos.x;
            v.y = u * P->keys[P->key].pos.y;
            v.z = u * P->keys[P->key].pos.z;
            v.x += t * P->keys[P->next].pos.x;
            v.y += t * P->keys[P->next].pos.y;
            v.z += t * P->keys[P->next].pos.z;
            if (!d3_VecEqual((D3VECTOR *)&v, &d3_Origin, 0.0f)) {
                v.x += P->pos.x;
                v.y += P->pos.y;
                v.z += P->pos.z;
                d3_SetObjectPos(obj, (D3VECTOR *)&v);
            }
            if (obj->type < 5) {
                w.x = u * P->keys[P->key].rot.x;
                w.y = u * P->keys[P->key].rot.y;
                w.z = u * P->keys[P->key].rot.z;
                w.x += t * P->keys[P->next].rot.x;
                w.y += t * P->keys[P->next].rot.y;
                w.z += t * P->keys[P->next].rot.z;
                w.x += P->rot.x;
                w.y += P->rot.y;
                w.z += P->rot.z;
                d3_SetObjectRot(obj, (D3VECTOR *)&w);
            }
        }
        f1 = P->flags.b[1];
        done = 0;
        if ((f1 & 4) && (f1 & 0x10) && !(f1 & 1) && !(P->flags.b[8] & 0x20) && P->key < P->nkeys - 1) {
            P->flags.b[8] |= 0x20;
            P->keys[P->nkeys - 1].posin.x = 0;
            P->keys[P->nkeys - 1].posin.y = 0;
            P->keys[P->nkeys - 1].posin.z = 0;
            P->keys[P->nkeys - 2].posout.x = 0;
            P->keys[P->nkeys - 2].posout.y = 0;
            P->keys[P->nkeys - 2].posout.z = 0;
            P->keys[P->nkeys - 1].rotin.x = 0;
            P->keys[P->nkeys - 1].rotin.y = 0;
            P->keys[P->nkeys - 1].rotin.z = 0;
            P->keys[P->nkeys - 2].rotout.x = 0;
            P->keys[P->nkeys - 2].rotout.y = 0;
            P->keys[P->nkeys - 2].rotout.z = 0;
        }
        if (now - D3_L(obj)->time > (unsigned int)P->maxStep)
            over2 = 1;
        acc = ((P->flags.b[8] >> 1) & 1) * P->fadeEnd * dt + P->frameTime;
        P->frameTime = acc;
        if (P->flags.b[1] & 2)
            P->time -= (int)acc;
        else
            P->time += (int)acc;
        P->frameTime = acc - (int)acc;
        for (;;) {
            f1 = P->flags.b[1];
            if (f1 & 2) {
                if (P->time < 0) {
                    if (P->next <= 0) {
                        if (f1 & 0x10) {
                            P->key = 0;
                            P->time = 0;
                            done = 1;
                        } else {
                            if (f1 & 1) {
                                P->time = -P->time;
                                P->flags.b[8] &= ~0x10;
                                P->flags.b[1] &= ~2;
                                P->key = 0;
                            } else {
                                P->key = nframes - 1;
                                P->time = P->keys[P->key].time + P->time;
                                P->flags.b[8] &= ~0x10;
                            }
                            if (P->flags.b[0] > 0) {
                                P->flags.b[0]--;
                                if (P->flags.b[0] == 1)
                                    P->flags.b[1] |= 0x10;
                            }
                        }
                    } else if (P->key - 1 == 0 && (f1 & 0x10)) {
                        done = 1;
                        P->key = 0;
                        P->time = 0;
                    } else {
                        P->flags.b[8] &= ~0x10;
                        P->time += P->keys[P->next].time;
                        P->key = P->key - 1;
                    }
                    P->next = d3anim_NextFrame(P->flags, P->key, 0, nframes - 1, nframes);
                    goto stepped;
                }
            } else if (P->time >= P->keys[P->key].time) {
                int kt = P->keys[P->key].time;

                if (P->key + 1 >= nframes - 1 && (f1 & 1)) {
bounce:
                    P->flags.b[1] |= 2;
                    P->key = P->next;
                    P->time = kt - ((P->flags.b[8] >> 1) & 1) - 1;
                } else if (P->key < nframes - 1 && !(P->key + 1 >= nframes - 1 && (f1 & 0x10) && !(f1 & 1))) {
                    if ((P->flags.b[8] & 0x10) && !(f1 & 0x11) && (f1 & 4)) {
                        d3path_Tangents(&P->keys[P->nkeys - 1], P->keys, &P->keys[1]);
                        P->flags.b[8] &= ~0x10;
                    }
                    P->time -= P->keys[P->key].time;
                    P->key++;
                } else if (f1 & 0x10) {
                    if (f1 & 1)
                        goto bounce;
                    done = 1;
                    P->key = P->nkeys - 1;
                    P->time = P->keys[P->key].time - 1;
                } else if (f1 & 1)
                    goto bounce;
                else {
                    P->time -= kt;
                    P->key = 0;
                    if (P->flags.b[0] > 0) {
                        P->flags.b[0]--;
                        if (P->flags.b[0] == 1)
                            P->flags.b[1] |= 0x10;
                    }
                }
                P->next = d3anim_NextFrame(P->flags, P->key, 0, nframes - 1, nframes);
stepped:
                if (done) {
                    v.x = P->keys[P->next].pos.x + P->pos.x;
                    v.y = P->keys[P->next].pos.y + P->pos.y;
                    v.z = P->keys[P->next].pos.z + P->pos.z;
                    d3_SetObjectPos(obj, (D3VECTOR *)&v);
                    w.x = P->keys[P->next].rot.x + P->rot.x;
                    w.y = P->keys[P->next].rot.y + P->rot.y;
                    w.z = P->keys[P->next].rot.z + P->rot.z;
                    d3_SetObjectRot(obj, (D3VECTOR *)&w);
                    if (!(P->flags.b[1] & 8))
                        d3anim_FreePath(obj);
                    else {
                        P->flags.b[8] &= ~2;
                        P->flags.b[1] |= 0x20;
                    }
                    break;
                }
            }
            if (P->time >= 0 && !(!(P->flags.b[1] & 2) && P->time >= P->keys[P->key].time)
                && !((P->flags.b[1] & 2) && P->time >= P->keys[P->next].time))
                break;
        }
    }
    if (over2 || over) {
        if (!second && obj->type == 4 && obj->material && obj->info && !(((D3ANIMMATV *)obj->material)->flags & 0x40)) {
            D3ANIMFRAME *f = shown;

            if (f) {
                D3ANIMSLOT *main = d3anim_GetMainSlot((D3ANIMCTL *)obj->info);

                if (main)
                    f = FRAME_AT(main->data, main->frame);
            }
            d3_RelightObjectFrame(obj, f);
        }
    }
    return 1;
}
/* 0x433b10, frees the object's path animation (its keys, then the D3PATH) and clears obj->path */

void d3anim_FreePath(D3OBJECT *obj)
{
    if (obj->path) {
        mem_Free(obj->path->keys);
        obj->path->keys = 0;
        mem_Free(obj->path);
        obj->path = 0;
    }
}
/* 0x433b50, spline tangents of all keys (flags.b[1] & 4: smooth); b[8] & 1 wraps the angles first. @deep B draft: 46 code hunks, dominated by the strength-reduced key offset in the first loop ([edi+ecx+0x20] ours vs [ecx+edi+0x20] original); the attempts ((p->keys + i)->, i[p->keys], 0.0f vs 0) failed - deep matching. Q2: the callee 0x453d60 is d3_UnwrapAngles in d3_vec.c, but d3.h declares the D3VECTOR* view as d3_UnwrapAngles, so naming it needs a shared-header decision. */

void d3path_Init(D3PATH *p)
{
    int i;

    if (p && (p->flags.b[1] & 4)) {
        if (p->nkeys <= 2) {
            for (i = 0; i < p->nkeys; i++) {
                if ((p->flags.b[8] & 1) && i < p->nkeys - 1)
                    d3_UnwrapAngles(&p->keys[i + 1].rot, &p->keys[i].rot, &p->keys[i + 1].rot, &p->keys[i].rot);
                p->keys[i].posin.x = 0;
                p->keys[i].posin.y = 0;
                p->keys[i].posin.z = 0;
                p->keys[i].posout.x = 0;
                p->keys[i].posout.y = 0;
                p->keys[i].posout.z = 0;
                p->keys[i].rotin.x = 0;
                p->keys[i].rotin.y = 0;
                p->keys[i].rotin.z = 0;
                p->keys[i].rotout.x = 0;
                p->keys[i].rotout.y = 0;
                p->keys[i].rotout.z = 0;
            }
        } else {
            for (i = 0; i < p->nkeys; i++) {
                if ((p->flags.b[8] & 1) && i < p->nkeys - 1)
                    d3_UnwrapAngles(&p->keys[i + 1].rot, &p->keys[i].rot, &p->keys[i + 1].rot, &p->keys[i].rot);
                if (i == 0) {
                    p->keys[0].posin.x = 0;
                    p->keys[0].posin.y = 0;
                    p->keys[0].posin.z = 0;
                    p->keys[0].rotin.x = 0;
                    p->keys[0].rotin.y = 0;
                    p->keys[0].rotin.z = 0;
                } else if (i < p->nkeys - 1) {
                    if (i == p->nkeys - 2 && ((p->flags.b[1] & 1) || (p->flags.b[1] & 0x10))) {
                        p->keys[i].posout.x = 0;
                        p->keys[i].posout.y = 0;
                        p->keys[i].posout.z = 0;
                        p->keys[i].rotout.x = 0;
                        p->keys[i].rotout.y = 0;
                        p->keys[i].rotout.z = 0;
                    }
                    d3path_Tangents(&p->keys[i - 1], &p->keys[i], &p->keys[i + 1]);
                } else if (!(p->flags.b[1] & 1) && !(p->flags.b[1] & 0x10))
                    d3path_Tangents(&p->keys[i - 1], &p->keys[i], &p->keys[0]);
                else {
                    p->keys[i].posin.x = 0;
                    p->keys[i].posin.y = 0;
                    p->keys[i].posin.z = 0;
                    p->keys[i].posout.x = 0;
                    p->keys[i].posout.y = 0;
                    p->keys[i].posout.z = 0;
                    p->keys[i].rotin.x = 0;
                    p->keys[i].rotin.y = 0;
                    p->keys[i].rotin.z = 0;
                    p->keys[i].rotout.x = 0;
                    p->keys[i].rotout.y = 0;
                    p->keys[i].rotout.z = 0;
                }
            }
        }
    }
}
/* 0x433d70, gives the object a path animation through nkeys keys (copied; times in frames). */

D3ANIM *d3anim_CreatePath(D3OBJECT *obj, D3ANIMFLAGS f, int nkeys, D3ANIMKEY *keys)
{
    D3PATH *p = 0;
    int i;

    if (obj->path)
        d3anim_FreePath(obj);
    if (obj->type == 4 || obj->type == 3 || obj->type0 == 4) {
        p = mem_Alloc(sizeof(D3PATH), "d3_int:CreateObjAnim");
        p->nkeys = nkeys;
        p->flags = f;
        p->maxStep = 100;
        p->fadeEnd = 1.0f;
        p->frameTime = 0;
        p->flags.b[1] &= ~0x20;
        p->flags.b[8] = p->flags.b[8] & ~0x20 | 0x10;
        p->keys = mem_Alloc(nkeys * sizeof(D3PATHKEY), "d3_int:CrObjAnimFrameData");
        D3PATH_COPY(p->keys, keys, p->nkeys * sizeof(D3PATHKEY));
        p->keys[p->nkeys - 1].time = p->keys[p->nkeys - 2].time;
        for (i = 0; i < p->nkeys; i++)
            p->keys[i].time *= 3;
        p->pos.x = obj->pos.x;
        p->pos.y = obj->pos.y;
        p->pos.z = obj->pos.z;
        p->rot.x = obj->rotation.x;
        p->rot.y = obj->rotation.y;
        p->rot.z = obj->rotation.z;
        if (!(p->flags.b[1] & 2)) {
            p->key = 0;
            p->time = 0;
            p->next = 1;
        } else {
            p->key = p->nkeys - 1;
            p->time = p->keys[p->key].time - 1;
            p->next = d3anim_NextFrame(p->flags, p->key, 0, p->key, p->key);
        }
        d3path_Init(p);
    }
    obj->path = p;
    ((D3ANIMOBJ *)obj)->time = d3_Time;
    return (D3ANIM *)p;
}
/* 0x433f40, the cached animation with this name, 0 if not loaded. */

int d3_FindAnim(char *name)
{
    D3ANIMDATA *a;

    for (a = d3anim_Head.next; a != &d3anim_Tail; a = a->next)
        if (!_strcmpi(name, a->name))
            return (int)a;
    return 0;
}
/* 0x433f80, creates a two-frame morph animation from frame (of mesh) to dframe, or to the shape of dmesh;
   bone transforms are matched by name. For a target mesh the packed (byte) positions are rebuilt: the per-vertex
   differences go to a temporary array, their bounding box (mn / mx, D3VECTOR4 as in the original's frame) gives
   base and scale, and each difference is quantised as (v * 255) / extent.
   @stuck orchestrator + research pass 2026-11-14, 16 ins left in two hunks, both read from the compiler.
   (A) reload order of the register parameters mesh (edi) / dmesh (ebx) at three block edges behind the
   D3ANIM_COPY3 asm blocks: the original has dmesh first at all three (0x434127, 0x43421c, 0x434319), ours
   mesh first at the two mem_Alloc edges and dmesh first at the third. Cause: the edge loads are emitted in
   ascending candidate id of the split pieces, ids are recycled from a LIFO free list, so two split
   generations come out in opposite orders; the initial ids depend on (unit symbol ordinal & 0x3f) of the
   function's seven string literals and d3anim_Tail. Sweep of all 64 x 64 states plus 1500 random ones
   through a proxy hook: every state gives mesh-mesh-dmesh or dmesh-dmesh-mesh, never the original's - so
   no declaration set fixes it for this source; the original's source differs in candidate structure (two
   candidates fewer in the first split, or another set in the second). Nine block-scope externs (probe
   only) give the two mem_Alloc sites right and the third wrong. (B) six stores a->frames[1].bone[i].x:
   ours [eax+ecx+0x114], the original [ecx+eax+0x114]; base / index are the first / second operand of the
   IL add, fixed before strength reduction; not hash state (0 flips in 2500 states); flips with unsigned i
   (all compares wrong) or when the dmesh loop and its twin use different index variables (one instruction
   more: the shared i = 0 splits). No code-neutral lever in 2916 index-variable assignments, 15 spellings
   of the bone address, 31 block-scope placements, struct assignment. Tools: scratch morphanim\\c2x.py
   (candidate ids, free list, edge loads). (tiebreak: sensitive) prior: 202 -> 16 ins (draft -> shape) this
   session: mn / mx / d as D3VECTOR4 (frame 0x34 -> 0x40, the original's 16-byte slots), the quantise loop
   as plain tmp[i] index loop with a block vector v and (v.x * 255.0f) / d.x (VC6 hoists the invariant
   division as a reciprocal; named sx locals spill, * (1.0f / d.x) reassociates), the bounding loop as
   tmp[i] with uniform min / max macros, no nverts guard. Left (tie-break sensitive per dcp headers sample:
   2 codegens in 32 samples, none better): (1) order: after the mem_Alloc calls of lines 1059 / 1070
   (behind the D3ANIM_COPY3 inline asm that clobbers edi / ebx) the reloads come mesh (edi) then dmesh
   (ebx), the original dmesh first; (2) sib: the dmesh bone-copy loop stores a->frames[1].bone[i] as
   [eax+ecx+0x114] where the original has [ecx+eax+0x114] (the dst branch's twin loop matches with
   [eax+ecx]). flip.py --want '[ecx+eax+276]': 6 hits, only m7 (in... */

D3ANIMDATA *d3anim_CreateMorphAnim(D3ANIMMESH *mesh, D3ANIMDATA *src, D3ANIMFRAME *frame, D3ANIMDATA *dst,
                                   D3ANIMFRAME *dframe, D3ANIMMESH *dmesh, char *name, int n)
{
    D3ANIMDATA *a = 0;
    int i, k, count;
    D3VECTOR4 *tmp;
    D3VECTOR4 mn, mx, d, v;
    float z;

    if (frame && mesh && (dmesh || (dst && dframe)) && n > 1) {
        a = mem_Alloc(sizeof(D3ANIMDATA), "d3_anim:CreateMorphAnim");
        a->nverts = mesh->nverts;
        a->nframes = 2;
        strcpy(a->name, name);
        a->frames = mem_Alloc(a->nframes * sizeof(D3ANIMFRAME), "d3_anim:CreateMorphAnimFrame");
        a->frames[0].time = 0;
        a->frames[0].count = count = n * 3;
        a->frames[0].move.x = frame->move.x;
        a->frames[0].move.y = frame->move.y;
        a->frames[0].move.z = frame->move.z;
        a->frames[0].turn.x = frame->turn.x;
        a->frames[0].turn.y = frame->turn.y;
        a->frames[0].turn.z = frame->turn.z;
        if (frame->pos) {
            a->frames[0].pos = mem_Alloc(a->nverts * sizeof(D3VECTOR), "d3_anim:CreateMorphAnimPoints0");
            D3ANIM_COPY3(a->frames[0].pos, frame->pos, a->nverts * sizeof(D3VECTOR));
            a->frames[1].pos = mem_Alloc(a->nverts * sizeof(D3VECTOR), "d3_anim:CreateMorphAnimPoints1");
        }
        if (frame->packed) {
            a->frames[0].packed = mem_Alloc(a->nverts * 3, "d3_anim:CreateMorphAnimWPoints0");
            D3ANIM_COPY3(a->frames[0].packed, frame->packed, a->nverts * 3);
            a->frames[0].base.x = frame->base.x;
            a->frames[0].base.y = frame->base.y;
            a->frames[0].base.z = frame->base.z;
            a->frames[0].scale.x = frame->scale.x;
            a->frames[0].scale.y = frame->scale.y;
            a->frames[0].scale.z = frame->scale.z;
            a->frames[1].packed = mem_Alloc(a->nverts * 3, "d3_anim:CreateMorphAnimWPoints1");
        }
        a->frames[1].time = n;
        a->frames[1].count = count;
        a->nbones = src->nbones;
        for (i = 0; i < a->nbones; i++) {
            strcpy(D3ANIM_BONENAME(a, i), D3ANIM_BONENAME(src, i));
            a->frames[0].bone[i].pos.x = frame->bone[i].pos.x;
            a->frames[0].bone[i].pos.y = frame->bone[i].pos.y;
            a->frames[0].bone[i].pos.z = frame->bone[i].pos.z;
            a->frames[0].bone[i].rot.x = frame->bone[i].rot.x;
            a->frames[0].bone[i].rot.y = frame->bone[i].rot.y;
            a->frames[0].bone[i].rot.z = frame->bone[i].rot.z;
        }
        if (dmesh) {
            for (i = 0; i < a->nbones; i++) {
                for (k = 0; k < 4; k++) {
                    if (!strcmp(D3ANIM_BONENAME(a, i), dmesh->bones[k].name)) {
                        a->frames[1].bone[i].pos.x = dmesh->bones[k].pos.x;
                        a->frames[1].bone[i].pos.y = dmesh->bones[k].pos.y;
                        a->frames[1].bone[i].pos.z = dmesh->bones[k].pos.z;
                        a->frames[1].bone[i].rot.x = dmesh->bones[k].rot.x;
                        a->frames[1].bone[i].rot.y = dmesh->bones[k].rot.y;
                        a->frames[1].bone[i].rot.z = dmesh->bones[k].rot.z;
                    }
                }
            }
            a->frames[1].move.x = ((D3ANIMMESHX *)dmesh)->pos.x - ((D3ANIMMESHX *)mesh)->pos.x;
            a->frames[1].move.y = ((D3ANIMMESHX *)dmesh)->pos.y - ((D3ANIMMESHX *)mesh)->pos.y;
            a->frames[1].move.z = ((D3ANIMMESHX *)dmesh)->pos.z - ((D3ANIMMESHX *)mesh)->pos.z;
            a->frames[1].move.y = 0.0f;
            a->frames[1].turn.x = ((D3ANIMMESHX *)dmesh)->rot.x - ((D3ANIMMESHX *)mesh)->rot.x;
            a->frames[1].turn.y = ((D3ANIMMESHX *)dmesh)->rot.y - ((D3ANIMMESHX *)mesh)->rot.y;
            a->frames[1].turn.z = ((D3ANIMMESHX *)dmesh)->rot.z - ((D3ANIMMESHX *)mesh)->rot.z;
            if (frame->pos) {
                for (i = 0; i < a->nverts; i++) {
                    a->frames[1].pos[i].x = dmesh->verts[i].pos.x - mesh->verts[i].pos.x;
                    a->frames[1].pos[i].y = dmesh->verts[i].pos.y - mesh->verts[i].pos.y;
                    a->frames[1].pos[i].z = dmesh->verts[i].pos.z - mesh->verts[i].pos.z;
                }
            }
            if (frame->packed) {
                tmp = mem_Alloc(a->nverts * sizeof(D3VECTOR4), "d3_anim:CreateMorph(TempWPoints)");
                mx.z = -1e10f;
                mn.x = 1e10f;
                mn.y = 1e10f;
                mn.z = 1e10f;
                mx.x = -1e10f;
                mx.y = -1e10f;
                for (i = 0; i < a->nverts; i++) {
                    tmp[i].x = dmesh->verts[i].pos.x - mesh->verts[i].pos.x;
                    tmp[i].y = dmesh->verts[i].pos.y - mesh->verts[i].pos.y;
                    tmp[i].z = (z = dmesh->verts[i].pos.z - mesh->verts[i].pos.z);
                    mn.x = min(mn.x, tmp[i].x);
                    mn.y = min(mn.y, tmp[i].y);
                    mn.z = min(mn.z, z);
                    mx.x = max(mx.x, tmp[i].x);
                    mx.y = max(mx.y, tmp[i].y);
                    mx.z = max(mx.z, z);
                }
                d.x = mx.x - mn.x;
                a->frames[1].base.x = mn.x;
                d.y = mx.y - mn.y;
                a->frames[1].base.y = mn.y;
                a->frames[1].base.z = mn.z;
                d.z = mx.z - mn.z;
                a->frames[1].scale.x = d.x * (1.0f / 255.0f);
                a->frames[1].scale.y = d.y * (1.0f / 255.0f);
                a->frames[1].scale.z = d.z * (1.0f / 255.0f);
                for (i = 0; i < a->nverts; i++) {
                    v.x = tmp[i].x - mn.x;
                    v.y = tmp[i].y - mn.y;
                    v.z = tmp[i].z - mn.z;
                    a->frames[1].packed[i * 3] = (unsigned char)((v.x * 255.0f) / d.x);
                    a->frames[1].packed[i * 3 + 1] = (unsigned char)((v.y * 255.0f) / d.y);
                    a->frames[1].packed[i * 3 + 2] = (unsigned char)((v.z * 255.0f) / d.z);
                }
                mem_Free(tmp);
            }
        } else {
            for (i = 0; i < a->nbones; i++) {
                for (k = 0; k < 4; k++) {
                    if (!strcmp(D3ANIM_BONENAME(a, i), D3ANIM_BONENAME(dst, k))) {
                        a->frames[1].bone[i].pos.x = dframe->bone[k].pos.x;
                        a->frames[1].bone[i].pos.y = dframe->bone[k].pos.y;
                        a->frames[1].bone[i].pos.z = dframe->bone[k].pos.z;
                        a->frames[1].bone[i].rot.x = dframe->bone[k].rot.x;
                        a->frames[1].bone[i].rot.y = dframe->bone[k].rot.y;
                        a->frames[1].bone[i].rot.z = dframe->bone[k].rot.z;
                    }
                }
            }
            a->frames[1].move.x = dframe->move.x;
            a->frames[1].move.y = dframe->move.y;
            a->frames[1].move.z = dframe->move.z;
            a->frames[1].turn.x = dframe->turn.x;
            a->frames[1].turn.y = dframe->turn.y;
            a->frames[1].turn.z = dframe->turn.z;
            if (frame->pos)
                D3ANIM_COPY3(a->frames[1].pos, dframe->pos, a->nverts * sizeof(D3VECTOR));
            if (frame->packed) {
                D3ANIM_COPY3(a->frames[1].packed, dframe->packed, a->nverts * 3);
                a->frames[1].base.x = dframe->base.x;
                a->frames[1].base.y = dframe->base.y;
                a->frames[1].base.z = dframe->base.z;
                a->frames[1].scale.x = dframe->scale.x;
                a->frames[1].scale.y = dframe->scale.y;
                a->frames[1].scale.z = dframe->scale.z;
            }
        }
        a->prev = d3anim_Tail.prev;
        d3anim_Tail.prev = a;
        a->next = &d3anim_Tail;
        a->prev->next = a;
    }
    return a;
}
/* 0x434a80, bytes used by a cached animation, -1 if not loaded. */

int d3anim_GetSize(D3ANIMDATA *a)
{
    int size = 0;

    if (a && a->frames) {
        if (a->frames->pos)
            size = a->nverts * 12;
        if (a->frames->packed)
            size += a->nverts * 3;
        if (a->frames->normals)
            size += a->nverts * 3;
        return a->nframes * (size + sizeof(D3ANIMFRAME)) + sizeof(D3ANIMDATA);
    }
    return -1;
}
/* 0x434af0, frees the least recently used unreferenced animations until size <= limit. */

char d3anim_Purge(int limit, int size)
{
    D3ANIMDATA *a, *oldest;
    unsigned int t;

    if (limit < 0)
        return 0;
    while (size > limit) {
        oldest = 0;
        t = d3_FrameCount;
        for (a = d3anim_Head.next; a != &d3anim_Tail; a = a->next) {
            if (a->refs == 0 && a->lastuse <= t) {
                t = a->lastuse;
                oldest = a;
            }
        }
        if (!oldest)
            return 0;
        size -= d3anim_GetSize(oldest);
        d3anim_Release(oldest, 1);
    }
    return 1;
}
/* 0x434b70, drops a reference; unreferenced animations stay cached while they fit. */

void d3anim_Release(D3ANIMDATA *a, char now)
{
    D3ANIMDATA *p;
    int i, used, size;

    if (--a->refs > 0)
        return;
    if (now) {
        a->next->prev = a->prev;
        a->prev->next = a->next;
        for (i = 0; i < a->nframes; i++) {
            if (a->frames[i].pos) {
                mem_Free(a->frames[i].pos);
                a->frames[i].pos = 0;
            }
            if (a->frames[i].packed) {
                mem_Free(a->frames[i].packed);
                a->frames[i].packed = 0;
            }
            if (a->frames[i].normals) {
                mem_Free(a->frames[i].normals);
                a->frames[i].normals = 0;
            }
        }
        mem_Free(a->frames);
        a->frames = 0;
        mem_Free(a);
        return;
    }
    a->refs = 0;
    used = 0;
    for (p = d3anim_Head.next; p != &d3anim_Tail; p = p->next) {
        if (p->refs == 0)
            used += d3anim_GetSize(p);
    }
    size = d3anim_GetSize(a);
    if (size + used <= d3anim_CacheMax)
        return;
    if (d3anim_Purge(d3anim_CacheMax - size, used))
        return;
    d3anim_Release(a, 1);
}
/* 0x434d00, the largest vector length. */

float d3anim_MaxRadius(D3VECTOR4 *v, int n)
{
    float r = -1.0f;
    float d;

    int i;
    for (i = 0; i < n; i++) {
        d = v[i].x * v[i].x + v[i].y * v[i].y + v[i].z * v[i].z;
        r = max(r, d);
    }
    return (float)sqrt(r);
}

typedef struct D3ANIMBOX {
    D3VECTOR4 min, max;
} D3ANIMBOX;

/* 0x434d60, per key frame: vertex normals (3 bytes, 0..255), radius and bounding box; the boxes are
   then widened to cover the neighbouring frames.
   @deep B draft: 34 code hunks, all of one form - [eax+esi+0x3c] vs [esi+eax+0x3c], the SIB base/index roles of the same frame-array address computation; every operation and constant is identical on both sides, so this is a VC6 addressing-mode choice (the d3path_Init note records the same pattern resisting (p->keys + i)-> / i[p->keys]), not a meaning error. Readable otherwise. */
void d3anim_CalculateAnimNormals(D3ANIMDATA *a, D3ANIMMESH *mesh)
{
    D3VECTOR4 *points;
    D3VECTOR4 *normals;
    D3ANIMBOX *bbox;
    D3ANIMFRAME *fr;
    D3VECTOR4 n;
    D3VECTOR4 mn, mx;
    int f, i, k;

    points = mem_Alloc(mesh->nverts << 4, "d3_anim:CalculateAnimNormals(points)");
    normals = mem_Alloc(mesh->nfaces << 4, "d3_anim:CalculateAnimNormals(normals)");
    for (f = 0; f < a->nframes; f++) {
        fr = &a->frames[f];
        fr->normals = mem_Alloc(mesh->nverts * 3, "d3_anim:CalculateAnimNormals");
        if (fr->pos) {
            for (i = 0; i < a->nverts; i++) {
                points[i].x = mesh->verts[i].pos.x + fr->pos[i].x;
                points[i].y = mesh->verts[i].pos.y + fr->pos[i].y;
                points[i].z = mesh->verts[i].pos.z + fr->pos[i].z;
            }
        } else {
            for (i = 0; i < a->nverts; i++) {
                n.x = (float)fr->packed[i * 3] * fr->scale.x + fr->base.x;
                n.y = (float)fr->packed[i * 3 + 1] * fr->scale.y + fr->base.y;
                n.z = (float)fr->packed[i * 3 + 2] * fr->scale.z + fr->base.z;
                points[i].x = n.x + mesh->verts[i].pos.x;
                points[i].y = n.y + mesh->verts[i].pos.y;
                points[i].z = n.z + mesh->verts[i].pos.z;
            }
        }
        fr->radius = mesh->radius - d3anim_MaxRadius(points, mesh->nverts);
        for (i = 0; i < mesh->nfaces; i++)
            d3_TriNormal(&points[mesh->faces[i].v[0]], &points[mesh->faces[i].v[1]],
                         &points[mesh->faces[i].v[2]], &normals[i]);
        mn.z = mn.y = mn.x = 1e10f;
        mx.z = mx.y = mx.x = -1e10f;
        for (i = 0; i < a->nverts; i++) {
            mn.x = min(mn.x, points[i].x);
            mn.y = min(mn.y, points[i].y);
            mn.z = min(mn.z, points[i].z);
            mx.x = max(mx.x, points[i].x);
            mx.y = max(mx.y, points[i].y);
            mx.z = max(mx.z, points[i].z);
            n.z = n.y = n.x = 0.0f;
            for (k = 0; k < mesh->nfaces; k++) {
                if (mesh->faces[k].v[0] == i || mesh->faces[k].v[1] == i || mesh->faces[k].v[2] == i) {
                    n.x += normals[k].x;
                    n.y += normals[k].y;
                    n.z += normals[k].z;
                }
            }
            d3_Normalize(&n);
            n.x += 1.0f;
            n.y += 1.0f;
            n.z += 1.0f;
            fr->normals[i * 3] = (unsigned char)(n.x * 127.5);
            fr->normals[i * 3 + 1] = (unsigned char)(n.y * 127.5);
            fr->normals[i * 3 + 2] = (unsigned char)(n.z * 127.5);
        }
        fr->min.x = mn.x;
        fr->min.y = mn.y;
        fr->min.z = mn.z;
        fr->max.x = mx.x;
        fr->max.y = mx.y;
        fr->max.z = mx.z;
    }
    mem_Free(normals);
    mem_Free(points);
    bbox = mem_Alloc(a->nframes << 5, "d3_anim:CalculateAnimNormals(bbox_points)");
    for (f = 0; f < a->nframes; f++) {
        bbox[f].min.x = a->frames[f].min.x;
        bbox[f].min.y = a->frames[f].min.y;
        bbox[f].min.z = a->frames[f].min.z;
        bbox[f].max.x = a->frames[f].max.x;
        bbox[f].max.y = a->frames[f].max.y;
        bbox[f].max.z = a->frames[f].max.z;
    }
    for (f = 0; f < a->nframes; f++) {
        if (f > 0) {
            bbox[f].min.x = min(bbox[f].min.x, a->frames[f - 1].min.x);
            bbox[f].min.y = min(bbox[f].min.y, a->frames[f - 1].min.y);
            bbox[f].min.z = min(bbox[f].min.z, a->frames[f - 1].min.z);
            bbox[f].max.x = max(bbox[f].max.x, a->frames[f - 1].max.x);
            bbox[f].max.y = max(bbox[f].max.y, a->frames[f - 1].max.y);
            bbox[f].max.z = max(bbox[f].max.z, a->frames[f - 1].max.z);
        }
        if (f < a->nframes - 1) {
            bbox[f].min.x = min(bbox[f].min.x, a->frames[f + 1].min.x);
            bbox[f].min.y = min(bbox[f].min.y, a->frames[f + 1].min.y);
            bbox[f].min.z = min(bbox[f].min.z, a->frames[f + 1].min.z);
            bbox[f].max.x = max(bbox[f].max.x, a->frames[f + 1].max.x);
            bbox[f].max.y = max(bbox[f].max.y, a->frames[f + 1].max.y);
            bbox[f].max.z = max(bbox[f].max.z, a->frames[f + 1].max.z);
        }
    }
    for (f = 0; f < a->nframes; f++) {
        a->frames[f].min.x = bbox[f].min.x;
        a->frames[f].min.y = bbox[f].min.y;
        a->frames[f].min.z = bbox[f].min.z;
        a->frames[f].max.x = bbox[f].max.x;
        a->frames[f].max.y = bbox[f].max.y;
        a->frames[f].max.z = bbox[f].max.z;
    }
    mem_Free(bbox);
}
/* 0x435550, starts the cached animation name in a free slot of the animation control: marks the control
   active, takes a reference on the data (calculating its normals once), sets up frame/nextFrame, speed,
   timeout and the attach fields of the slot and poses the bones. Returns the slot, 0 when there is no free
   slot, the data is missing or the vertex counts differ. */

int d3anim_StartAnim(void *list, char *name, D3ANIMFLAGS f)
{
    D3ANIMCTL *ctl = list;
    D3ANIMDATA *data;
    D3ANIMSLOT *s;
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        data = (D3ANIMDATA *)d3_FindAnim(name);
        for (i = 0; i < 3; i++)
            if (!ctl->slots[i].data)
                break;
        if (data && data->nverts == ctl->nverts && i < 3) {
            d3_Busy++;
            s = &ctl->slots[i];
            obj = ctl->obj;
            obj->time = d3_Time;
            ctl->active = 1;
            s->data = data;
            s->speed = 1.0f;
            s->frameTime = 0;
            s->timeout = 100;
            data->refs++;
            data->lastuse = d3_FrameCount;
            if (!data->normals && ctl->mesh) {
                d3anim_CalculateAnimNormals(data, ctl->mesh);
                data->normals = 1;
            }
            s->flags = f;
            s->flags.b[1] &= ~0x20;
            s->flags.b[8] = s->flags.b[8] & ~0x48 | 0x10;
            if (f.b[0] == 1)
                s->flags.b[1] |= 0x10;
            s->attachFirst = s->attachLast = s->attachBone = -1;
            s->fadeStart = s->fadeEnd = s->attached = 0;
            s->weight = s->fadeFrom = s->fadeTo = 1.0f;
            s->lastSub = 0;
            if (data->relative)
                s->lastFrame = 0;
            else
                s->lastFrame = data->first;
            if (s->flags.b[1] & 2) {
                s->frame = data->nframes - 1;
                s->subFrame = data->frames[s->frame].count - 1;
                s->nextFrame = d3anim_NextFrame(s->flags, s->frame, data->first, data->last, data->nframes);
            } else {
                s->subFrame = 0;
                s->frame = 0;
                s->nextFrame = 1;
            }
            d3anim_MapBones((D3OBJECT *)obj);
            d3anim_PoseBones((D3OBJECT *)obj);
            d3_Busy--;
            return (int)s;
        }
    }
    return 0;
}
/* 0x435740, stops the animation name. */

void d3_ReleaseTexture(void *list, char *name)
{
    D3ANIMCTL *ctl = list;
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        obj = ctl->obj;
        for (i = 0; i < 3; i++) {
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name)) {
                ctl->slots[i].flags.b[8] &= ~2;
                d3anim_Release(ctl->slots[i].data, 0);
                ctl->slots[i].data = 0;
            }
        }
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data)
                break;
        if (i == 3)
            ctl->active = 0;
        d3anim_MapBones((D3OBJECT *)obj);
        d3anim_PoseBones((D3OBJECT *)obj);
    }
}
/* 0x435810, pauses the slot that plays the animation name: clears its play flag (the slot and its data
   stay loaded; ch_PauseAnim calls this with pause = 1). */

void d3anim_PauseAnim(D3ANIMCTL *ctl, char *name)
{
    int i;

    if (ctl) {
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name))
                ctl->slots[i].flags.b[8] &= ~2;
    }
}
/* 0x435870, starts or stops the slot of the animation name (its play flag = on) and marks the object's
   animation time as current. */

void d3anim_PlayAnim(D3ANIMCTL *ctl, char *name, char on)
{
    D3ANIMOBJ *obj;
    int i;

    if (ctl) {
        obj = ctl->obj;
        obj->time = d3_Time;
        for (i = 0; i < 3; i++)
            if (ctl->slots[i].data && !strcmp(name, ctl->slots[i].data->name))
                ctl->slots[i].play = on;
    }
}
/* 0x435900, the running slot with the largest weight. */

D3ANIMSLOT *d3anim_GetMainSlot(D3ANIMCTL *ctl)
{
    float best = 0.0f;
    D3ANIMSLOT *s = 0;
    int i;

    if (ctl->active) {
        for (i = 2; i >= 0; i--) {
            if (ctl->slots[i].data && best < ctl->slots[i].weight) {
                best = ctl->slots[i].weight;
                s = &ctl->slots[i];
            }
        }
    }
    return s;
}
/* 0x435950, sets the slot's current frame (clamped to the last frame) and the sub-frame 0 or the frame's
   end, computes the next frame and marks the object for an update. */

void d3anim_SetFrame(D3OBJECT *obj, D3ANIMSLOT *s, int frame)
{
    D3ANIMDATA *data;

    if (s && (data = s->data)) {
        if (frame >= data->last)
            frame = data->last - 1;
        if (frame < 0)
            frame = data->last - 1;
        if (frame + 1 >= data->last)
            s->subFrame = data->frames[frame].count - 1;
        else
            s->subFrame = 0;
        s->frame = frame;
        s->nextFrame = d3anim_NextFrame(s->flags, frame, data->first, data->last, data->nframes);
        d3_UpdateObject(obj, 1);
    }
}
