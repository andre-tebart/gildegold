/* 0x46e260, hit test through the textures of the 3D views: the object under the point on a texture */

int d2_HitTestViews(int x, int y)
{
    D2VIEW *v;
    D2TEX *t;
    D2WIN *win;
    D2OBJ *o;
    char *s;
    float u, w;
    int n, j, k, i, px;

    d2_PointerX = 0;
    d2_PointerY = 0;
    d2_PointerTexName[0] = 0;
    for (n = 0; n < 0x30; n++) {
        v = &d2_Views[n];
        if (v->active && v->loaded && v->shown) {
            for (j = 0; j < 2; j++) {
                if (j == 0)
                    s = v->tex1;
                else if (j == 1)
                    s = v->tex2;
                else
                    continue;
                if (*s && (t = tex_Find(s)) != NULL && d3_ScreenToUV((float)x, (float)y + 5.0f, 0, t, &u, &w)) {
                    px = (int)(u * t->size);
                    y = (int)(w * t->size);
                    if (j == 1)
                        y += 0x100;
                    d2_PointerX = px;
                    d2_PointerY = y;
                    strcpy(d2_PointerTexName, (char *)t);
                    for (k = v->nwins - 1; k >= 0; k--) {
                        win = &d2_Wins[v->wins[k]];
                        for (i = 0; i < win->nobjs; i++) {
                            o = &d2_Objs[win->objs[i]];
                            if (px >= o->x && px <= o->x + o->w && y >= o->y && y <= o->y + o->h) {
                                if (o->type != 0x40) {
                                    d2_PointerObj = win->objs[i];
                                    if (o->win)
                                        d2_PointerWin = o->win->id;
                                    return o->pic;
                                }
                                d2_PointerWin = o->input;
                            }
                        }
                    }
                    d2_PointerObj = -1;
                    return -1;
                }
            }
        }
    }
    return -1;
}
