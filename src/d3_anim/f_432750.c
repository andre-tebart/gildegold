/* 0x432750, the per-object animation step (d3_ForEachObject callback, ctx = d3_Time, | 0x80000000 in the
   second pass): steps the three vertex animation slots of every active control of the mesh material, then
   the object's path animation (hermite through the keys). ROUGH draft from the disassembly.
   @deep B draft, 4141/4480 bytes, 2599 code hunks and a ROUGH marker: the per-object animation step is a laid-out draft (notes/units/d3_anim.md Open work: struct fields for the vertex slots, key types of the jump-free path loop still open) - a deep reconstruction, not a base meaning fix. Readability also needs Q1/Q2 (16 unnamed identifiers) and Q4 (D3_OBJPATH, OBJ_LAST, SLOT_F, SLOT_TURN). */

/* The material of an animated mesh (D3MATERIAL view): up to five animation controls of 0x198 bytes from 0xf4. */
typedef struct D3ANIMMATV {
    char pad000[0xf4];
    D3ANIMCTL ctl[5];               /* 0xf4..0x8ec */
    char pad8ec[0x80];
    unsigned char nctl;             /* 0x96c (the original reads the byte there, not after 0xe0 bytes of pad) */
    unsigned char flags;            /* 0x96d, 0x40 = no vertex animation */
} D3ANIMMATV;

#define OBJ_LAST(o) (*(unsigned int *)((char *)(o) + 0x44))
#define SLOT_F(s, o) (*(float *)((char *)(s) + (o)))
#define SLOT_TURN(s) (((*(int *)((char *)(s) + 0x70)) << 30) >> 30)
#define FRAME_AT(d, i) ((D3ANIMFRAME *)((char *)(d)->frames + (i) * 0xc0))

float d3_Hermite(float p0, float p1, float m0, float m1, float t);                      /* 0x453e20 */
void d3_DirToWorld(struct XOBJ *obj, float *in, float *out);                                /* 0x458810 */
void tex_Animate(D3OBJECT *obj, unsigned int now);                                     /* 0x460b90 */
void d3_RelightObjectFrame(D3OBJECT *obj, D3ANIMFRAME *f);                                       /* 0x4535b0, d3_light.h's canonical prototype; d3_anim.c keeps this local copy, the include is WORSE for d3anim_CreatePath / d3anim_CreateMorphAnim (notes/units/d3_light.md) */
struct D3ANIMSLOT * d3anim_GetMainSlot(struct D3ANIMCTL *ctl);                                         /* 0x435900 */

char d3anim_StepObject(D3OBJECT *obj, void *ctx)
{
    unsigned int now = (unsigned int)ctx & 0x7fffffff;
    char second = (unsigned int)ctx >> 31;
    int dt = now - ((D3ANIMOBJ *)obj)->time;
    D3ANIMMATV *mat;
    D3ANIMCTL *c;
    D3ANIMSLOT *s;
    D3ANIMDATA *data;
    D3ANIMFRAME *shown = 0;
    D3PATH *P;
    D3PATHKEY *keys, *k0, *k1;
    D3VECTOR v, w, out, tin, tout;
    int i, j, n, last, limit, nframes, nk, half;
    char done, over = 0, over2 = 0, released;
    float acc, t, u;
    unsigned char f1;

    ((D3ANIMOBJ *)obj)->time = now;
    if (dt == 0)
        return 1;
    mat = (D3ANIMMATV *)obj->material;
    if (mat) {
        if (!second && obj->info && !(mat->flags & 0x40) && obj->type == 4)
            tex_Animate(obj, now);
        for (i = 0; i < mat->nctl; i++) {
            c = &mat->ctl[i];
            if (!c->active)
                continue;
            for (j = 0; j < 3; j++) {
                s = &c->slots[j];
                data = s->data;
                if (!data || !(s->flags.b[8] & 2))
                    continue;
                if (now - OBJ_LAST(obj) > (unsigned int)s->timeout)
                    over = 1;
                last = data->first;
                nframes = data->nframes;
                if (s->flags.b[1] & 0x10) {
                    limit = nframes - 1;
                    if (!data->relative)
                        s->lastFrame = 0;
                } else
                    limit = data->last;
                s->flags.b[1] &= ~0x20;
                data->lastuse = d3anim_Clock;
                d3_UpdateObject(obj, 1);
                done = 0;
                if (s->fadeStart != s->fadeEnd) {
                    if ((unsigned int)s->fadeEnd > now)
                        s->weight = (float)(int)(now - s->fadeStart) / (s->fadeEnd - s->fadeStart) * (s->fadeTo - s->fadeFrom) + s->fadeFrom;
                    else {
                        s->fadeEnd = 0;
                        s->fadeStart = 0;
                        s->weight = s->fadeTo;
                        if (s->weight == 0.0)
                            done = 1;
                    }
                }
                acc = ((s->flags.b[8] >> 1) & 1) * s->speed * dt + s->frameTime;
                s->frameTime = acc;
                n = (int)acc;
                if (s->flags.b[1] & 2)
                    s->subFrame -= n;
                else
                    s->subFrame += n;
                s->frameTime = acc - n;
normalize:
                if (s->flags.b[1] & 2) {
                    n = s->subFrame;
                    if (n < 0) {
                        if (s->nextFrame > last) {
                            s->frame--;
                            s->nextFrame = d3anim_NextFrame(s->flags, s->frame, last, limit, nframes);
                            s->subFrame = FRAME_AT(data, s->nextFrame)->count + n;
                        } else {
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
                                d3anim_ApplyRootMotion(obj, s, s->lastFrame, limit, SLOT_TURN(s));
                        }
                        s->nextFrame = d3anim_NextFrame(s->flags, s->frame, last, limit, nframes);
                    }
                }
                if (s->flags.b[8] & 4) {
                    n = FRAME_AT(data, s->frame)->count;
                    half = n / 2;
                    if (s->subFrame >= 0 && s->subFrame < n)
                        half = s->subFrame;
                    d3anim_Unk432190(obj, s, s->frame, half, &out);
                    if (fabs(out.x - SLOT_F(s, 0x4c)) < SLOT_F(s, 0x5c) && fabs(out.z - SLOT_F(s, 0x54)) < SLOT_F(s, 0x5c)) {
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
                    d3anim_ApplyMotionTo(obj, s, s->subFrame, s->frame, SLOT_TURN(s));
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

                            if (!(s->flags.b[8] & 0x40)) {
                                s->flags.b[8] |= 0x40;
                                SLOT_F(s, 0x20) = att->pos.x;
                                SLOT_F(s, 0x24) = att->pos.y;
                                SLOT_F(s, 0x28) = att->pos.z;
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
                            out.x += SLOT_F(s, 0x20);
                            out.y += SLOT_F(s, 0x24);
                            out.z += SLOT_F(s, 0x28);
                            d3_SetObjectPos(att, &out);
                        }
                    }
                }
                released = 0;
                if (done) {
                    s->flags.b[8] &= ~2;
                    if (!(s->flags.b[1] & 8)) {
                        released = 1;
                        if (!data->relative)
                            d3anim_ApplyRootMotion(obj, s, s->lastFrame, limit, SLOT_TURN(s));
                        d3_ReleaseTexture(c, (char *)data);
                    } else
                        s->flags.b[1] |= 0x20;
                }
                if (over && !released)
                    shown = FRAME_AT(s->data, s->frame);
            }
        }
    }
    d3anim_PoseBones(obj);
    P = D3_OBJPATH(obj);
    if (P && (P->flags.b[8] & 2)) {
        nk = P->nkeys;
        keys = P->keys;
        P->flags.b[1] &= ~0x20;
        if (P->flags.b[1] & 2) {
            u = (float)P->time / keys[P->next].time;
            t = 1.0f - u;
        } else {
            t = (float)P->time / keys[P->key].time;
            u = 1.0f - t;
        }
        k0 = &keys[P->key];
        k1 = &keys[P->next];
        if (P->flags.b[1] & 4) {
            if (P->flags.b[1] & 2) {
                tin.x = -k1->posin.x; tin.y = -k1->posin.y; tin.z = -k1->posin.z;
                tout.x = -k1->posout.x; tout.y = -k1->posout.y; tout.z = -k1->posout.z;
                v.x = d3_Hermite(k0->pos.x, k1->pos.x, tout.x, tin.x, t) + P->pos.x;
                v.y = d3_Hermite(k0->pos.y, k1->pos.y, tout.y, tin.y, t) + P->pos.y;
                v.z = d3_Hermite(k0->pos.z, k1->pos.z, tout.z, tin.z, t) + P->pos.z;
                d3_SetObjectPos(obj, &v);
                if (obj->type < 5) {
                    tin.x = -k1->rotin.x; tin.y = -k1->rotin.y; tin.z = -k1->rotin.z;
                    tout.x = -k1->rotout.x; tout.y = -k1->rotout.y; tout.z = -k1->rotout.z;
                    w.x = d3_Hermite(k0->rot.x, k1->rot.x, tout.x, tin.x, t) + P->rot.x;
                    w.y = d3_Hermite(k0->rot.y, k1->rot.y, tout.y, tin.y, t) + P->rot.y;
                    w.z = d3_Hermite(k0->rot.z, k1->rot.z, tout.z, tin.z, t) + P->rot.z;
                    d3_SetObjectRot(obj, &w);
                }
            } else {
                v.x = d3_Hermite(k0->pos.x, k1->pos.x, k0->posin.x, k0->posout.x, t) + P->pos.x;
                v.y = d3_Hermite(k0->pos.y, k1->pos.y, k0->posin.y, k0->posout.y, t) + P->pos.y;
                v.z = d3_Hermite(k0->pos.z, k1->pos.z, k0->posin.z, k0->posout.z, t) + P->pos.z;
                d3_SetObjectPos(obj, &v);
                if (obj->type < 5) {
                    w.x = d3_Hermite(k0->rot.x, k1->rot.x, k0->rotin.x, k0->rotout.x, t) + P->rot.x;
                    w.y = d3_Hermite(k0->rot.y, k1->rot.y, k0->rotin.y, k0->rotout.y, t) + P->rot.y;
                    w.z = d3_Hermite(k0->rot.z, k1->rot.z, k0->rotin.z, k0->rotout.z, t) + P->rot.z;
                    d3_SetObjectRot(obj, &w);
                }
            }
        } else {
            v.x = t * k0->pos.x + u * k1->pos.x;
            v.y = t * k0->pos.y + u * k1->pos.y;
            v.z = t * k0->pos.z + u * k1->pos.z;
            if (!d3_VecEqual(&v, &v, 0.0f)) {
                v.x += P->pos.x;
                v.y += P->pos.y;
                v.z += P->pos.z;
                d3_SetObjectPos(obj, &v);
            }
            if (obj->type < 5) {
                w.x = u * k1->rot.x + t * k0->rot.x + P->rot.x;
                w.y = u * k1->rot.y + t * k0->rot.y + P->rot.y;
                w.z = u * k1->rot.z + t * k0->rot.z + P->rot.z;
                d3_SetObjectRot(obj, &w);
            }
        }
        f1 = P->flags.b[1];
        done = 0;
        if ((f1 & 4) && (f1 & 0x10) && !(f1 & 1) && !(P->flags.b[8] & 0x20) && P->next < P->key - 1) {
            P->flags.b[8] |= 0x20;
            keys[P->key - 1].posin.x = keys[P->key - 1].posin.y = keys[P->key - 1].posin.z = 0;
            keys[P->key - 2].posout.x = keys[P->key - 2].posout.y = keys[P->key - 2].posout.z = 0;
            keys[P->key - 1].rotin.x = keys[P->key - 1].rotin.y = keys[P->key - 1].rotin.z = 0;
            keys[P->key - 2].rotout.x = keys[P->key - 2].rotout.y = keys[P->key - 2].rotout.z = 0;
        }
        if (now - OBJ_LAST(obj) > (unsigned int)P->unk10)
            over2 = 1;
        acc = ((P->flags.b[8] >> 1) & 1) * P->fadeEnd * dt + *(float *)&P->timeout;
        *(float *)&P->timeout = acc;
        n = (int)acc;
        if (P->flags.b[1] & 2)
            P->time -= n;
        else
            P->time += n;
        *(float *)&P->timeout = acc - n;
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
                            if (f1 & 1)
                                P->time = -P->time;
                            else
                                P->time = keys[nk - 1].time + P->time;
                            P->flags.b[8] &= ~0x10;
                            P->flags.b[1] &= ~2;
                            P->key = 0;
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
                        P->time += keys[P->next].time;
                        P->key = P->key - 1;
                    }
                    P->next = d3anim_NextFrame(P->flags, P->key, 0, nk - 1, nk);
                    goto stepped;
                }
            } else if (P->time >= keys[P->key].time) {
                int kt = keys[P->key].time;

                if (P->key + 1 >= nk - 1 && (f1 & 1)) {
bounce:
                    P->flags.b[1] |= 2;
                    P->key = P->next;
                    P->time = kt - ((P->flags.b[8] >> 1) & 1) - 1;
                } else if (P->key < nk - 1 && !(P->key + 1 >= nk - 1 && (f1 & 0x10) && !(f1 & 1))) {
                    if ((P->flags.b[8] & 0x10) && !(f1 & 0x11) && (f1 & 4)) {
                        d3path_Tangents(&keys[P->key - 1], keys, &keys[1]);
                        P->flags.b[8] &= ~0x10;
                    }
                    P->time -= keys[P->key].time;
                    P->key++;
                } else if (f1 & 0x10) {
                    if (f1 & 1)
                        goto bounce;
                    done = 1;
                    P->key = nk - 1;
                    P->time = keys[P->key].time - 1;
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
                P->next = d3anim_NextFrame(P->flags, P->key, 0, nk - 1, nk);
stepped:
                if (done) {
                    v.x = keys[P->next].pos.x + P->pos.x;
                    v.y = keys[P->next].pos.y + P->pos.y;
                    v.z = keys[P->next].pos.z + P->pos.z;
                    d3_SetObjectPos(obj, &v);
                    w.x = keys[P->next].rot.x + P->rot.x;
                    w.y = keys[P->next].rot.y + P->rot.y;
                    w.z = keys[P->next].rot.z + P->rot.z;
                    d3_SetObjectRot(obj, &w);
                    if (!(P->flags.b[1] & 8))
                        d3anim_FreePath(obj);
                    else {
                        P->flags.b[8] &= ~2;
                        P->flags.b[1] |= 0x20;
                    }
                    break;
                }
            }
            if (P->time >= 0 && !(!(P->flags.b[1] & 2) && P->time >= keys[P->key].time)
                && !((P->flags.b[1] & 2) && P->time >= keys[P->next].time))
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
