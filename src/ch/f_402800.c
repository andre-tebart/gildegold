/* 0x402800 */

void ch_UpdatePositions(void)
{
    int i;
    CHARACTER *ch;
    D3VECTOR4 pos;

    for (i = 0; i < CH_MAX; i++) {
        ch = ch_List[i];
        if (ch && ch->flags.dirty && ch->scene == d3_DefaultScene) {
            pos.x = ch->obj->pos.x + ch->offset.x;
            pos.y = ch->obj->pos.y + ch->offset.y;
            pos.z = ch->obj->pos.z + ch->offset.z;
            if (ch->anim) {
                if (ch->anim->name[9] != '-')
                    ch_SetPos(ch, (D3VECTOR *)&pos, 0);
            } else
                ch_SetPos(ch, (D3VECTOR *)&pos, 0);
            ch->flags.dirty = 0;
        }
    }
}
