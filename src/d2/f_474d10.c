/* 0x474d10, forgets the two textures of a view: their images are cleared */

void d2_ClearViewTextures(int view)
{
    D2VIEW *v = &d2_Views[view];
    int i;

    for (i = 0; i < 2; i++) {
        char *name;
        D2TEXX *t;

        if (i == 0)
            name = v->tex1;
        else if (i == 1)
            name = v->tex2;
        else
            continue;
        if (*name != 0) {
            t = (D2TEXX *)tex_Find(name);
            if (t != 0) {
                unsigned int sz = t->size;

                if (i == 0)
                    d2_BlitPartToDD(0, i, sz, sz, (void *)v->texSurf, 0, 0, t->surf);
                else
                    d2_BlitPartToDD(0, sz, sz, sz, (void *)v->texSurf, 0, 0, t->surf);
            }
        }
    }
    v->tex1[0] = 0;
    v->tex2[0] = 0;
}
