/* 0x52d010, the callback of 0x52d1e0 over the scene objects of kind 0x20: for a "gate" part of the shown building (0x6f230c / 0x6f2308) it finds the item of that name (the contact gates "contact_gate_<name>")
   in the building's lists (or the 0xfd item) and draws its name above the part's screen box; a missed start inside 0x52cef0 for Ghidra; ROUGH */

void sv_Unk52d010(char *part)
{
    char *b;
    unsigned short *it;
    char name[0x60], buf[0x80];
    int t[3], x, y, lst;

    b = SV_C((unsigned)&ai_FallbackBuilding);
    if (b == 0) {
        b = SV_C((unsigned)&ai_PlayerBuilding);
        if (b == 0)
            return;
    }
    if (strstr(part, (char *)(unsigned)"gate") == 0)
        return;
    strcpy(name, part + 0x10);
    lst = SV_I((unsigned)&ai_FallbackBuilding) != 0 ? 6 : 2;
    it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 4, lst);
    while (it != 0) {
        if (_strcmpi(name, ai_Goods + *it * 0x41 + 1) == 0)
            goto lfound;
        it = (unsigned short *)gm_QueryNext();
    }
    if (SV_I((unsigned)&ai_FallbackBuilding) == 0)
        return;
    it = (unsigned short *)gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
lfound:
    if (it == 0)
        return;
    if (*it == 0xfd) {
        if (SV_I((unsigned)&sv_PanelShown) != 0)
            sprintf(buf, "%s %s", SV_I((unsigned)&ai_Unk143c860 + *(unsigned short *)SV_I((unsigned)&sv_PanelShown) * 8), SV_I((unsigned)&full_data_143a3d0));
        else
            sprintf(buf, "%s %s", SV_I((unsigned)&full_data_143b3f8 + *(unsigned char *)b * 0x38), SV_I((unsigned)&full_data_143a3d0));
    } else
        sprintf(buf, "%s %s", SV_I((unsigned)&ai_Unk143c860 + *it * 8), SV_I((unsigned)&full_data_143a3cc));
    if ((char)d3if_ScreenBox(part, t) != 0) {
        x = (t[2] - t[0]) / 2 - 0x50 + t[0];
        y = t[1];
        d2_DrawTextBox(x, y, 0xa0, 0x60, d2_ScreenSurf, buf, 0x28);
        d2_AddDirty(x, y, SV_I((unsigned)&d2_TextDrawnW), SV_I((unsigned)&d2_TextDrawnH));
    }
}
