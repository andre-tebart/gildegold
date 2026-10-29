/* 0x45fbc0 */

void tex_Release(TSTEXTURE *tex)
{
    TSTEXTURE *t;
    unsigned int i, j;
    int index, n;

    if (tex && tex->refs > 0) {
        if (tex->anim > 0 && !tex->frame) {
            for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                if (t->refs > 0 && t->group == tex->group && t->frame > 0)
                    tex_Release(t);
            }
        }
        tex->refs--;
        if (tex->refs <= 0) {
            if (tex->alias) {
                tex->surf = 0;
                tex->d3dtex = 0;
                tex->data = 0;
            } else {
                index = tex - d3_Textures;
                for (i = 0, t = d3_Textures; i < d3_TexCount; i++, t++) {
                    if (t->refs > 0 && t->alias && t->mask == index) {
                        t->alias = 0;
                        t->mask = tex->mask;
                        n = t - d3_Textures;
                        for (j = i + 1, t++; j < d3_TexCount; j++, t++) {
                            if (t->refs > 0 && t->alias && t->mask == index)
                                t->mask = n;
                        }
                        break;
                    }
                }
                if (i == d3_TexCount)
                    tex_Free(tex);
            }
            tex->refs = 0;
            ts_TexUsed--;
        }
    }
}
