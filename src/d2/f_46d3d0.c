/* 0x46d3d0, draws all objects in drawing order, each clipped to its windows @deep draft (93 differing instructions of 1137 vs 1156; 997/3456 bytes). The reading is register allocation and scheduling only: the mode flags variable sits in edi where the original uses ebp (spell regs at 46db68, the whole 46db68..46dbf5 block), the caption/dirty blocks push the same arguments in another order (46dbfa..46dc7f), and the 46e0d4..46e102 stretch is instruction-stream misalignment (garbage decode on one side) with 46e14c.. alignment fill. spell dup-tail at 46dbb0 (5 instructions, the caption bounds) tried as if/else instead of the ternaries tx = captionX != -1 ? ... : ...: identical bytes, reverted; pull-after-branch and polarity swap do not apply (the arms compute different expressions, no common statement). code at 46e0fc: no menu rule, the bytes are a misaligned decode. Q1 fixed (renamed d2_Unk46d3d0 this session); Q2 blocked: d3io_NoopCallback (empty no-op, no evidence, @deep in d3_io_a.c), d2_NoBackgroundRestore (char flag only compared to 1, no writer in our source), d2_SetSurfaceClip (defined in gfx.c 0x479cd0, its prototype would have to move unit), D2OBJ/D2WIN unk00/unk58/unk6e/unk268 (no evidence in this function's reads); fillFraction named this session. Left: the ebp/edi allocation and the push scheduling (listing/c2prio/siblings). */

void d2_DrawObjects(void)
{
    D2OBJ *o, *p;
    D2WIN *win;
    D2PICHDR *pic;
    void *frame;
    D2FRAME *part;
    unsigned int fx, fy;
    int i, x, y, x0, x1, y0, y1, yy, mode, font, tx, ty, tw, th;
    char buf[64];

    i = 0;
    d2_PendingCaptionWin = 0;
    d2_PendingCaptionObj = 0;
    d2_StepAnims();
    for (; d2_DrawOrder[i] && i < 0x2fe; i++) {
        o = d2_DrawOrder[i];
        x = o->x;
        y = o->y;
        if ((!o->buf || !o->buf->loaded) && o->used && !o->disabled) {
            x0 = o->clipx0;
            x1 = o->clipx1;
            y0 = o->clipy0;
            y1 = o->clipy1;
            for (win = o->win; win; win = p->win) {
                fx = 0;
                fy = 0;
                if ((win->flags & 4) && !o->unk6e) {
                    frame = d2_GetPic(win->frame);
                    part = d2_GetFrame(frame, 4);
                    fy = part->h;
                    part = d2_GetFrame(frame, 6);
                    fx = part->w;
                }
                p = &d2_Objs[win->parent];
                if (x0 <= (int)(p->clipx0 + fx))
                    x0 = p->clipx0 + fx;
                if (x1 >= (int)(p->clipx1 - fx))
                    x1 = p->clipx1 - fx;
                if (y0 <= (int)(p->clipy0 + fy))
                    y0 = p->clipy0 + fy;
                if (y1 >= (int)(p->clipy1 - fy))
                    y1 = p->clipy1 - fy;
            }
            if (d2_PendingCaptionObj && (o->win != d2_PendingCaptionWin || !d2_PendingCaptionObj->win)) {
                d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
                d2_DrawObjCaption(d2_PendingCaptionObj);
                d2_PendingCaptionWin = o->win;
                d2_PendingCaptionObj = 0;
            }
            d2_SetClipRect(x0, y0, x1, y1);
            switch (o->type) {
            case 1:
                if (o->icon != -1) {
                    frame = d2_GetPic(o->icon);
                    d2_DrawShape(o->x + o->iconx, o->y + o->icony, d2_ScreenSurf, frame, (char)d2_PicShift);
                }
                if (o->style & 2) {
                    if ((&d2_Objs[d2_PointerObj] == o || o->checked) && !o->clickDisabled) {
                        if (o->style & 1)
                            o->highlighted = 1;
                    } else if (o->style & 1)
                        o->highlighted = 0;
                }
                if (o->picScale != 0 && o->picScale != 1 && o->picScale != -1) {
                    if (o->highlighted)
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, 0, (char)o->picScale, 1);
                    else
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, 0, (char)o->picScale, 0);
                } else {
                    if (o->textShadow)
                        d2_DrawShapeMode2(x + o->shadowX, y + o->shadowY, d2_ScreenSurf, o->data, 0);
                    if (!o->unk58)
                        d2_DrawShape(x, y, d2_ScreenSurf, o->data, 0);
                    if (o->highlighted)
                        d2_DrawShapeMode3(x, y, d2_ScreenSurf, o->data, 0);
                    else if (o->unk58)
                        d2_DrawShapeHalfDark(x, y, d2_ScreenSurf, o->data, 0);
                    else if (o->greyed)
                        d2_DrawShapeMode2(x, y, d2_ScreenSurf, o->data, 0);
                }
                if (!o->win)
                    d2_AddDirty(x, y, d2_ShpW, d2_ShpH);
                break;
            case 0x11:
                if (o->drawFrames > 0) {
                    ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, d2_ScreenSurf, o->data);
                    if (o->drawFrames == 2) {
                        if (d2_NoBackgroundRestore == 1)
                            break;
                        ((void (*)(int, int, void *, void *))d3io_NoopCallback)(x, y, d2_BackSurf, o->data);
                    }
                    if (d2_NoBackgroundRestore != 1)
                        o->drawFrames--;
                }
                break;
            case 4:
                d2_DrawAnim(o->input, d2_ScreenSurf);
                if (!o->win) {
                    if (d2_AnimView[o->input].unk00)
                        d2_AddDirty(x, y, o->w + 8, o->h + 8);
                    else
                        d2_AddDirty(x, y, o->w, o->h);
                }
                break;
            case 5:
            case 8:
                if (o->icon != -1) {
                    frame = d2_GetPic(o->icon);
                    d2_DrawShape(o->x + o->iconx, o->y + o->icony, d2_ScreenSurf, frame, (char)d2_PicShift);
                }
                if (&d2_Objs[d2_PointerObj] != o && !o->checked) {
                    if (o->style & 1)
                        o->highlighted = 0;
                } else if (o->style & 1)
                    o->highlighted = 1;
                if (o->picScale != 0 && o->picScale != 1 && o->picScale != -1) {
                    if (o->highlighted)
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, (char)o->input, (char)o->picScale, 1);
                    else
                        d2_DrawShapeScaled(x, y, d2_ScreenSurf, o->data, (char)o->input, (char)o->picScale, 0);
                } else {
                    if (o->textShadow)
                        d2_DrawShapeMode2(x + o->shadowX, y + o->shadowY, d2_ScreenSurf, o->data, (char)o->input);
                    if (!o->unk58)
                        d2_DrawShape(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    if (o->highlighted) {
                        if (o->style & 0x80)
                            d2_DrawShape(x, y, d2_ScreenSurf, o->data, (char)(o->input + 1));
                        else
                            d2_DrawShapeMode3(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    } else if (o->unk58)
                        d2_DrawShapeHalfDark(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    else if (o->greyed) {
                        if (o->fillFraction == 0.0f)
                            d2_DrawShapeMode2(x, y, d2_ScreenSurf, o->data, (char)o->input);
                    }
                    if (o->fillFraction != 0.0f) {
                        pic = o->data;
                        yy = (int)((1.0f - o->fillFraction) * o->h + y);
                        if (y1 < yy)
                            yy = y1;
                        d2_SetClipRect(x0, y0, x1, yy);
                        d2_DrawShapeMode2(x, y, d2_ScreenSurf, pic, (char)o->input);
                        d2_SetClipRect(x0, y0, x1, y1);
                        if (d2_PointerObj != o->id) {
                            sprintf(buf, "%i%%", (int)((o->fillFraction > 1.0 ? 1.0 : o->fillFraction) * 100.0));
                            d2_SetFont(d2_DefaultFont + 1);
                            d2_DrawTextBox(o->x, o->y + o->h - 15, pic->w, o->h, d2_ScreenSurf, buf, 0x28);
                        }
                    }
                }
                if (o->u.text[0]) {
                    if (o->font)
                        d2_SetFont(o->font);
                    if (p)
                        d2_SetClipRect(x0, y0, x1, y1);
                    else
                        d2_SetClipRect(0, 0, 800, 600);
                    mode = 0;
                    if (o->highlighted)
                        mode = 2;
                    else if (o->greyed)
                        mode = 1;
                    if (o->centred)
                        mode |= 8;
                    if (o->rightAligned)
                        mode |= 0x10;
                    d2_GetObjPicSize(o->id, &tw, &th);
                    tx = o->captionX != -1 ? o->x + o->captionX : o->x + tw + 6;
                    ty = o->captionY != -1 ? o->y + o->captionY : o->y - 1;
                    d2_DrawTextBox(tx, ty, o->w, o->h, d2_ScreenSurf, o->u.text, mode);
                }
                if (o->caption[0] && &d2_Objs[d2_PointerObj] == o && !d2_FocusedInput && !(o->style & 0x40)) {
                    d2_PendingCaptionObj = o;
                    d2_PendingCaptionWin = o->win;
                }
                if (!o->win) {
                    if (o->textShadow)
                        d2_AddDirty(x, y, d2_ShpW + 8, d2_ShpH + 8);
                    else
                        d2_AddDirty(x, y, d2_ShpW, d2_ShpH);
                }
                break;
            case 0x40:
                if (o->drawFrames > 0) {
                    d2_SetSurfaceClip(x0, y0, x1 - x0, y1 - y0, d2_ScreenSurf);
                    d2_DrawWindow(o->input, d2_ScreenSurf);
                    if ((d2_Wins[o->input].flags & 0x1000) && d2_Wins[o->input].unk268 == 1)
                        d2_DrawWindow(o->input, d2_BackSurf);
                    if (!o->win) {
                        if (d2_Wins[o->input].flags & 0x8000)
                            d2_AddDirty(o->x - 2, o->y - 2, o->w + 4, o->h + 4);
                        else
                            d2_AddDirty(o->x, o->y, o->w, o->h);
                    }
                    d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, d2_ScreenSurf);
                }
                break;
            case 0x41:
                d2_DrawInput(o->input, d2_ScreenSurf);
                break;
            case 0x43:
                if (o->drawFrames > 0 && !o->hidden) {
                    if (o->style & 2) {
                        if ((&d2_Objs[d2_PointerObj] == o || o->checked)) {
                            if (o->style & 1)
                                o->highlighted = 1;
                        } else if (o->style & 1)
                            o->highlighted = 0;
                    }
                    if (o->drawFrames == 2)
                        o->drawFrames = 1;
                    font = d2_CurrentFont;
                    d2_SetFont(o->font);
                    mode = 0;
                    if (o->highlighted)
                        mode = 2;
                    else if (o->greyed)
                        mode = 1;
                    if (o->centred)
                        mode |= 8;
                    if (o->rightAligned)
                        mode |= 0x10;
                    if (o->textShadow)
                        mode |= 4;
                    if (o->u.text[0]) {
                        d2_SetTextColour(o->u.text[1], o->u.text[2], o->u.text[3]);
                        d2_DrawTextBox(o->x, o->y, o->w, o->h, d2_ScreenSurf, (char *)o->input, mode | 0x40);
                    } else
                        d2_DrawTextBox(o->x, o->y, o->w, o->h, d2_ScreenSurf, (char *)o->input, mode);
                    o->h = (short)d2_TextDrawnH;
                    if (!o->win)
                        d2_AddDirty(o->x, o->y, o->w, o->h);
                    d2_SetFont(font);
                }
                break;
            case 0x44:
                d2_DissolveStep(o->data);
                break;
            case 0x45:
                if (o->win)
                    d2_SetSurfaceClip(o->clipx0, o->clipy0, o->win->w, o->win->h, d2_ScreenSurf);
                d2_DrawSlider(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y - 4, o->w, o->h + 8);
                d2_SetSurfaceClip(0, 0, d2_ScreenW, d2_ScreenH, d2_ScreenSurf);
                break;
            case 9:
                d2_DrawButton(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y, o->w, o->h);
                break;
            case 0x47:
                o->clipy0 = (short)y0;
                o->clipy1 = (short)y1;
                d2_DrawRaw(o->id, d2_ScreenSurf);
                if (!o->win)
                    d2_AddDirty(o->x, o->y, o->w, o->h);
                break;
            }
        }
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    }
    if (d2_PendingCaptionObj) {
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
        d2_DrawObjCaption(d2_PendingCaptionObj);
        d2_PendingCaptionWin = 0;
        d2_PendingCaptionObj = 0;
    }
    if (d2_FocusedInput && (d2_FocusedInput->flags & 0x80) && d2_FocusedInput->value != -1 && d2_FocusedInput->maxval > d2_FocusedInput->minval
        && dm_L.lhold)
        d2_DrawFrame(d2_FocusedInput, d2_ScreenSurf);
    d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = 0x18;
    d2_FrameCounter++;
}
