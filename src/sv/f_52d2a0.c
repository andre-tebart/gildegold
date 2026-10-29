/* 0x52d2a0, draws the names of the items (0x4ffe90 lists of the shown building / group: the not-person items first, then the persons (class 6 / 2)) above their screen boxes */

void sv_Unk52d2a0(void)
{
    int sub, t[3], x;
    unsigned short *q;
    char c;

    if (SV_I((unsigned)&sv_PanelShown) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
            return;
        q = (unsigned short *)gm_Query(*(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d), 1, 0, 0xfd);
        if (q == 0)
            sub = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d);
        else
            sub = *(int *)((char *)q + 0x14);
    } else
        sub = *(int *)(SV_I((unsigned)&sv_PanelShown) + 0x14 * 2);
    d2_SetFont(0x54);
    q = (unsigned short *)((char *(*)())gm_Query)(sub, 1, 5);
    while (q != 0) {
        if (*(int *)(q + 0xc) != 0 && (c = ai_Goods[*q * 0x41]) != 6 && c != 2 && (char)d3if_ScreenBox(*(int *)(q + 0xc), t) != 0) {
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, SV_I((unsigned)&ai_Unk143c860 + *q * 8), 0x28);
            d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        q = (unsigned short *)gm_QueryNext();
    }
    if (SV_I((unsigned)&sv_PanelShown) == 0) {
        if (SV_I((unsigned)&ai_FallbackBuilding) != 0)
            sub = *(int *)(SV_I((unsigned)&ai_FallbackBuilding) + 0x5d);
    } else
        sub = *(int *)(SV_I((unsigned)&sv_PanelShown) + 0x14 * 2);
    q = (unsigned short *)gm_Query(sub, 2, 5, 6);
    while (q != 0) {
        if (*(int *)(q + 0xc) != 0 && ((c = ai_Goods[*q * 0x41]) == 6 || c == 2) && (char)d3if_ScreenBox(*(int *)(q + 0xc), t) != 0) {
            x = (t[2] - t[0]) / 2 - 0x50 + t[0];
            d2_DrawTextBox(x, t[1], 0xa0, 0x60, d2_ScreenSurf, SV_I((unsigned)&ai_Unk143c860 + *q * 8), 0x28);
            d2_AddDirty(x, t[1], SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
        }
        q = (unsigned short *)gm_QueryNext();
    }
}
