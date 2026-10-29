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
