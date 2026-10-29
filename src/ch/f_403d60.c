/* 0x403d60 */

void ch_ReleaseMorph(CHARACTER *ch)
{
    char name[0x60];
    D3ANIMDATA *a;

    if (ch->morph) {
        sprintf(name, "morph_%i", ch);
        d3_ReleaseTexture(ch->obj->material->animCtl, name);
        a = (D3ANIMDATA *)d3_FindAnim(name);
        d3anim_Release(a, 1);
        ch->morph = 0;
    }
}
