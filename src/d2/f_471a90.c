/* 0x471a90, draws a window: its objects, scroll buttons, background and frame @deep near: 613 vs 613 instructions, 45/1872 bytes differ, 54 asm lines - only stack-slot permutations of the frame locals (the (int)((float)..+1.5f) casts use [esp+0x40] where the original has [esp+0x44] at 471d86/471de8/471e99/471ecf, and the scroll-button loop locals are permuted at 471e18/471e38/471eba/471f15/471fbc/[0x50]/[0x54]/[0x58]/[0x68]/[0x6c]) plus two moved add/sub pairs at 471fa7 and 471ff2 - i.e. declaration-order / spill allocation (vargen, listing). Tried: rewriting w->w + w->x - pw as w->w - pw + w->x at 471fa7/471ff2 - identical bytes, reverted; spell.py reload entries name the loop locals re-read at their uses (sw/sh/pw/ph/x are locals used as-is, nothing to drop - 471e38/471e4f/471eba/471f15/471fdb), iv-shift is the loop base suggestion only. Q2 clean (all fields named; the cornerObj / extraObj child objects are the 0x3bc / 0x3c0 pair). Named this session: d2_WinBorderPic / d2_WinBorderPergaPic (d2_FindPic of _WIN_BORDER / _WIN_BORDER_PERGA), D2OBJ.winIndex -> winIndex, d2_SetObjEnabled -> d2_SetObjEnabled. */

#define D2WOP(o) ((D2WIN *)((char *)d2_Wins + (o)))   /* window at byte offset o (win * 0x3c4); a pointer, so
                                                         the original's offset-in-a-register addressing stays */

void d2_DrawWindow(win, surf)
int win;
void *surf;
{
    int saved;
    D2WIN *w = &d2_Wins[win];
    D2WIN *w2;
    int i, ob, in, v, n, p, pw, ph, sw, sh, x;
    char buf[32];
    void *pic;

    if (w->active) {
        saved = d2_LayoutMode;
        i = 0;
        d2_LayoutMode = 1;
        if (w->nobjs > 0) {
            do {
                ob = w->objs[i];
                if (d2_Objs[ob].type == 0x40) {
                    in = d2_Objs[ob].input;
                    if (d2_Wins[in].flags & 0x4000)
                        d2_DrawWindowText(0, in);
                }
                i++;
            } while (i < w->nobjs);
        }
        if (w->flags & 0x4000)
            d2_DrawWindowText(0, win);
        v = w->visible;
        d2_LayoutMode = saved;
        if (v != 0) {
            if (v % 8 == 0)
                w->scrollStep = 8;
            else if (v % 6 == 0)
                w->scrollStep = 6;
            else if (v % 4 == 0)
                w->scrollStep = 4;
            else if (v % 2 == 0)
                w->scrollStep = 2;
        }
        if (w->btnup != -1 || w->btndown != -1) {
            ob = w->btndown;
            w2 = &d2_Wins[d2_Objs[ob].winIndex];
            if (ob != -1) {
                if (w2->contentHeight < w2->h + w2->visible)
                    d2_SetObjEnabled(ob, 0);
                else
                    d2_SetObjEnabled(ob, 1);
                if (w2->scrollRemY + w2->scrollY + w2->h < w2->contentHeight)
                    d2_SetObjEnabled(w->btndown, 1);
                else
                    d2_SetObjEnabled(w->btndown, 0);
            }
            if (w->btnup != -1) {
                if (w2->scrollY == 0)
                    d2_SetObjEnabled(w->btnup, 0);
                else
                    d2_SetObjEnabled(w->btnup, 1);
            }
            if (w->bar != -1) {
                _itoa((int)((float)w2->contentHeight / w2->visible + 0.5f), buf, 10);
                d2_SetObjText(w->bar, buf);
            }
        }
        d2_ScrollWindow(win);
        if (d2_LayoutMode == 0)
            d2_DrawWindowBackground(w, surf);
        if (w->flags & 0x10)
            d2_DrawWindowText(surf, win);
        d2_SetClipRect(w->x, w->y, w->w + w->x, w->h + w->y);
        if ((w->flags & 4) && d2_LayoutMode == 0) {
            pic = d2_GetPic(w->frame);
            p = w->frame;
            if (p == d2_WinBorderPic || p == d2_WinBorderPergaPic) {
                if (w->cornerObj == -1)
                    d2_DrawShape(w->x, w->y, surf, pic, 0);
                d2_DrawShape(w->w - 8 + w->x, w->y, surf, pic, 1);
                d2_DrawShape(w->x, w->h - 8 + w->y, surf, pic, 2);
                d2_DrawShape(w->w - 8 + w->x, w->h - 8 + w->y, surf, pic, 3);
                n = (w->w - 0x10) >> 3;
                for (i = 0; i < n; i++) {
                    d2_DrawShape(w->x + 8 + i * 8, w->y, surf, pic, 5);
                    d2_DrawShape(w->x + 8 + i * 8, w->h - 8 + w->y, surf, pic, 8);
                }
                n = (w->h - 0x10) >> 3;
                for (i = 0; i < n; i++) {
                    d2_DrawShape(w->x, w->y + 8 + i * 8, surf, pic, 0xb);
                    d2_DrawShape(w->w - 8 + w->x, w->y + 8 + i * 8, surf, pic, 0xd);
                }
                if (w->flags & 0x8000) {
                    d2_DrawShape(w->x - 2, w->y - 2, surf, pic, 0x10);
                    d2_DrawShape(w->w - 6 + w->x, w->y - 2, surf, pic, 0x10);
                    d2_DrawShape(w->x - 2, w->h - 6 + w->y, surf, pic, 0x10);
                    d2_DrawShape(w->w - 6 + w->x, w->h - 6 + w->y, surf, pic, 0x10);
                }
            } else {
                pw = d2_Pics[p].w;
                ph = d2_Pics[p].h;
                d2_SetClipRect(w->x, w->y, w->w - pw + w->x, w->h + w->y);
                sh = d2_Pics[w->frame + 4].h;
                sw = d2_Pics[w->frame + 4].w;
                n = (int)((float)((w->w - pw * 2) / sw) + 1.5f);
                if (n > 0) {
                    for (x = 0; n; n--, x += sw) {
                        d2_DrawShape(w->x + x + pw, w->y, surf, pic, 4);
                        d2_DrawShape(w->x + x + pw, w->h + w->y - sh, surf, pic, 5);
                    }
                }
                sw = d2_Pics[w->frame + 7].w;
                sh = d2_Pics[w->frame + 7].h;
                d2_SetClipRect(w->x, w->y, w->w + w->x, w->h - ph + w->y);
                n = (int)((float)((w->h - ph * 2) / sh) + 1.5f);
                if (n > 0) {
                    for (x = 0; n; n--, x += sh) {
                        d2_DrawShape(w->x, w->y + x + ph, surf, pic, 6);
                        d2_DrawShape(w->w + w->x - sw, w->y + x + ph, surf, pic, 7);
                    }
                }
                d2_SetClipRect(w->x, w->y, w->w + w->x, w->h + w->y);
                if (w->cornerObj == -1)
                    d2_DrawShape(w->x, w->y, surf, pic, 0);
                d2_DrawShape(w->w + w->x - pw, w->y, surf, pic, 1);
                d2_DrawShape(w->x, w->h + w->y - ph, surf, pic, 2);
                d2_DrawShape(w->w + w->x - pw, w->h + w->y - ph, surf, pic, 3);
            }
        }
        if (w->backgroundState)
            w->backgroundState--;
        d2_SetClipRect(0, 0, d2_ScreenW, d2_ScreenH);
    }
}
