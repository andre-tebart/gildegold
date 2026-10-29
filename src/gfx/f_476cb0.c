/* 0x476cb0, draws the pointer at x, y on surf: the pointer picture (or the pointer sprite d2_PointerSprite when
   one is set, moved to x, y) and the item list drawn next to it, amounts under the pictures; locks the surface
   while it draws and unlocks it again unless the caller held it locked; no effect during a cutscene */

void d2_DrawPointer(void *surf, int x, int y, int a, int b)
{
    int n, px, py, saved, hold, i;
    int c0, c1, c2, c3;
    void *pic;
    int amt, id;
    D2FRCT *f;
    char buf[0x40];
    char *s, *d;

    if (d2_CutsceneActive == 0 && ((hold = ((D2SURF *)surf)->locked) != 0 || d2_LockSurface(surf) != 0)) {
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        d2_SetClipRect(0, 0, ((D2SURF *)surf)->w, ((D2SURF *)surf)->h);
        if (d2_PointerSprite == -1) {
            d2_DrawShape(x, y, surf, d2_PointerPic, (unsigned char)d2_PointerLook);
        } else {
            d2_AnimView[d2_PointerSprite].x = (short)x;
            d2_AnimView[d2_PointerSprite].y = (short)y;
            d2_DrawAnim(d2_PointerSprite, surf);
        }
        saved = d2_CurrentFont;
        d2_SetFont(d2_DefaultFont + 1);
        n = 0;
        for (i = 0; i < 6; i++) {
            f = &d2_PointerItems[i];
            if (f->id != -1) {
                px = x + d2_Pics[d2_PointerPicIndex].w + (n % 3) * 0x1a;
                py = (d2_FontHeight + 0x1a) * (n / 3) + y;
                pic = d2_GetPic(f->id);
                if (d2_DrawShapeScaled(px, py, surf, pic, d2_PicShift, -2, 0)) {
                    amt = f->amount;
                    id = f->id;
                    if (id >= 0xdf && id < 0x3bb) {
                        if (amt != -1) {
                            sprintf(buf, "%i", amt);
                        } else {
                            s = txt_Ptrs[0xd7];
                            d = buf;
                            while ((*d++ = *s++) != 0)
                                ;
                        }
                    } else {
                        sprintf(buf, "%i", amt);
                    }
                    d2_DrawTextBox(px - 8, py + 0x18, 0x28, d2_FontHeight, surf, buf, 8);
                }
                n++;
            }
        }
        d2_AddDirty(x, y, 0x80, 0x80);
        d2_SetFont(saved);
        d2_SetClipRect(c0, c1, c2, c3);
        if (hold == 0)
            d2_UnlockSurface(surf);
    }
}
