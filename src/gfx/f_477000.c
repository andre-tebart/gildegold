/* 0x477000, hover time and the scroll wheel / scroll buttons of windows @deep B3: 151 code hunks; read the whole body against 0x477000..0x47731c: flow, stores, conditions and calls are the same (reset block 5 stores, f8aac8/6ed9e4 if-else, held block with di_ClearEvents 0x10, f8a650/f8a652/f8ab64 stores, btndown/btnup order, both scroll-button branches, wup/wdown, saved font); the code hunks are instruction shape only: the original caches the mouse shorts in di/bx at entry and re-reads them after di_ClearEvents (0x4115b0) (mov di,WORD; movsx ecx,di), ours loads/re-uses them as 32-bit (mov eax,dword / mov di late); previous note already tried short locals (71 asm lines, no reload). No meaning error found */

void d2_Unk477000(void)
{
    int saved = -1;
    D2WIN *w, *tw;
    int i, step, id, bd, bu;

    if (d2_Unkf8aacc != d2_Unkf8ab64 || d2_MouseX < d2_Unk5febdc - 5 || d2_MouseY < d2_Unk5febe0 - 5 ||
        d2_MouseX > d2_Unk5febdc + 5 || d2_MouseY > d2_Unk5febe0 + 5 || dm_L.ldown != 0) {
        d2_Unkf8aac8 = -1;
        d2_Unkf8a654 = tb_AnimTick;
        d2_Unk6ed9e4 = 0;
        d2_Unk5febdc = d2_MouseX;
        d2_Unk5febe0 = d2_MouseY;
    }
    if (d2_Unkf8aac8 == -1) {
        if (d2_Unkf8aacc != -1 && dm_L.rhold != 0) {
            d2_Unkf8aac8 = d2_Unkf8aacc;
            d2_Unk6ed9e4 = 1;
            d2_Unk5febdc = d2_MouseX;
            d2_Unk5febe0 = d2_MouseY;
            goto held;
        }
    } else if (d2_Unk6ed9e4 != 0) {
held:
        if (dm_L.rhold == 0) {
            d2_Unkf8aac8 = -1;
            d2_Unkf8a654 = tb_AnimTick;
            d2_Unk6ed9e4 = 0;
            d2_Unk5febdc = d2_MouseX;
            d2_Unk5febe0 = d2_MouseY;
            di_ClearEvents(0x10);
        }
    }
    if (d2_CurrentFont != -1)
        saved = d2_CurrentFont;
    d2_Unkf8a650 = d2_MouseX;
    d2_Unkf8a652 = d2_MouseY;
    d2_Unkf8ab64 = d2_Unkf8aacc;
    for (i = 0; i < 0x60; i++) {
        w = &d2_Wins[i];
        if ((w->flags & 0x200) && w->active == 1 && w->font != 0)
            d2_SetFont(w->font);
    }
    if (d2_HitWindow != -1 && (w = &d2_Wins[d2_HitWindow]) != 0 && dm_L.ldown != 0 && (bd = w->btndown) != -1 &&
        (bu = w->btnup) != -1 && d2_FocusedInput == 0) {
        if (d2_PointerObj == bd) {
            tw = &d2_Wins[d2_Objs[bd].winIndex];
            if (tw->scrollRemY == tw->scrollStep || tw->scrollRemY == 0)
                d2_ScrollWindowBy(0, tw->visible, tw->id);
        } else if (d2_PointerObj == bu) {
            tw = &d2_Wins[d2_Objs[bu].winIndex];
            if (tw->scrollRemY == tw->scrollStep || tw->scrollRemY == 0)
                d2_ScrollWindowBy(0, -tw->visible, tw->id);
        }
    }
    if (d2_PointerWinVisible != -1) {
        w = &d2_Wins[d2_PointerWinVisible];
        if (w != 0) {
            if (dm_L.wup != dm_L.wdown && (step = w->visible) != 0 && d2_FocusedInput == 0) {
                if (dm_L.wup != 0) {
                    id = w->id;
                    step = -step;
                } else {
                    if (dm_L.wdown == 0)
                        goto done;
                    id = w->id;
                }
                d2_ScrollWindowBy(0, step, id);
            }
        }
    }
done:
    if (saved != -1)
        d2_SetFont(saved);
}
