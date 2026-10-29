/* 0x404c20 */

void ch_PauseAnim(CHARACTER *ch, int pause)
{
    if (CH_ACTIONS(ch) && ch->slot) {
        if (pause) {
            d3anim_PauseAnim(ch->obj->material->animCtl, ch->slot->data->name);
            ch->flags.all |= 4;
        } else {
            d3anim_PlayAnim(ch->obj->material->animCtl, ch->slot->data->name, 1);
            ch->flags.all &= ~4;
        }
    }
}
