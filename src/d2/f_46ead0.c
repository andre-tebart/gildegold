/* 0x46ead0, draws the pieces of text collected in d2_TextSegs in one line at y of window win, starting at x and
   ending at right: justified by the window's flags (0x80: spread, 0x40: right, 0x100: centered), pieces move
   right past the objects of the window; "$T" jumps to a position, "$H" draws with a second pass, "$[" "$]" draw
   glyph 0
   @deep draft (481 differing instructions of 441 vs 438; 1109/1344 bytes). Same statements, another register map: the original keeps the zero constant in edi (xor edi,edi at 46ead7, cmp/test against edi) where ours keeps layout values in ebp, and the whole 46ead7..46f1c0 stream is scheduled differently (the strlen scas loop at 46ee7b, the d2_TextSegCount comparisons at 46ee67 vs 46ee9d). spell.py: reload at 46ee15 (the original re-reads the char locals [esp+0x5c] where ours keeps them in ebp) - the lead asks to drop a local and re-read its expression, but the value is hl/skip set in the '$H'/'/'$M' switch arms, not a re-readable expression; dup-tail at 46ead7 (3 instructions) and 46ee63 (7) are the same calls/compares under the other registers, no repeated statement in the source. No meaning error read in the justification block (gap/extra/k), the object-width walk (start/n/pobj) or the glyph switch. Readability done this session (Q1..Q7: d2_DrawTextLine, d2_TextSegCount, d2_LayoutMode). Left: the register map and the scheduling (listing/c2prio/siblings). */

void d2_DrawTextLine(int a, int y, void *surf, int win, char nojustify, int flags, int x, int right)
{
    D2WIN *w;
    D2OBJ *o;
    D2FRAME *g;
    D2PICHDR *gfont;
    int (*draw)(int, int, void *, void *, unsigned char);
    int gap, extra, saved, total, start, glyph, i, j, k, n, savedflags;
    int *pobj;
    char hl, skip;
    unsigned char c;                /* a glyph byte: the original zero-extends it into d2_GetFrame's index
                                       (0x46ee33 movzx dx,dl / 0x46eef3 xor dx,dx; mov dl,al); a signed
                                       char sign-extended 0xfc to 0xfffc and read 256 KB past the font bank */

    total = 0;
    extra = 0;
    hl = 0;
    gap = d2_SpaceWidth;
    w = &d2_Wins[win];
    savedflags = w->flags;
    if (flags != 0)
        w->flags = flags;
    if (d2_LayoutMode != 1) {
        for (i = 0; i < d2_TextSegCount; i++)
            total += d2_TextSegs[i].w;
        saved = w->flags;
        start = ((int)(char)saved & 8) >> 3;
        if (start < w->nobjs) {
            n = w->nobjs - start;
            pobj = &w->objs[start];
            do {
                o = &d2_Objs[*pobj];
                if (o->hidden == 0 && o->y < d2_FontHeight + y && y < o->y + o->h && flags != 0x40)
                    total += 6 + o->w + d2_LetterGap;
                pobj++;
                n--;
            } while (n);
        }
        if ((char)saved < 0 && d2_TextSegCount > 1 && nojustify == 0) {
            k = right - total - x;
            gap = k / (d2_TextSegCount - 1);
            extra = k % (d2_TextSegCount - 1);
        } else if (!(saved & 0x40)) {
            if (saved & 0x100) {
                k = right - (d2_TextSegCount - 1) * d2_SpaceWidth - total - x;
                x = x + k / 2;
            }
        } else
            x = right - (d2_TextSegCount - 1) * d2_SpaceWidth - total;
        for (j = 0; j < d2_TextSegCount; j++) {
            glyph = -1;
            skip = 0;
            start = ((int)(char)w->flags & 8) >> 3;
            if (d2_TextSegs[j].text[0] == '$') {
                switch (d2_TextSegs[j].text[1]) {
                case 'H':
                    hl = 1;
                case 'M':
                    skip = 1;
                    break;
                case 'T':
                    x = d2_TextSegs[j].w;
                    skip = 1;
                    break;
                case '[':
                case ']':
                    gfont = d2_FontPic;
                    glyph = 0;
                }
            }
            if (!(d2_TextSegs[j].flags & 0x40))
                draw = d2_DrawShape;
            else {
                d2_SetTextColour(d2_TextSegs[j].r, d2_TextSegs[j].g, d2_TextSegs[j].b);
                draw = d2_DrawShapeMode5;
            }
            if (!skip) {
                if (start < w->nobjs) {
                    n = w->nobjs - start;
                    pobj = &w->objs[start];
                    do {
                        o = &d2_Objs[*pobj];
                        if (o->hidden == 0) {
                            while (o->x < d2_TextSegs[j].w + x && x < o->x + o->w + 6
                                   && d2_FontHeight + y > o->y && y < o->y + o->h && flags != 0x40)
                                x = o->x + o->w + 6;
                        }
                        pobj++;
                        n--;
                    } while (n);
                }
                if (glyph == -1) {
                    if (hl) {
                        for (i = 0; i < (int)strlen(d2_TextSegs[j].text); i++) {
                            c = d2_TextSegs[j].text[i];
                            if (c != '~') {
                                g = d2_GetFrame(d2_FontPic, c);
                                if (i != 0)
                                    x -= g->kern;
                                if (d2_DrawShape(x, y, surf, d2_FontPic, c)) {
                                    d2_DrawShapeMode3(x, y, surf, d2_FontPic, c);
                                    x += g->adv + d2_LetterGap;
                                }
                            }
                            if (right < x)
                                break;
                        }
                        hl = 0;
                    } else {
                        for (i = 0; i < (int)strlen(d2_TextSegs[j].text); i++) {
                            c = d2_TextSegs[j].text[i];
                            if (c != '~') {
                                g = d2_GetFrame(d2_FontPic, c);
                                if (i != 0)
                                    x -= g->kern;
                                if (draw(x, y, surf, d2_FontPic, c))
                                    x += g->adv + d2_LetterGap;
                            }
                            if (right < x)
                                break;
                        }
                    }
                } else {
                    g = d2_GetFrame(gfont, glyph);
                    if (d2_DrawShape(x, y, surf, gfont, glyph))
                        x += g->adv + d2_LetterGap;
                }
            }
            if (c != '~' && !skip) {
                x += gap;
                if (extra != 0) {
                    x++;
                    extra--;
                }
            }
        }
        d2_TextSegCount = 0;
    }
    w->flags = savedflags;
}
