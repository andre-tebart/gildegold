/* 0x408cb0, starts the walk animation; its speed follows the cell size of the height map */

int ch_StartWalkAnim(CHACTION *a, D3SM *h)
{
    CHARACTER *ch = a->ch;
    float f;
    CHSLOT *s;
    D3ANIMDATA *ad;
    char name[0x60];

    CH_UNK080(ch) = 0;
    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        ad = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(ad, 1);
        ch->morph = 0;
    }
    d3t_ReleaseObjectTextures(ch->obj);
    if (!ch->obj->info)
        ch_Unk402540(ch);
    if (ch->transport)
        s = (CHSLOT *)ch_AttachAni(ch, "bewegung/karren_ziehen", 1);
    else
        s = (CHSLOT *)ch_AttachAni(ch, "bewegung/gehen", 1);
    if (!(ch->slot = (D3ANIMSLOT *)s)) {
        if (ch_Unk631058)
            a->ch->flags.bytes.lo |= 8;
        if (a->u.walk.path) {
            mem_Free(a->u.walk.path);
            a->u.walk.path = 0;
        }
        ch_RemoveAction(a);
        return 0;
    }
    s->f6d_10 = 0;
    s = CH_XSLOT(ch);
    s->b70 = 0;
    s->a70 = 1;
    if (ch->kind & 8)
        CH_XSLOT(ch)->speed = 30.0f;
    else if (ch->kind & 0x10)
        CH_XSLOT(ch)->speed = 60.0f;
    else
        CH_XSLOT(ch)->speed = 15.0f;
    f = 50.0f / h->cell.x;
    f = min(2.0f, f);
    CH_XSLOT(ch)->speed *= f;
    CH_XSLOT(ch)->play = 1;
    CH_XSLOT(ch)->f74_8 = 0;
    CH_XSLOT(ch)->f74_4 = 1;
    CH_XSLOT(ch)->f6d_20 = 0;
    a->u.walk.started = 1;
    return 1;
}
