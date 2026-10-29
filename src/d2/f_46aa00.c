/* 0x46aa00, draws a slider (flags 2: horizontal, 1: vertical): track, filled part, thumb, arrows, value @deep B draft, 8 differing instructions (6 code + order at 0x46b5d5 / 0x46b60c): the member load order and ecx/edx split of w->x + w->w + 4 in the two max-label calls (original: x->ecx, w->edx; ours: w->ecx, x->edx; lea [ecx+edx*1+0x4] symmetric, only the loads differ). spell.py: no menu rule matched either hunk ('escalate with the diffclass verdict'); reading found no meaning error (same values, same order of the y/x/w loads otherwise). Leads tried: 13 source forms in rounds 15-19 (both operand orders, parentheses, casts, temp, dead refs, nested struct, d2_Objs[id]. form) and the ~60-variant member-load-order campaign of d2_DrawInput / d2_DrawButton; notes/units/d2.md [z3]/[d2j]. Readability done (d2_DrawSlider, d2_DrawText, d2_ListCopyItem, d2_PutPixel/Clipped, d2_TextDrawnW/H, D2CTRL.loaded). */

void d2_DrawSlider(int id, D2SURF *surf)
{
    D2OBJ *w;
    D2FRAME *spr0, *spr1, *spr3, *spr6;
    void *sprites;
    unsigned int flags;
    int len, value, max, min, range, value2;
    float flen, scale, pos1, pos2, d, t;
    int hilo = 0, hihi = 0;
    int i, j, k, mx, my, tx, ty, tw, dy, color, x1, y1, x2, y2;
    DWORD c;
    unsigned char rgb[3];
    char buf2[32];
    char buf[512];

    sprites = d2_Objs[id].data;
    w = &d2_Objs[id];
    spr1 = d2_GetFrame(sprites, 1);
    spr0 = d2_GetFrame(sprites, 0);
    spr3 = d2_GetFrame(sprites, 3);
    spr6 = d2_GetFrame(sprites, 6);
    value = w->u.s.value;
    flags = w->u.s.flags;
    len = w->u.s.len;
    max = w->u.s.max;
    min = w->u.s.min;
    value2 = w->u.s.value2;
    range = max - min;
    if (range < 1)
        range = 1;
    flen = (float)len;
    scale = flen / range;
    pos1 = (value - min) * scale;
    pos2 = (value2 - min) * scale;
    if (pos1 > 0.0f && pos1 < 1.0f)
        pos1 = 1.0f;
    if (pos2 > 0.0f && pos2 < 1.0f)
        pos2 = 1.0f;
    if (flags & 2) {
        if ((d = flen - pos1) > 0.0f && !(flags & 8))
            d2_BlitPart((int)pos1, spr1->h, (int)(d + 1.0f), spr1->h, w->u.s.img,
                         (int)(w->x + spr0->w + pos1), (spr3->h - spr1->h) / 2 + w->y, surf);
        if (!(flags & 0x400)) {
            if ((flags & 0x10) && pos2 > 0.0f)
                d2_BlitPart(0, spr1->h * 2, (int)pos2, spr1->h, w->u.s.img,
                             spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
            if (pos1 > 0.0f) {
                if (!(flags & 8))
                    d2_BlitPart(0, 0, (int)pos1, spr1->h, w->u.s.img,
                                 spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
                else
                    d2_BlitPart(0, spr1->h, (int)pos1, spr1->h, w->u.s.img,
                                 spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf);
            }
        } else {
            dd_RGB(0xff, 0x80, 0);
            t = pos1 / flen * 2.0f;
            if (t < 1.0) {
                rgb[0] = 0xc4 - (int)(-24.0f * t);
                rgb[1] = (int)(220.0f * t);
                rgb[2] = (int)(t * 0.0f);
            } else {
                t -= 1.0;
                rgb[0] = 0xdc - (int)(194.0 * t);
                rgb[1] = 0xdc - (int)(135.0 * t);
                rgb[2] = (int)(t * 0.0);
            }
            if (w->h + w->y <= surf->bottom && w->y >= surf->top) {
                for (j = 0; j < w->h; j++)
                    for (i = 0; i < pos1; i++) {
                        c = dd_RGB(rgb[0], rgb[1], rgb[2]);
                        d2_PutPixel(w->x + (i + spr0->w), w->y + (j + (spr3->h - spr1->h) / 2), c, surf);
                    }
            } else {
                for (j = 0; j < w->h; j++)
                    for (i = 0; i < pos1; i++) {
                        c = dd_RGB(rgb[0], rgb[1], rgb[2]);
                        d2_PutPixelClipped(w->x + (i + spr0->w), w->y + (j + (spr3->h - spr1->h) / 2), c, surf);
                    }
            }
            d2_SetClipRect(w->x, surf->top, spr0->w + w->x + len, surf->bottom);
            for (k = 0; k < len / spr1->w + 0.5; k++)
                d2_DrawShape(spr1->w * k + spr0->w + w->x, (spr3->h - spr1->h) / 2 + w->y, surf, sprites, 1);
            d2_SetClipRect(w->clipx0, w->clipy0, w->clipx1, w->clipy1);
        }
        if (!(flags & 0x20)) {
            if (d2_PointerObj == w->id) {
                if (w->buf && w->buf->loaded)
                    mx = d2_PointerX;
                else
                    mx = d2_MouseX;
                if (mx >= w->x && mx < spr6->w + w->x)
                    hilo = 1;
                else if (mx >= w->w - spr6->w + w->x && mx < w->w + w->x)
                    hihi = 1;
            }
            w->hilo = hilo;
            w->hihi = hihi;
            d2_DrawShape(w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 0);
            if (hilo)
                d2_DrawShapeMode3(w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 6);
            if (!d2_DrawShape(w->x + (len + spr0->w), (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 5))
                d2_DrawShape(w->x + (len + spr0->w), (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 0);
            if (hihi)
                d2_DrawShapeMode3(w->w - spr6->w + w->x, (spr3->h - spr0->h) / 2 + w->y, surf, sprites, 7);
        }
        if (!(flags & 0x48)) {
            d2_DrawShape((int)((float)spr0->w + w->x + pos1 - spr3->w / 2 + 0.5f), w->y, surf, sprites, 3);
            if (w->list[0]) {
                d2_ListCopyItem(id, value, buf);
                if (w->font)
                    d2_SetFont(w->font);
                else
                    d2_SetFont(d2_DefaultFont + 1);
                x1 = (int)((float)spr0->w + w->x + pos1 - spr3->w / 2);
                y1 = spr3->h / 2 - d2_FontHeight / 2 + w->y + 1;
                d2_DrawTextBox(x1, y1, spr3->w, spr3->h, surf, buf, 8);
                d2_AddDirty(x1, y1, d2_TextDrawnW, d2_TextDrawnH);
            } else {
                if (w->font)
                    d2_SetFont(w->font);
                else
                    d2_SetFont(d2_DefaultFont + 1);
                x2 = (int)((float)spr0->w + w->x + pos1 - spr3->w / 2);
                y2 = spr3->h / 2 - d2_FontHeight / 2 + w->y + 2;
                if (flags & 0x1000)
                    sprintf(buf, "%i%%", w->scale * value);
                else if (flags & 0x800)
                    sprintf(buf, "%i%%", (int)(value * 100.0f / max + 0.5));
                else
                    sprintf(buf, "%i", w->scale * value);
                d2_DrawTextBox(x2, y2, spr3->w, spr3->h, surf, buf, 8);
                d2_AddDirty(x2, y2, d2_TextDrawnW, d2_TextDrawnH);
            }
        }
        if (flags & 4) {
            if (w->font)
                d2_SetFont(w->font);
            else if (w->win)
                d2_SetFont(w->win->font);
            else
                d2_SetFont(d2_DefaultFont + 1);
            if (w->dirty) {
                _itoa(w->scale * value, buf2, 10);
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_DrawText((int)(w->x + pos1), w->y - 0x10, surf, buf2, 0);
                d2_AddDirty((int)(w->x + pos1), w->y - 0x10, d2_TextDrawnW, d2_TextDrawnH);
            }
            sprintf(buf2, "%i", min);
            dy = (d2_FontHeight - spr0->h) / 2 - 1;
            d2_DrawText(w->x - d2_TextWidth(buf2, d2_CurrentFont) - 4, w->y - dy, surf, buf2, 0);
            d2_AddDirty(w->x - 0x16, w->y + dy, d2_TextDrawnW, d2_TextDrawnH);
            sprintf(buf2, "%i", max);
            d2_DrawText(w->x + w->w + 4, w->y - dy, surf, buf2, 0);
            d2_AddDirty(w->x + w->w + 4, w->y - dy, d2_TextDrawnW, d2_TextDrawnH);
        }
        if (w->label[0]) {
            color = 8;
            if (w->font)
                d2_SetFont(w->font);
            else
                d2_SetFont(d2_DefaultFont + 1);
            tx = w->x;
            ty = w->y - 0xd;
            tw = w->w;
            if (flags & 0x80) {
                tw = d2_TextWidth(w->label, d2_DefaultFont);
                ty = w->y;
                tx = w->x - tw - 0x18;
                color = 0x10;
            }
            d2_DrawTextBox(tx, ty, tw, 0x20, surf, w->label, color);
            d2_AddDirty(tx, ty, d2_TextDrawnW, d2_TextDrawnH);
        }
    } else if (flags & 1) {
        if ((d = flen - pos1) > 0.0f && !(flags & 8))
            d2_BlitPart(spr1->w, 0, spr1->w, (int)(d + 1.0f + 1.5), w->u.s.img,
                         (spr3->w - spr1->w) / 2 + w->x, spr0->h + w->y, surf);
        if ((flags & 0x10) && pos2 > 0.0f)
            d2_BlitPart(spr1->w * 2, (int)(flen - pos2 + 1.0f + 0.5), spr1->w, (int)(pos2 + 1.5), w->u.s.img,
                         (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos2 - spr0->h + 0.5), surf);
        if (pos1 > 0.0f) {
            if (!(flags & 8))
                d2_BlitPart(0, (int)(d + 1.0f + 0.5), spr1->w, (int)(pos1 + 1.5), w->u.s.img,
                             (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos1 - spr0->h + 0.5), surf);
            else
                d2_BlitPart(spr1->w, 0, spr1->w, (int)(pos1 + 1.5), w->u.s.img,
                             (spr3->w - spr1->w) / 2 + w->x, (int)(w->h + w->y - pos1 - spr0->h + 0.5), surf);
        }
        if (!(flags & 0x20)) {
            if (d2_PointerObj == w->id) {
                if (w->buf && w->buf->loaded)
                    my = d2_PointerY;
                else
                    my = d2_MouseY;
                if (my >= w->y && my < spr6->h + w->y)
                    hilo = 1;
                else if (my >= w->h - spr6->h + w->y && my < w->h + w->y)
                    hihi = 1;
            }
            w->hilo = hilo;
            w->hihi = hihi;
            d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y, surf, sprites, 0);
            if (!d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 5))
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 0);
            if (hilo)
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y, surf, sprites, 6);
            if (hihi)
                d2_DrawShape((spr3->w - spr0->w) / 2 + w->x, w->y + (len + spr0->h), surf, sprites, 7);
        }
        if (!(flags & 8) && !(flags & 0x40))
            d2_DrawShape(w->x, (int)(w->h - spr0->h + w->y - pos1 - spr3->h / 2), surf, sprites, 3);
        if (flags & 4) {
            if (w->dirty && !(flags & 8)) {
                _itoa(value, buf2, 10);
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_SetFont(d2_DefaultFont + 1);
                d2_DrawText(w->x - 0x18, (int)(w->h + w->y - pos1), surf, buf2, 0);
                d2_AddDirty(w->x - 0x18, (int)(w->h + w->y - pos1), d2_TextDrawnW, d2_TextDrawnH);
            }
        }
    }
    w->dirty = 0;
}
