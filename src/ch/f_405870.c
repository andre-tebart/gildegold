/* 0x405870, action callback: sits down with the animation a->u.name */

void ch_ActSitDown(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    int cur;

    if (!a->count) {
        if (a->u.name[0] && !D3OBJ_ANIM(ch->obj)->slots[0].data) {
            ch->flags.all |= 2;
            if (CHACT_UNK18C(a) & 1) {
                CH_SLOT2(ch) = ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, a->u.name, 1);
                d3anim_SetFrame(ch->obj, ch->slot, -1);
            } else
                CH_SLOT2(ch) = ch->slot = (D3ANIMSLOT *)ch_AttachAni(a->ch, a->u.name, 1);
        }
        if (ch->slot) {
            ch->slot->flags.b[0] = 1;
            ((int *)&ch->slot->flags)[1] &= ~3;
            ch->slot->flags.b[1] |= 0x10;
        }
    } else if (ch_ActAnim(a) == -1) {
        cur = d3_CurrentScene;
        d3if_SelectScene(d3t_SceneIndex(ch->scene), 1);
        a->ch->flags.all |= 0x10;
        a->ch->flags.all &= ~2;
        d3if_SelectScene(cur, 1);
        ch_RemoveAction(a);
    }
}
