/* 0x4059c0, drops the sitting animation */

void ch_ReleaseSit(CHARACTER *ch)
{
    if (ch->flags.all & 0x10) {
        if (CH_SLOT2(ch)) {
            if (!ch->obj->info)
                ch_Unk402540(ch);
            if (CH_SLOT2(ch)->data)
                d3_ReleaseTexture(ch->obj->material->animCtl, CH_SLOT2(ch)->data->name);
            CH_SLOT2(ch) = 0;
        }
        ch->flags.all &= ~0x10;
    }
}
