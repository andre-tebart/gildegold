/* 0x468a90, frees everything of the layer @deep Q4 blocks the byte match: the four raw *(int *) accesses of the D2PIC free loop rewritten as D2PIC *p = (D2PIC *)(i + base) with p->data / p->size flip the address operand order to [ecx+ebx*1+0x34] where the original has [ebx+ecx*1+0x34] (i+base and base+i both, all four accesses, build check REGRESSED d2_Exit) - the IV-offset-as-base class of notes/units/d2.md; Q2 needs 9 names (d2_RemoveWindow, d2_FreeFades, d2_FrontSurf/92c/930/948/960/964/99c, roles only partly evidenced in d2.h); Q3 0xfa5d20 is the window walk's pointer-compare constant: w starts at &d2_Wins[0].active (0xf8f120 + 0x280 = 0xf8f3a0) and the end is &d2_Wins[0x60].active (0xfa5d20), 0x60 windows; the typed spelling `while (w < &d2_Wins[0x60].active)` clears Q3 but loses the byte match (check 2609, the original compares against the immediate), so the literal stays */

void d2_Exit(void)
{
    int i, base, t;
    int *w;

    base = 0;
    d2_FreeFades();
    i = 0;
    w = &d2_Wins[0].active;
    do {
        if (*w == 1)
            d2_RemoveWindow(i);
        w += sizeof(D2WIN) / 4;
        i++;
    } while ((int)w < (int)((unsigned)&d2_Wins + 0x16c00));
    i = 0;
    do {
        d2_RemoveObj(i);
        i++;
    } while (i < 0x2ff);
    i = 0;
    do {
        base = (int)d2_Pics;
        t = *(int *)(i + 0x34 + base);
        if (t && *(int *)(i + 0x38 + base)) {
            mem_Free((void *)t);
            *(int *)(i + 0x34 + base) = 0;
            d2_PicBytes -= *(int *)(i + 0x38 + base);
            base = (int)d2_Pics;
        }
        i += 0x54;
    } while (i < 0x2a000);
    if (d2_BackSurf) {
        d2_FreeSurface(d2_BackSurf);
        d2_BackSurf = 0;
    }
    if (d2_ScreenSurf) {
        d2_FreeSurface(d2_ScreenSurf);
        d2_ScreenSurf = 0;
    }
    if (d2_FrontSurf) {
        d2_FreeSurface(d2_FrontSurf);
        d2_FrontSurf = 0;
    }
    if (d2_BackgroundTile) {
        d2_FreeSurface(d2_BackgroundTile);
        d2_BackgroundTile = 0;
    }
    d2_FreeLightTable();
    if (d2_CaptionTex) {
        d3if_FreeSprite((int)d2_CaptionTex);
        d2_CaptionTex = 0;
    }
    if (d2_File)
        fclose(d2_File);
    d2_FontPic = 0;
    d2_File = 0;
    if (d2_DrawOrder) {
        mem_Free(d2_DrawOrder);
        d2_DrawOrder = 0;
    }
    if (d2_Objs) {
        mem_Free(d2_Objs);
        d2_Objs = 0;
    }
    if (d2_Pics) {
        mem_Free(d2_Pics);
        d2_Pics = 0;
    }
    if (d2_Dirty[0]) {
        mem_Free(d2_Dirty[0]);
        d2_Dirty[0] = 0;
    }
    if (d2_Dirty[1]) {
        mem_Free(d2_Dirty[1]);
        d2_Dirty[1] = 0;
    }
    if (d2_RedrawInfo2) {
        mem_Free(d2_RedrawInfo2);
        d2_RedrawInfo2 = 0;
    }
    if (d2_Shapes) {
        mem_Free(d2_Shapes);
        d2_Shapes = 0;
    }
}
