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
