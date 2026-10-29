/* 0x409bd0, action callback 0x3a: walks along the cells of a->u.walk.path (0x100 cells, filled by the caller;
   empty cells are skipped) to the cell args[0], args[1] in scene args[2]
   @deep B3 far draft, 2130/3376 bytes, 138 differing instructions of 971 (ours 969, two fewer - the same redundant ch->slot test as ch_ActWalk: the source writes '!ch->slot || CH_XSLOT(ch)->f6d_20 || !ch->slot' and VC6 propagates the slot value, see ch_ActWalk's @deep for the exhaustive spelling list). All other code hunks were read against the original (the function is the inline copy of ch_InitWalk/ch_StartWalkAnim/ch_WalkSound/ch_NextWaypoint/ch_WalkRotate; the noted sign bug of the original's right turn - the 2.0943952f branch subtracting, step*0.32f - is already reproduced) and are allocation: 104 slots hunks, the frame 4 bytes larger (n in a stack slot) and the cell-skip loop bound min(0x100, n+32) hoisted (the 'n = cur + 1; for (i = n; ...)' form reproduces the original loop but moves ground into edi and spills the constant 0 - recorded in notes/units/ch.md, tried and reverted). Readability: no gap left beyond the unit-wide ones recorded in the note (CH_UNK080, D3OBJ_UNK200, ch_Unk402540, ch_Unk5f1290, ch_Unk631058/60, u.walk.unk0fc, CHARACTER.unk1a0). */

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
        CH_UNK080(ch) = 0;
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
        a->u.walk.unk0fc = -1;
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
        CH_UNK080(ch) = 0;
        if (ch->morph) {
            sprintf(name, "morph_%i", ch);
            d3_ReleaseTexture(ch->obj->material->animCtl, name);
            ad = (D3ANIMDATA *)d3_FindAnim(name);
            d3anim_Release(ad, 1);
            ch->morph = 0;
        }
        d3t_ReleaseObjectTextures(ch->obj);
        if (!ch->obj->info)
            ch_Unk402540(ch);
        if (ch->transport)
            s = (CHSLOT *)ch_AttachAni(ch, "bewegung/karren_ziehen", 1);
        else
            s = (CHSLOT *)ch_AttachAni(ch, "bewegung/gehen", 1);
        if (!(ch->slot = (D3ANIMSLOT *)s)) {
            if (ch_Unk631058)
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
        if (ch_Unk631058)
            a->ch->flags.all |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return;
    }
    if (!a->u.walk.started && !ch->slot) {
        if (ch_Unk631058)
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
            if ((ch->kind & 1) && ch_Unk631060 < 3 && d3t_SceneIndex(ch->scene)) {
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
                    snd = sb_GetSample(ch_SndLib, "normal_s");
                if (snd) {
                    d3snd_PlayAt(snd, ch->obj, 40, 500.0f);
                    ch_Unk631060++;
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
            if (ch_Unk631058)
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
                r = rot - step * 0.2f;
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
                r = rot + step * 0.2f;
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
        if (ch->group == -1 && D3OBJ_UNK200(ch->obj)) {
            f = ch_Unk5f1290;
            if (!ch->transport)
                f += 0.2f;
        }
        if (ch->kind & 8) {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = (f * ch->unk1a0) * ch->speedScale * 1.9f;
            else
                CH_XSLOT(ch)->playSpeed = (f * ch->unk1a0) * ch->speedScale * 1.7f;
        } else {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = (f * ch->unk1a0) * ch->speedScale * 2.5f;
            else
                CH_XSLOT(ch)->playSpeed = (f * ch->unk1a0) * ch->speedScale * 2.2f;
        }
    }
}
