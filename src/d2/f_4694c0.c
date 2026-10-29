/* 0x4694c0, draws an input field (and the slider-like frame around it) @deep B draft: 3 code hunks (6 instructions) at 0x4697a3/0x4697d6/0x4697e9 in the number-input frame block (in->w = p0->w + in->ndigits * 0xe + p1->w; o->w = in->w; bw = in->w - p0->w - p1->w; d2_DrawShape(o->x - p1->w + o->w, ...)): the original loads the WORD o->w ([edi+0x14]) where ours reuses the int in->w ([esi+0x10]) and reads o->x/o->w in the other order - same values, different loads; bw = o->w - p0->w - p1->w tried, 6 code hunks became 165 (whole-function allocation), reverted; Q2 needs 12 names (d2_DrawText, d2_DrawShapeMode2, d2_PointerObj/14, d2_ButtonPulldownPic, d2_PendingCaptionWin/9bc, D2OBJ.textShadow/greyed/picW/picH/1e4) */

void d2_DrawInput(int idx, void *surf)
{
    D2INPUT *in = &d2_Inputs[idx];
    D2OBJ *o;
    D2PICHDR *pic;
    D2PICHDR *ip;
    D2FRAME *p0, *p1, *p2;
    unsigned int flags;
    int n, v, bw, i, hi, x0, y1;
    volatile int hv;
    char text[32];
    char fmt[32];
    char pct[64];

    d2_SetFont(in->font);
    flags = in->flags;
    if (flags & 1) {
        if (in->icon == -1 && !(flags & 0x200))
            in->w = d2_TextWidth(in->text, in->font);
        else
            in->w = d2_Objs[in->obj].w;
        if (in->w < 0x30)
            in->w = 0x30;
        if (in->maxw > 0)
            in->w = min(in->w, in->maxw);
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        d2_Objs[in->obj].w = in->w;
        if (d2_Objs[in->obj].highlighted == 0 && d2_PointerObj != in->obj)
            d2_DrawText(in->x, in->y, surf, in->text, 0);
        else
            d2_DrawText(in->x, in->y, surf, in->text, 2);
    } else if (flags & 2) {
        n = 0;
        v = in->maxval;
        do {
            if (v <= 0)
                break;
            v /= 10;
            n++;
        } while (n < 10);
        in->ndigits = n;
        if (flags & 4) {
            if (in->mult * in->value < 1000)
                sprintf(text, "%li g", in->mult * in->value);
            else if (in->mult * in->value < 1000000)
                sprintf(text, "%.1f kg", in->mult * in->value * 0.001);
            else
                sprintf(text, "%.1f t", in->mult * in->value * 1e-06);
        } else if (flags & 8) {
            sprintf(fmt, "%%0%ii Stk.", in->ndigits);
            sprintf(text, fmt, in->mult * in->value);
        } else {
            sprintf(text, "%i", in->value);
        }
        if (in->icon == -1) {
            o = &d2_Objs[in->obj];
            pic = d2_GetPic(d2_ButtonPulldownPic);
            p0 = d2_GetFrame(pic, 0);
            p1 = d2_GetFrame(pic, 1);
            p2 = d2_GetFrame(pic, 2);
            in->w = p0->w + in->ndigits * 0xe + p1->w;
            o->w = in->w;
            o->x = in->x;
            bw = in->w - p0->w - p1->w;
            d2_DrawShape(o->x, o->y, surf, pic, 0);
            d2_DrawShape(o->x - p1->w + o->w, o->y, surf, pic, 1);
            x0 = o->x + (unsigned)p0->w;
            d2_SetClipRect(x0 - 1, d2_ClipTop, x0 + bw, d2_ClipBottom);
            i = 0;
            if ((double)(bw / p2->w) + 0.5 > 0.0) {
                do {
                    d2_DrawShape(p2->w * i + p0->w + o->x, o->y, surf, pic, 2);
                    i++;
                } while ((double)i < (double)(bw / p2->w) + 0.5);
            }
            d2_SetFont(d2_DefaultFont);
            d2_DrawTextBox(o->x, o->y + 1, bw, o->h, surf, text, 0x10);
        } else {
            ip = d2_GetPic(in->icon);
            d2_Objs[in->obj].picW = ip->w;
            d2_Objs[in->obj].picH = ip->h;
            if (d2_Objs[in->obj].textShadow)
                d2_DrawShapeMode2(in->x + 8, in->y + 8, d2_ScreenSurf, ip, d2_PicShift);
            d2_DrawShape(in->x, in->y, surf, ip, d2_PicShift);
            o = &d2_Objs[in->obj];
            if (d2_Objs[in->obj].greyed) {
                if (o->fillFraction == 0.0f)
                    d2_DrawShapeMode2(in->x, in->y, surf, ip, d2_PicShift);
            }
            o = &d2_Objs[in->obj];
            if (d2_Objs[in->obj].fillFraction != 0.0f) {
                hv = hi = d2_ClipBottom;
                y1 = (int)((1.0 - o->fillFraction) * o->h + in->y);
                if (hi < y1)
                    y1 = hi;
                d2_SetClipRect(d2_ClipLeft, d2_ClipTop, d2_ClipRight, y1);
                d2_DrawShapeMode2(in->x, in->y, d2_ScreenSurf, ip, d2_PicShift);
                d2_SetClipRect(d2_ClipLeft, d2_ClipTop, d2_ClipRight, hv);
                if (d2_PointerObj != o->id) {
                    sprintf(pct, "%i%%", (int)((d2_Objs[in->obj].fillFraction > 1.0 ? 1.0 : d2_Objs[in->obj].fillFraction) * 100.0));
                    d2_SetFont(d2_DefaultFont + 1);
                    d2_DrawTextBox(in->x, o->y + o->h - 0xf, ip->w, in->h, surf, pct, 0x28);
                }
            }
        }
        if (in->icon == -1 || (in->flags & 0x100) || in->value == 0 || (in->flags & 0x20)) {
            strcpy(in->text, text);
        } else if (d2_Objs[in->obj].highlighted) {
            d2_DrawText(in->x, d2_Pics[in->icon].h + in->y, surf, text, 2);
        } else {
            d2_DrawText(in->x, d2_Pics[in->icon].h + in->y, surf, text, 0);
        }
    }
    if (d2_HitObjId != -1) {
        o = &d2_Objs[in->obj];
        if (d2_Objs[in->obj].label[0x20] && d2_Objs + d2_HitObjId == o && d2_FocusedInput == 0) {
            d2_PendingCaptionObj = o;
            d2_PendingCaptionWin = o->win;
        }
    }
}
