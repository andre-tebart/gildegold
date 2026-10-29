/* 0x40b530, attaches the sitting animation at its last frame */

void ch_SetSitAnim(CHARACTER *ch, char *name)
{
    D3ANIMSLOT *s;

    CH_SLOT2(ch) = s = (D3ANIMSLOT *)ch_AttachAni(ch, name, 1);
    ch->slot = 0;
    if (s) {
        d3anim_SetFrame(ch->obj, s, -1);
        CH_SLOT2(ch)->flags.b[0] = 1;
        *(unsigned int *)&CH_SLOT2(ch)->flags.b[4] &= ~3;
        CH_SLOT2(ch)->flags.b[1] |= 0x10;
    }
}
