/* 0x4768a0, one frame of a fade: draws the sprite with the alpha of the elapsed time */

void d2_DrawFade(D2FADE *f, D2SURF *surf)
{
    float e, a;
    int on, cx, cy;

    on = 0;
    e = (float)(tb_Calls - f->start);
    if (tb_Calls - f->last > 3)
        e = (float)(int)(f->last - f->start + 3);
    if (f->flags & 1)
        a = e / (float)(int)f->dur;
    else if (f->flags & 2)
        a = 1.0f - e / (float)(int)f->dur;
    if (!(a > 0.0f))
        a = 0.0f;
    else if (!(a < 1.0f))
        a = 1.0f;
    cx = f->w / 2 + f->x;
    cy = f->h / 2 + f->y;
    if (surf->locked != 0) {
        d2_UnlockSurface(surf);
        on = 1;
    }
    d3if_DrawSprite(f->sprite, cx, cy, 0, (float)f->w / (float)f->sprite->size, (float)f->h / (float)f->sprite->size, a);
    if (on)
        d2_LockSurface(surf);
    if ((f->flags & 4) && (f->flags & 8)) {
        if (f->w + f->x > d2_DirtyRectRight)
            d2_AddDirty(d2_DirtyRectRight, 0, d2_ScreenW - d2_DirtyRectRight, d2_ScreenH);
        if (f->y < d2_DirtyRectTop)
            d2_AddDirty(d2_DirtyRectRight, f->y, d2_ScreenW - d2_DirtyRectRight, d2_DirtyRectTop - f->y);
        if (f->h + f->y > d2_DirtyRectBottom)
            d2_AddDirty(d2_DirtyRectRight, d2_DirtyRectBottom, d2_ScreenW - d2_DirtyRectRight, f->h + f->y - d2_DirtyRectBottom);
        d2_ReleaseFade(f);
        return;
    }
    if (((f->flags & 1) && a == 1.0) || ((f->flags & 2) && a == 0.0))
        f->flags |= 4;
    if (f->w + f->x > d2_DirtyRectRight)
        d2_AddDirty(d2_DirtyRectRight, 0, d2_ScreenW - d2_DirtyRectRight, d2_ScreenH);
    if (f->y + f->h > d2_DirtyRectBottom)
        d2_AddDirty(0, d2_DirtyRectBottom, d2_ScreenW, f->y + f->h - d2_DirtyRectBottom);
    f->last = tb_Calls;
}
