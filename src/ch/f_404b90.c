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
        if (a->func2 && (s = ch->slot) && !CH_UNK080(ch) && CH_FRAME(ch) != s->flags.b[0]
            && s->frame >= a->unk08) {
            a->func2(a);
            CH_FRAME(ch) = ch->slot->flags.b[0];
        }
    }
    return 1;
}
