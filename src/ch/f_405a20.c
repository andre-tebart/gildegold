/* 0x405a20, action callback: stands up with the animation a->u.name */

void ch_ActStandUp(CHACTION *a)
{
    CHARACTER *ch = a->ch;

    if ((ch->slot || ch->morph) && !(a->unk10 & 1)) {
        a->count = -1;
        return;
    }
    a->unk10 |= 1;
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
