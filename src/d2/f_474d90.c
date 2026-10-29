/* 0x474d90, copies the two pictures named tex1 / tex2 into the view's two textures and the view's image (one
   above the other) */

void d2_FillViewTextures(int view, char *tex1, char *tex2)
{
    D2VIEW *v = &d2_Views[view];
    D2TEXX *t1 = 0;
    D2TEXX *t2;
    char *s, *d;
    char c;

    if (tex1 != 0) {
        s = tex1;
        d = v->tex1;
        do {
            c = *s;
            d[s - tex1] = c;
            s++;
        } while (c != 0);
        t1 = (D2TEXX *)tex_Find(tex1);
        if (t1 != 0) {
            tex_SetHiColor((D3TEXTURE *)t1, 1);
            d2_BlitPartFromDD(0, 0, t1->size, t1->size, t1->surf, 0, 0, (void *)v->texSurf);
        }
    }
    if (tex2 != 0) {
        s = tex2;
        d = v->tex2;
        do {
            c = *s;
            d[s - tex2] = c;
            s++;
        } while (c != 0);
        t2 = (D2TEXX *)tex_Find(tex2);
        if (t2 != 0) {
            tex_SetHiColor((D3TEXTURE *)t2, 1);
            d2_BlitPartFromDD(0, 0, t2->size, ((D2SURF *)v->texSurf)->h - t1->size, t2->surf, 0, t1->size,
                         (void *)v->texSurf);
        }
    }
    d2_BlitPart(0, 0, ((D2SURF *)v->viewSurf)->w, ((D2SURF *)v->viewSurf)->h, (void *)v->texSurf, 0, 0,
                 (void *)v->viewSurf);
}
