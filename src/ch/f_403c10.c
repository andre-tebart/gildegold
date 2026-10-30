/* 0x403c10, ends the current animation, blending from its frame into a morph */

void ch_StopAnim(CHARACTER *ch)
{
    D3OBJECT *obj = ch->obj;
    CHANIMFLAGS f;
    char name[0x60];
    D3ANIMSLOT *s;

    if (ch->slot) {
        sprintf(name, "morph_%i", ch);
        if (!d3_FindAnim(name)) {
            if (obj->info && d3anim_CreateMorphAnim(D3OBJ_ANIM(obj)->mesh, ch->slot->data,
                                                      &ch->slot->data->frames[ch->slot->frame], 0, 0,
                                                      D3OBJ_ANIM(obj)->mesh, name, 4)) {
                f.u.all = 0;
                f.b8 = 1;
                f.u.c.b1 = 0x18;
                f.b = 0;
                f.a = 0;
                ch->morph = (D3ANIMSLOT *)d3anim_StartAnimCh(obj->material->animCtl, name, f);
                if (ch->morph) {
                    if (!d3t_SceneIndex(ch->scene))
                        ch->morph->timeout = -1;
                    else
                        ch->morph->timeout = ch_MorphTimeout;
                }
            }
            s = ch->slot;
            if (!s->data->relative && (s->flags.b[4] & 3))
                d3anim_ApplyRootMotion(obj, s, 0, s->frame, 3);
            d3_ReleaseTexture(obj->material->animCtl, s->data->name);
            ch->slot = 0;
        }
    }
}
