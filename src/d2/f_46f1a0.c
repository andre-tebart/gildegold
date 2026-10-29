/* 0x46f1a0, draws text word-wrapped into the box x, y, w, h (flags 8: centered, 0x10: right aligned, 4 / 2 / 1:
   shadow and extra passes, 0x40: colored, 0x20 / 0x80: background sprite, 0x80 keeps the box on the screen,
   0x100: box is given); with surf 0 it only measures. Leaves left edge, top and size in d2_TextDrawnW..74
   @deep far layout draft: 528 vs 522 instructions, 584 differing asm lines. Same statements, another allocation: original frame 0x638, lines[] at [esp+0x48], x/y home slots [esp+0x64c]/[esp+0x650], ebx=y esi=x ebp=0; ours 0x630, lines[] at [esp+0x40], x kept in a register, ebx=x ebp=y esi=0, plus extra spills (mov [esp+0x10],[esp+0x24],[esp+0x28]) and the whole wrap loop scheduled in another order. Read against the original disassembly: prologue (0x46f1a0-46f23d), measure/clip block (0x46f240-46f3c0), sprite blit, word split with the goto fits case, glyph loop and tail - every statement corresponds, no wrong type/signedness/offset/width found. spell.py: rep-stos already satisfied (both sides rep stos 0x180 after lea of lines[]), dup-tail at 46f82b is the shared epilogue (add esp/pops, no source form), cmp-test lead tried: if (ln > 1) -> if (ln >= 2) gives the original cmp reg,2; jl (kept, 586 -> 584). Remaining: VC6 locals layout and spill/register choice (vargen/listing), no meaning-class lead left. Readable Q1..Q7. */

void d2_DrawTextBox(int x, int y, int w, int h, void *surf, char *text, unsigned int flags)
{
    char lines[16][0x60];
    int (*draw)(int, int, void *, void *, unsigned char);
    D2FRAME *g;
    char *sp, *p, *q, *word;
    int right, lw, len, pos, ln, wd, x0, y0, bx, bw, bh, vis, sw, top, yy, k, wl, n, j, i, cx;
    unsigned char c;
    char ch;

    lw = 0;
    x0 = x;
    y0 = y;
    d2_TextDrawnW = 0;
    d2_TextDrawnH = 0;
    d2_TextLeft = 9999;
    d2_TextTop = y;
    if (d2_FontPic == 0)
        d2_FontPic = d2_GetPic(d2_FindPic("_FONT"));
    d2_FontHeight = d2_FontPic->h;
    memset(lines, 0, sizeof(lines));
    if (surf != 0 && (flags & 0xa0)) {
        if (!(flags & 0x100)) {
            d2_DrawTextBox(x, y, w, h, 0, text, flags);
            bx = d2_TextLeft;
            bh = d2_TextDrawnH;
            y = d2_TextTop;
            bw = d2_TextDrawnW - d2_TextLeft + x;
        } else {
            bx = x;
            bh = h;
            bw = w;
        }
        if ((char)flags < 0) {
            sw = d2_ScreenW;
            if (bx + bw > sw) {
                bx = sw - bw;
                d2_TextLeft = sw - d2_TextDrawnW;
                x = d2_TextLeft;
                x0 = x;
            }
            if (d2_ScreenH < y + bh) {
                y = d2_ScreenH - bh;
                d2_TextTop = y;
                y0 = y;
            }
        }
        vis = 1;
        top = d2_ClipTop;
        yy = y;
        if (y + bh < top)
            vis = 0;
        if (y > d2_ClipBottom)
            vis = 0;
        if (y < top) {
            bh = top - y;
            yy = top;
        }
        if (yy + bh > d2_ClipBottom)
            bh = d2_ClipBottom - yy;
        if (vis) {
            d2_UnlockSurface(surf);
            d3if_DrawSprite(d2_CaptionTex, bw / 2 + bx, bh / 2 + yy, 0, (float)bw / (float)d2_CaptionTex->size,
                         (float)bh / (float)d2_CaptionTex->size, 0.7f);
            d2_LockSurface(surf);
        }
        d2_TextDrawnH = bh;
        d2_TextDrawnW = bw;
    }
    pos = 0;
    ln = 0;
    draw = d2_DrawShapeMode5;
    if (!(flags & 0x40))
        draw = d2_DrawShape;
    len = strlen(text);
    right = x + w;
    if (len > 0) {
        do {
            word = lines[ln];
            p = text + pos;
            q = word;
            do {
                ch = *p;
                *q++ = ch;
                p++;
            } while (ch);
            sp = strchr(word, ' ');
            if (sp)
                *sp = 0;
            else
                sp = strchr(word, 0);
            pos = pos + (sp - word) + 1;
            wd = d2_TextWidth(word, d2_CurrentFont);
            x += wd;
            lw += wd;
            if (pos <= len) {
                lw += d2_SpaceWidth;
                x += d2_SpaceWidth;
            }
            ln++;
            if (x >= right || pos >= len || sp[-1] == '~') {
                if (lw > w) {
                    word = lines[ln - 1];
                    wl = strlen(word);
                    for (k = wl; k >= 0; k--) {
                        ch = word[k];
                        if (ch == '~' || ch == '-') {
                            if (ch == '~')
                                word[k] = '-';
                            word[k + 1] = 0;
                            if (d2_TextWidth(word, d2_CurrentFont) + (lw - wd) <= w) {
                                if (k >= 0) {
                                    pos += k - wl;
                                    lw = 0;
                                    for (j = 0; j < ln; j++)
                                        lw += d2_TextWidth(lines[j], d2_CurrentFont) + d2_SpaceWidth;
                                    goto fits;
                                }
                                break;
                            }
                        }
                    }
                    if (ln >= 2) {
                        lw = 0;
                        pos += -1 - wl;
                        ln--;
                        for (j = 0; j < ln; j++)
                            lw += d2_TextWidth(lines[j], d2_CurrentFont) + d2_SpaceWidth;
                        goto fits;
                    }
                } else {
fits:
                    lw -= d2_SpaceWidth;
                }
                if ((flags & 8) && lw <= w)
                    cx = (w - lw) / 2 + x0;
                else {
                    cx = x0;
                    if (flags & 0x10)
                        cx = x0 - lw + w;
                }
                yy = y;
                if (d2_TextLeft >= cx)
                    d2_TextLeft = cx;
                for (n = ln; n > 0; n--) {
                    word = lines[ln - n];
                    for (i = 0; i < (int)strlen(word); i++) {
                        c = word[i];
                        if (c != '~') {
                            g = d2_GetFrame(d2_FontPic, c);
                            if (i != 0)
                                cx -= g->kern;
                            if (c != ' ') {
                                if (surf != 0) {
                                    if (flags & 4)
                                        d2_DrawShapeMode2(cx + 6, yy + 6, surf, d2_FontPic, c);
                                    draw(cx, yy, surf, d2_FontPic, c);
                                    if (flags & 2)
                                        d2_DrawShapeMode3(cx, yy, surf, d2_FontPic, c);
                                    else if (flags & 1)
                                        d2_DrawShapeMode2(cx, yy, surf, d2_FontPic, c);
                                }
                                cx += g->adv + d2_LetterGap;
                            }
                        }
                        if (cx > right + d2_LetterGap) {
                            yy += d2_FontHeight;
                            cx = x0;
                        }
                    }
                    if (d2_TextDrawnW < cx - x0)
                        d2_TextDrawnW = cx - x0;
                    cx += d2_SpaceWidth;
                }
                y += d2_FontHeight;
                lw = 0;
                x = x0;
                ln = 0;
            }
        } while (pos < len);
    }
    if (d2_TextDrawnH <= y - y0)
        d2_TextDrawnH = y - y0;
}
