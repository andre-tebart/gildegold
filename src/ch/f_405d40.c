/* 0x405d40, action callback: plays a->u.name and takes the object name2 at frame args[0] */

void ch_ActTakingObject(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    char buf[0x100];

    if (!a->count) {
        if (a->u.name[0])
            CH_ACTOBJ(a)->unk170 = (int)ch_SetNextAnim(ch, a->u.name, 1);
        else {
            ch_SetHandObject(ch, a->args[2], CH_ACTOBJ(a)->name2);
            ch_RemoveAction(a);
            return;
        }
    }
    if (a->args[0] != 99999999) {
        if (CH_UNK080(ch))
            return;
        if (ch->slot && ch->slot->frame >= a->args[0]) {
            ch_SetHandObject(a->ch, a->args[2], CH_ACTOBJ(a)->name2);
            a->args[0] = 99999999;
        }
    }
    if (CH_UNK080(ch))
        return;
    if (!ch_AnimDone(ch))
        return;
    if (a->args[0] != 99999999) {
        sprintf(buf, "ch_TakingObject(): object not taken yet (should be in frame:%i)!", a->args[0]);
        ch_SetHandObject(a->ch, a->args[2], CH_ACTOBJ(a)->name2);
    }
    if (CH_UNK080(ch))
        sprintf(buf, "ch_TakingObject(): c->NextAnim still valid %s!\n", CH_UNK080(ch));
    ch_CheckAniMorph(ch);
    ch_RemoveAction(a);
}
