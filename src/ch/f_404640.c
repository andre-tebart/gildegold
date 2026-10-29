/* 0x404640, action callback: plays the action's animation */

int ch_ActAnim(CHACTION *a)
{
    CHARACTER *ch = a->ch;
    D3ANIMSLOT *s;

    if (!ch->slot && !a->count) {
        ch->slot = ch_AttachMotion(ch, a->type);
        return a->unk10;
    }
    if (ch->slot && (ch->slot->flags.b[1] & 0x20)) {
        s = ch->slot;
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(ch->obj, s, 0, s->frame, 3);
        if (!(ch->flags.bytes.lo & 2))
            d3_ReleaseTexture(a->ch->obj->material->animCtl, ch->slot->data->name);
        ch->slot = 0;
        return -1;
    }
    if (ch->slot && a->stop) {
        s = ch->slot;
        if (!s->data->relative && (s->flags.b[4] & 3))
            d3anim_ApplyRootMotion(ch->obj, s, 0, s->frame, 3);
        if (!(ch->flags.bytes.lo & 2))
            d3_ReleaseTexture(a->ch->obj->material->animCtl, ch->slot->data->name);
        ch->slot = 0;
        return -1;
    }
    if (!ch->slot)
        return -1;
    return a->unk10;
}
