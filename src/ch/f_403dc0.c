/* 0x403dc0, ends a finished animation or morph and starts the next animation, blending into it @deep behaviour: far (123 code + 26 slots + 4 order + 4 fill differing instructions), read hunk by hunk against build/ghidra/403dc0.c and tools/asmdiff.py: statements, constants, callees, field offsets and conditions match. Left: the original's redundant re-test if (!ch->morph && !ch->slot) at 40408b-404096 (ours proves both are 0 and drops it; earlier tries, notes/units/ch.md: if/else-if, separate ifs, swapped order, local copy - only a volatile read keeps it), six reload hunks (spell 403e87, 403ed7, 403f58, 40400f, 404113, 40418b: the original re-reads ch->morph / ch->slot / slots[0] fields where ours keeps the value; the source's own locals are used in both), dup-tail 4041fc (artifact of the re-test), and the slots hunks 403dd3 / 4040ca (released/f/n at [esp+0x10..0x20]; earlier tries: initialization places, declaration in block) - no meaning-class lead left. Q2 left: CHARACTER.flags.unk1 (bit 0x02) has one reader (ch.c 0x848) and no writer in any drafted source - no evidence for its role. Readability done this session: ch_MorphTimeout -> ch_MorphTimeout (-1), ch_MotionTimeout -> ch_MotionTimeout (2), D3ANIMFRAME.unk00 -> time (frame start time, ch.c 0x2332 writes it to D3ANIMKEY.time), CHARACTER.lastActionFrame -> lastActionFrame (CH_FRAME, written -1 here, compared with slot->flags.b[0] in ch_UpdateAction 0x404b90); Q1/Q3/Q4/Q5/Q6/Q7 pass. */

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
    if (s && (s->flags.b[1] & 0x20) && !(s->flags.b[8] & 8) && !ch->flags.unk1) {
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
    if (ch->next && !ch->morph) {
        if (ch->slot) {
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
}
