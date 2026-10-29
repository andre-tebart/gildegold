/* 0x46fba0, lays out and draws the text of window win onto surf: words are wrapped (at '~' and '-' too) and
   collected in d2_TextSegs, d2_DrawTextLine draws each line. Commands start with '%' or '$' and an optional
   digit n: A (new line, n lines high), N / Y (paragraph, half line), < > (right / left margin n "M"s),
   T (tab), B Z R L (spread, centered, right, left), F (font n, FF: color n), C (clear), M, [ ], i (picture
   or button), l, m / mt (inputs), t (text of an input); the arguments of i, l, m, t come from w->args
   @deep far layout draft: 1865 vs 1890 instructions, 565 differing asm lines, hunks code 603 / slots 198 / order 22 / regs 14. Same statements, another allocation and schedule: the prologue spills left/right/top/bottom to [ebp-0x8]/[-0x14]/[-0x34]/[-0x68] where ours uses -0x10/-0x14/-0x38/-0x64, the original repeats the four stores inside the style&4 arm, the command switch and the glyph loop run in another order. Read the head (margins + style&4), the command cases (A N Y C m i l t, the F/color path), the object creation, the wrap loop and the tail: every statement corresponds, no wrong type/signedness/offset/width found. spell.py leads: reload x6 (46fdb2 46fdfe 46fe4a 46fe6f 46fec8 46feed - the original re-reads id [ebp-0x8] and align [ebp-0x28] from their slots; all our uses are already the bare local, both are call results / command state, so there is no expression to re-read and no parameter to use instead), dup-tail 46fc3b (the four local stores the original repeats in both paths - the source already writes them once before the branch and once in the arm), dup-tail 46ff06 (6 instructions), slots swaps 46fe8d / 470098. Q2 left: D2WIN.unk30 (0x30) is written once by the C (clear) command (d2.c:3029) and read nowhere in src/ or include/ - no evidence for a role (name gap). Named this session: d2_TextLayoutX/Y, d2_TextSegFlags, d2_MiscPic, d2_ResizeWindow, d2_ReplaceInputText. */

void d2_DrawWindowText(void *surf, int win)
{
    char tmp1[256];
    char tmp2[256];
    char errbuf[256];
    char buf128[128];
    char word[256];
    int c3, c2, c1, c0;
    int bottom, hasinputs, maxw;
    int pctflag, len;
    D2WIN *w;
    int top;
    D2RGB col;
    int width, extra, num, nlines, align, right, i, argi, left, pos;
    int arg, id, h, xx, yy, k, t;
    short pw;
    char *text, *start, *end, *p, *q;
    D2OBJ *o;
    D2INPUT *in;

    argi = 0;
    extra = 0;
    hasinputs = 0;
    nlines = 0;
    align = 0;
    pctflag = 0;
    w = &d2_Wins[win];
    if (w->font)
        d2_SetFont(w->font);
    else
        d2_SetFont(d2_FindPic("_FONT"));
    d2_TextSegFlags = 0;
    if (w->text) {
        left = w->ml + w->x;
        right = w->w - w->mr + w->x;
        top = w->mt + w->y;
        bottom = w->h - w->mb + top;
        if (w->flags & 4) {
            left += 3;
            right -= 3;
            top += 3;
            bottom -= 3;
        }
        maxw = w->w;
        if (w->flags & 8)
            top = w->y + 0xc;
        c0 = d2_ClipLeft;
        c1 = d2_ClipTop;
        c2 = d2_ClipRight;
        c3 = d2_ClipBottom;
        d2_TextLayoutY = top - w->scrollY;
        d2_TextLayoutX = left;
        d2_FontHeight = d2_FontPic->h;
        d2_TextSegCount = 0;
        pos = 0;
        len = strlen(w->text);
        {
            for (; pos < len;) {
                if (!w->text[pos])
                    break;
                d2_TextSegs[d2_TextSegCount].flags = 0;
                while (w->text[pos] == '%' || w->text[pos] == '$') {
                    pos++;
                    if (isdigit(w->text[pos])) {
                        num = w->text[pos] - '0';
                        pos++;
                    } else
                        num = -1;
                    text = w->text;
                    switch (text[pos]) {
                    case 'A':
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        k = w->text[pos - 1] - '0';
                        if (k > 0 && k < 9)
                            h = d2_FontHeight * k;
                        else
                            h = d2_FontHeight;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextLayoutX = left;
                        d2_TextLayoutY += h;
                        d2_TextSegFlags = 0;
                        pos++;
                        nlines++;
                        break;
                    case 'N':
                        if (text[pos - 1] != '$')
                            break;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left;
                        h = d2_FontHeight * 3 / 2;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextSegFlags = 0;
                        d2_TextLayoutY += h;
                        pos++;
                        nlines++;
                        break;
                    case 'Y':
                        if (text[pos - 1] != '$')
                            break;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left;
                        h = d2_FontHeight / 2;
                        if (extra) {
                            h = extra;
                            extra = 0;
                        }
                        d2_TextLayoutY += h;
                        pos++;
                        nlines++;
                        break;
                    case 'i':
                        if (text[pos + 1] == 'a' || text[pos + 1] == 'n') {
                            hasinputs = 1;
                            if (w->args[argi] == -1) {
                                k = d2_FindPic("_BUTTON_RED");
                                id = d2_WinCreateSlider(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, k, (int)ch_EmptyName, win);
                                if (w->text[pos + 2] == '[') {
                                    char buf512[512];
                                    D3_ZERO(buf512, 0x200);
                                    start = w->text + pos + 3;
                                    q = strchr(start, ']');
                                    if (!q) {
                                        sprintf(errbuf, "Missing ']'");
                                        break;
                                    }
                                    strncpy(buf512, start, q - start);
                                    d2_SetCaption(id, buf512);
                                    if (!q[1])
                                        w->text[pos + 2] = 0;
                                    else
                                        memmove(w->text + pos + 2, q + 1, strlen(q + 1) + 2);
                                }
                                if (w->text[pos + 1] == 'n')
                                    d2_Objs[id].style |= 0x10;
                                w->args[argi] = -1;
                                d2_TextLayoutX += d2_Objs[id].w;
                                w->args[argi] = id;
                                h = d2_Objs[id].h;
                            } else
                                h = d2_Objs[w->args[argi]].h;
                            extra = h + 7;
                        } else {
                            arg = w->args[argi];
                            if (arg != -1) {
                                yy = w->scrollY + d2_TextLayoutY;
                                t = d2_TextLayoutX;
                                if (arg == 0 || arg == d2_MiscPic || arg == d2_MiscPic + 1)
                                    yy += 2;
                                pw = d2_Pics[arg].w;
                                if (pw + d2_TextLayoutX > right) {
                                    xx = left;
                                    yy += d2_FontHeight;
                                } else
                                    xx = t;
                                if (pw > w->w) {
                                    pos++;
                                    break;
                                }
                                if (d2_TextLayoutX > left)
                                    xx += 6;
                                id = d2_WinOpenPic(xx - w->x, yy - w->y, arg, win);
                                if (id != -1) {
                                    if (w->text[pos + 1] == 'c') {
                                        d2_Objs[id].interactive = 0;
                                        d2_Objs[id].checkable = 1;
                                    } else if (w->text[pos + 1] == 'i') {
                                        d2_Objs[id].interactive = 0;
                                        d2_Objs[id].checkable = 0;
                                    } else if (w->text[pos + 1] == 'b') {
                                        d2_Objs[id].interactive = 1;
                                        d2_Objs[id].checkable = 0;
                                    } else if ((w->text[pos + 1] == 't' || w->text[pos + 1] == 'a' || w->text[pos + 1] == 'n')
                                               && w->text[pos + 2] == '[') {
                                        D3_ZERO(buf128, 0x80);
                                        d2_Objs[id].interactive = 1;
                                        d2_Objs[id].font = w->font;
                                        d2_Objs[id].style = 3;
                                        start = w->text + pos + 3;
                                        q = strchr(start, ']');
                                        if (!q) {
                                            sprintf(errbuf, "Missing ']'");
                                            break;
                                        }
                                        strncpy(buf128, start, q - start);
                                        d2_SetCaption(id, buf128);
                                        if (!q[1])
                                            w->text[pos + 2] = 0;
                                        else
                                            memmove(w->text + pos + 2, q + 1, strlen(q + 1) + 2);
                                    }
                                }
                                w->args[argi] = -1;
                                d2_TextLayoutX += d2_Objs[id].w;
                            }
                        }
                        pos += 2;
                        argi++;
                        break;
                    case 'C':
                        memmove(text, text + pos + 1, strlen(text + pos + 1) + 1);
                        d2_TextLayoutY = top - w->scrollY;
                        d2_TextLayoutX = left;
                        pos = 0;
                        w->unk30 = 0;
                        break;
                    case 'm':
                        if (text[pos + 1] != 't') {
                            pos++;
                            if (w->args[argi] == -1) {
                                xx = d2_TextLayoutX;
                                if (xx > left)
                                    xx += 6;
                                id = d2_WinOpenInput(xx - w->x, d2_TextLayoutY - w->y, 0x10, d2_FontHeight, 0x82, win);
                                k = d2_Objs[id].input;
                                w->args[argi] = k;
                                d2_TextLayoutX = d2_TextLayoutX + 6 + d2_Inputs[k].w;
                            }
                            argi++;
                        } else {
                            pos += 2;
                            if (w->args[argi] != -1) {
                                id = d2_WinOpenInput(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, 8, d2_FontHeight, 0x10, win);
                                d2_Objs[id].hidden = 1;
                                k = d2_Objs[id].input;
                                d2_Inputs[k].firstTextId = w->args[argi];
                                d2_Inputs[k].textCount = num;
                                d2_Inputs[k].currentTextId = w->args[argi + 1];
                                d2_Inputs[k].buf = w->text + pos;
                                d2_InputSetString(k, w->args[argi + 1]);
                                w->args[argi] = -1;
                                w->args[argi + 1] = k;
                                len = strlen(w->text);
                            }
                            d2_Inputs[w->args[argi + 1]].buf = w->text + pos;
                            d2_MoveObj((short)d2_TextLayoutX, (short)d2_TextLayoutY,
                                                     d2_Inputs[w->args[argi + 1]].obj);
                            argi += 2;
                        }
                        break;
                    case 'l':
                        pos++;
                        if (w->args[argi] != -1) {
                            id = d2_CreateWinCaption(d2_TextLayoutX - w->x, d2_TextLayoutY - w->y, (char *)w->args[argi], win);
                            d2_Objs[id].hidden = 1;
                            d2_Objs[id].u.raw = (unsigned short *)(w->text + pos);
                            d2_ReplaceInputText(id, (char *)w->args[argi]);
                            w->args[argi] = -1;
                            len = strlen(w->text);
                        }
                        argi++;
                        break;
                    case 'M':
                        memmove(text + pos - 1, text + pos + 1, strlen(text) - pos + 1);
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$M");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        break;
                    case 't':
                        if (w->args[argi] != -1
                            && ((text[pos - 1] == '%' && num == -1) || (text[pos - 2] == '%' && num != -1))) {
                            xx = d2_TextLayoutX;
                            if (xx > left)
                                xx += 6;
                            id = d2_WinOpenInput(xx - w->x, d2_TextLayoutY - w->y, 0, d2_FontHeight, 1, win);
                            in = &d2_Inputs[d2_Objs[id].input];
                            arg = w->args[argi];
                            if (arg > 0x4800)
                                strcpy(in->text, (char *)arg);
                            else
                                strcpy(in->text, txt_Ptrs[arg]);
                            in->w = d2_TextWidth(in->text, in->font);
                            d2_Objs[in->obj].w = in->w;
                            w->args[argi] = -1;
                        }
                        pos++;
                        argi++;
                        break;
                    case 'T':
                        if (num == -1)
                            num = 1;
                        xx = left + d2_TextWidth("M", w->font) * num * 3;
                        if (xx > d2_TextLayoutX) {
                            d2_TextLayoutX = xx;
                            if (xx > right) {
                                d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                                d2_TextLayoutY += d2_FontHeight;
                                d2_TextLayoutX = left;
                            }
                            strcpy(d2_TextSegs[d2_TextSegCount].text, "$T");
                            d2_TextSegs[d2_TextSegCount].w = xx;
                            d2_TextSegCount++;
                        }
                        pos++;
                        break;
                    case '>':
                        if (num == -1)
                            num = 0;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        d2_TextLayoutX = left = w->x + d2_TextWidth("M", w->font) * num * 2 + w->ml;
                        pos++;
                        break;
                    case '<':
                        if (num == -1)
                            num = 0;
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        right = w->x - w->mr - d2_TextWidth("M", w->font) * num * 2 + w->w;
                        pos++;
                        break;
                    case '[':
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$[");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        pos++;
                        break;
                    case ']':
                        strcpy(d2_TextSegs[d2_TextSegCount].text, "$]");
                        d2_TextSegs[d2_TextSegCount++].w = 0;
                        pos++;
                        break;
                    case 'Z':
                        align = 0x100;
                        pos++;
                        break;
                    case 'R':
                        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
                        align = 0x40;
                        pos++;
                        break;
                    case 'B':
                        align = 0x80;
                        pos++;
                        break;
                    case 'L':
                        align = 0;
                        d2_TextLayoutX = left;
                        pos++;
                        break;
                    case 'F':
                        if (text[pos + 1] != 'F') {
                            d2_SetFont(d2_DefaultFont + num);
                            pos++;
                        } else {
                            pos++;
                            {
                            unsigned char tab[27] = {
                                0xdc, 0, 0,  0, 0xdc, 0,  0, 0, 0xdc,  0xff, 0xff, 0,  0xff, 0, 0xa8,
                                0xff, 0xff, 0xff,  0xa8, 0xa8, 0xa8,  0, 0xff, 0xc4,  0x96, 3, 3
                            };
                            if (num == 9) {
                                d2_TextSegFlags &= ~0x40;
                            } else {
                                d2_TextSegFlags |= 0x40;
                                col.r = tab[num * 3];
                                col.g = tab[num * 3 + 1];
                                col.b = tab[num * 3 + 2];
                            }
                            }
                            pos++;
                        }
                        break;
                    case '$':
                    case '%':
                        pctflag = 1;
                        break;
                    default:
                        sprintf(errbuf, "Unknown textparameter: %%%c", text[pos - 1]);
                        break;
                    }
                }
                if (w->text[pos]) {
                    int wl;
                    while (w->text[pos] == ' ')
                        pos++;
                    start = w->text + pos;
                    end = strchr(start, ' ');
                    if (!end)
                        end = w->text + len;
                    if (!pctflag && (p = strchr(start, '%')) != 0 && p <= end && p[1] != '%')
                        end = p;
                    pctflag = 0;
                    p = strchr(start, '$');
                    if (p && p < end)
                        end = p;
                    wl = end - start;
                    if (wl) {
                        memset(word, 0, sizeof(word));
                        if (start && wl >= 0) {
                            memcpy(word, start, wl);
                            word[wl + 1] = 0;
                        }
                        width = d2_TextWidth(word, w->font);
                        if (width + d2_TextLayoutX > right) {
                            if (w->flags & 0x2000) {
                                if (width + d2_TextLayoutX < d2_ScreenW)
                                    d2_ResizeWindow(w->w + width, w->h, win);
                                else {
                                    d2_ResizeWindow(d2_ScreenW - w->x, w->h, win);
                                    d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                    d2_TextLayoutY += d2_FontHeight;
                                    d2_TextLayoutX = left;
                                    w->lines[w->firstline + nlines] = pos;
                                    nlines++;
                                }
                            } else {
                                t = 0;
                                id = strlen(word);
                                strcpy(tmp1, word);
                                strcpy(tmp2, word);
                                for (; id >= 0; id--) {
                                    if (word[id] == '~' || word[id] == '-') {
                                        strcpy(tmp1, &word[id]);
                                        t = 0;
                                        if (word[id] == '-')
                                            t = 1;
                                        word[id] = '-';
                                        word[id + 1] = 0;
                                        if (d2_TextWidth(word, w->font) + d2_TextLayoutX <= right)
                                            break;
                                    }
                                }
                                if (id > 0) {
                                    id += t;
                                    wl = strlen(word);
                                    memcpy(d2_TextSegs[d2_TextSegCount].text, word, wl + 1);
                                    d2_TextSegs[d2_TextSegCount++].w = d2_TextWidth(word, w->font);
                                    pos = pos + t + wl - 1;
                                } else
                                    strcpy(word, tmp2);
                                d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                d2_TextLayoutY += d2_FontHeight;
                                d2_TextLayoutX = left;
                                w->lines[w->firstline + nlines] = pos;
                                nlines++;
                                if (id > 0) {
                                    strcpy(word, &tmp2[id]);
                                    wl = strlen(word);
                                    width = d2_TextWidth(word, w->font);
                                }
                            }
                        }
                        w->nlines = w->firstline + nlines;
                        for (t = ((int)(char)w->flags & 8U) >> 3; t < w->nobjs; t++) {
                            o = &d2_Objs[w->objs[t]];
                            while (!o->hidden && width + d2_TextLayoutX > o->x) {
                                id = o->x + o->w;
                                if (d2_TextLayoutX >= id || d2_FontHeight + d2_TextLayoutY <= o->y
                                    || d2_TextLayoutY >= o->h + o->y)
                                    break;
                                d2_TextLayoutX = id + 6;
                                if (width + d2_TextLayoutX > right) {
                                    d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 0, align, left, right);
                                    d2_TextLayoutX = left;
                                    d2_TextLayoutY += d2_FontHeight;
                                    w->lines[w->firstline + nlines] = pos;
                                    nlines++;
                                }
                            }
                        }
                        xx = d2_SpaceWidth - w->x + width + d2_TextLayoutX;
                        if (xx > maxw)
                            maxw = xx;
                        if (wl)
                            d2_TextLayoutX += d2_SpaceWidth + width;
                        memcpy(d2_TextSegs[d2_TextSegCount].text, word, wl + 1);
                        d2_TextSegs[d2_TextSegCount].w = width;
                        d2_TextSegs[d2_TextSegCount].flags = d2_TextSegFlags;
                        d2_TextSegs[d2_TextSegCount].col = col;
                        d2_TextSegCount++;
                        pos += wl;
                        if (d2_TextSegCount > 0x20)
                            d2_TextSegCount = 0;
                    }
                }
            }
        }
        d2_DrawTextLine(d2_TextLayoutX, d2_TextLayoutY, surf, win, 1, align, left, right);
        if (w->flags & 0x4000) {
            if (d2_FontHeight + d2_TextLayoutY - 8 > d2_ScreenH) {
                D2WIN *pw2 = d2_Objs[w->parent].win;
                if (pw2)
                    d2_MoveObj(pw2->x, (short)(pw2->y - d2_FontHeight - d2_TextLayoutY + d2_ScreenH + 8),
                                             pw2->parent);
                else
                    d2_MoveObj(w->x, (short)(d2_ScreenH - d2_FontHeight - d2_TextLayoutY + w->y - 8),
                                             w->parent);
            }
            d2_ResizeWindow(w->w, d2_FontHeight - w->y + d2_TextLayoutY, win);
        }
        d2_TextLayoutY += d2_FontHeight;
        if (w->flags & 0x2000)
            d2_ResizeWindow(maxw, w->h, win);
        if (d2_TextLayoutY - w->y > w->contentHeight)
            w->contentHeight = d2_TextLayoutY - w->y;
        d2_SetClipRect(c0, c1, c2, c3);
        if (w->flags & 0x20) {
            yy = (bottom - top - 0x10) / w->contentHeight + top;
            if (!d2_LayoutMode)
                d2_DrawShape(right + 1, yy, surf, d2_GetPic(d2_ScrollMarkerPic), 5);
        }
        if (w->sizedObjCount != w->nobjs && hasinputs) {
            d2_LayoutButtons(win);
            w->sizedObjCount = w->nobjs;
        }
    }
}
