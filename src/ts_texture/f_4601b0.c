/* 0x4601b0 */

void tex_AddRef(TSTEXTURE *tex, char all)
{
    TSTEXTURE *t;
    unsigned int i;

    if (tex) {
        if (all && tex->anim > 0 && !tex->frame && tex->group != -1) {
            for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                if (t->refs > 0 && t->group == tex->group)
                    t->refs++;
            }
        } else
            tex->refs++;
    }
}
