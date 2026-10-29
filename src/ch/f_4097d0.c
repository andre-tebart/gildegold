/* 0x4097d0, action callback 0x2d: walks along the path to the cell args[0], args[1] in scene args[2] @deep B3 draft, 440/1024 bytes, 8 differing instructions of 323: exactly one redundant test is missing - the original tests ch->slot a third time after the f6d_20 bit test (4099c4 mov eax,[edi+0x70]; cmp eax,ebx; je start; test [eax+0x6d],0x20; jne start; cmp eax,ebx; jne skip), ours drops that last cmp/jne; the other hunk is the 409bcc alignment fill. The source condition is 'if (!a->u.walk.started && (!ch->slot || CH_XSLOT(ch)->f6d_20 || !ch->slot))' and VC6 propagates the value in eax across the three terms. spell: 4099cf has no menu rule (meaning check: the third term is the same load), 409bcc dup-tail is the fill. Round-7 attempts are in notes/units/ch.md (== 0 / == NULL / CH_XSLOT spellings, a->ch for one term -> reloads, ternary, !(slot && !f6d_20 && slot), nested ifs, else-if with the call duplicated, goto, call folded into the condition, a static __inline helper, f6d_20 == 1/!= 0, a raw byte test); this session only re-read the original flow (the same shape is kept correctly in the following block, where the original re-tests started at 4099fc and our build reproduces it). Q2 gaps: ch_Unk5f1290 (0x5f1290 = 1.0f, read only here as the speed factor of a group -1 object with D3OBJ_UNK200 set; no other reader or data alias), ch_Unk631058 (0x631058, read-only in ch.c, never written anywhere), CHARACTER.unk1a0 (a speed factor multiplied into CH_XSLOT.playSpeed, only reads in any source). Q4 gap: D3OBJ_UNK200 is D3OBJECT.pad200[0] (0x200); notes/units/ch.md records the evidence for the role (the per-object int d3_WriteObject saves and rd_object restores, the item pointer the sprite spawners store) so a typed d3.h field is the next step, but d3.h is shared and was not touched in this session. Readability advanced: CHSLOT.unk60 -> playSpeed (0x60, the playback speed the walk code sets from ground type x speedScale). */

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
        if (ch_Unk631058)
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
        if (ch_Unk631058)
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
        if (ch_Unk631058)
            a->ch->flags.all |= 8;
        a->u.walk.flags |= CH_WALK_ARRIVED;
        d3_SetObjectPos(ch->obj, &a->u.walk.dest);
        ch_PlaceOnGround(ch, d3_CurrentScene != 0);
        return;
    }
    ch_WalkRotate(a);
    if (ground && ground != 13 && ch->slot) {
        f = 0.7f;
        if (ch->group == -1 && D3OBJ_UNK200(ch->obj))
            f = ch_Unk5f1290;
        if (ch->kind & 8) {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = ch->unk1a0 * ch->speedScale * f * 1.9f;
            else
                CH_XSLOT(ch)->playSpeed = ch->unk1a0 * ch->speedScale * f * 1.7f;
        } else {
            if (ground == 11 || ground == 6)
                CH_XSLOT(ch)->playSpeed = ch->unk1a0 * ch->speedScale * f * 2.5f;
            else
                CH_XSLOT(ch)->playSpeed = ch->unk1a0 * ch->speedScale * f * 2.2f;
        }
    }
}
