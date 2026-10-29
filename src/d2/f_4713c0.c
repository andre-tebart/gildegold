/* 0x4713c0, draws the background of window w onto surf: frame, saved images, sprite backgrounds, the tiled
   pictures (scrolled with the text) and the overlay image. The window's backgroundState picks the saved-image
   path (2: save the window's area into surf0, 1: put the saved image back, 0: nothing pending); the global
   d2_NoBackgroundRestore (char, role from its six tests: 1 draws the caption texture without the
   saved-background path; nothing in the sources writes it) forces the alternate path. */

void d2_DrawWindowBackground(D2WIN *w, void *surf)
{
    int x0, y0, ww, hh;
    void *pic, *fp;
    D2FRAME *fr;
    D2PIC *p;
    int top, left, c0, c1, c2, c3;
    int i, j, yy, xx, th, tw, rows, cols;

    if (w->flags & 0x400)
        d2_ClearRect(w->x, w->y, w->w, w->h, surf);
    if (!(w->flags & 0x400) && !(w->flags & 1) && d2_NoBackgroundRestore != 1 && w->backgroundState < 2 && (w->flags & 0x1000))
        d2_BlitPart(0, 0, w->w, w->h, w->surf0, w->x, w->y, surf);
    if (w->backgroundState == 2 && (w->flags & 0x1000) && d2_NoBackgroundRestore != 1)
        d2_BlitPart(w->x, w->y, w->w, w->h, surf, 0, 0, w->surf0);
    x0 = w->padl + w->x;
    y0 = w->padt + w->y;
    ww = w->w - w->padr - w->padl;
    hh = w->h - w->padb - w->padt;
    if ((w->flags & 1) && w->backgroundState == 2 && d2_NoBackgroundRestore != 1) {
        if (d2_CaptionTex && surf == d2_ScreenSurf) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, ww / 2 + x0, hh / 2 + y0, 0,
                         (float)ww / (float)d2_CaptionTex->size, (float)hh / (float)d2_CaptionTex->size, w->alpha);
            d2_LockSurface(d2_ScreenSurf);
        } else
            d2_FillRect(w->x, w->y, w->w, w->h, surf);
        if (w->flags & 0x1000)
            d2_BlitPart(w->x, w->y, w->w, w->h, surf, 0, 0, w->surf1);
    }
    if ((w->flags & 1) && d2_NoBackgroundRestore == 1) {
        if (d2_CaptionTex && surf == d2_ScreenSurf) {
            d2_UnlockSurface(d2_ScreenSurf);
            d3if_DrawSprite(d2_CaptionTex, ww / 2 + x0, hh / 2 + y0, 0,
                         (float)ww / (float)d2_CaptionTex->size, (float)hh / (float)d2_CaptionTex->size, w->alpha);
            d2_LockSurface(d2_ScreenSurf);
        } else
            d2_FillRect(w->x, w->y, w->w, w->h, surf);
    }
    if (w->spr && surf == d2_ScreenSurf) {
        d2_UnlockSurface(d2_ScreenSurf);
        d3if_DrawSprite(w->spr, ww / 2 + x0, hh / 2 + y0, 0,
                     (float)ww / (float)w->spr->size, (float)hh / (float)w->spr->size, w->alpha);
        d2_LockSurface(d2_ScreenSurf);
    }
    if (w->flags & 2)
        d2_DrawRectOutline(w->x, w->y, w->w - 1, w->h - 1, 0xff, 0xff, 0xff, surf);
    if (w->bgpic != -1 && (pic = d2_GetPic(w->bgpic)) != 0) {
        top = 0;
        left = 0;
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        if (w->flags & 4) {
            fp = d2_GetPic(w->frame);
            top = d2_GetFrame(fp, 4)->h;
            fr = d2_GetFrame(fp, 6);
            left = fr->w;
        }
        d2_SetClipRect(w->x + left, w->y + top, w->w - left + w->x, w->h - top + w->y);
        p = &d2_Pics[w->bgpic];
        if (w->scrollY >= 0)
            yy = w->y - w->scrollY % p->h;
        else
            yy = w->y - w->scrollY % p->h - p->h;
        for (j = 0; (float)j < (float)(w->h / p->h) + 1.5f; j++)
            for (i = 0; (float)i < (float)(w->w / p->w) + 1.5f; i++)
                d2_DrawShape(w->x + left + p->w * i, p->h * j + yy + top, surf, pic, 0);
        d2_SetClipRect(c0, c1, c2, c3);
    }
    if (w->tileBackground) {
        th = d2_BackgroundTile->h;
        tw = d2_BackgroundTile->w;
        d2_SetSurfaceClip(w->x, w->y, w->w, w->h, surf);
        if (w->scrollY >= 0)
            yy = w->y - w->scrollY % th;
        else
            yy = w->y - w->scrollY % th - th;
        if (w->scrollX >= 0)
            xx = w->x - w->scrollX % tw;
        else
            xx = w->x - w->scrollX % tw - tw;
        rows = w->h / th + 2;
        cols = w->w / tw + 2;
        for (j = 0; j < rows; j++)
            for (i = 0; i < cols; i++)
                d2_BlitPart(0, 0, d2_BackgroundTile->w, d2_BackgroundTile->h, d2_BackgroundTile, xx + i * tw, yy + j * th, surf);
        d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, surf);
    }
    if (w->surf2)
        d2_BlitPartMode(0, 0, w->surf2->w, w->surf2->h, w->surf2, w->x, w->y, surf, 1);
}
