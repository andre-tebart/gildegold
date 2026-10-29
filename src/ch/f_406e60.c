/* 0x406e60, turn action: args[0] degrees (args[1]: relative to the z axis), plays "dreh_90_*" and turns @deep behaviour: far draft, 125 differing instructions / 586 bytes. Same flow; the code hunks are the two D3ANIMFLAGS/key blocks (the original builds the flags block twice, one per branch of if (ch->slot), as a load/or/or/store after a dword zero store with dword 1 enregistered, and loads turn.z/turn.y/turn.x and key[0].time before the stores) and the key[] stores. Read against 406e60..407200. Last-bytes leads tried this session: the 406f5f branch spell (jp vs and eax,0x100;je) both ways - !(angle<0.2f) with the two attach calls swapped gave 135 asm lines, angle>=0.2f 137 lines, the kept nested if (angle<0.2) form is 125 (baseline) so both reverted; spell reload at 406fbf/407098 and dup-tail at 4071fb need register/slot variants. Earlier round-5 attempts (flags b1|=0x14;b1|=8 folded to 0x1c or hoisted, memset stores dword 1, fr=&frames[n-1], keys order) are in notes/units/ch.md. Readability done: ch->slot typed, CHARACTER+0x78 named turnAnim (CH_ANIM078 macro gone), no Q failure left. */

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
