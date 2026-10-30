/* 0x5194c0, the header of the save game: the id 0x1005a (also kept in 0x623650), a flag word (the argument), the player's name (0x20 bytes), two words, a byte, the language byte, the clock, then the scenario record (the name "SCENARIO" and the first character's fields: +0xb0 / +0xc0 16 bytes, +0x8c, +0xd4, +0xce4 / +0xce6, +0x8d, a value of 0x50a640 and the byte +0x89; or the zeroed defaults when no character), the byte 0x6f1c38, the buildings 0x6f2308 / 0x6f230c / 0x6f2310 ids, the table 0x76fa24 (8 words of 8 bytes), and the screen shot (0x78 x 0xa0 pixels of 3 bytes made with 0x41e6d0 from the bitmap 0x766320, 0xe100 bytes); finally 0x6f23fc and the 0x60 bytes of 0x75e640. Returns 1 when everything was written. ROUGH @deep B: 586 code hunks in 13 of 18 hunks (373 instructions ours vs 459 in the original; hunk 0: our frame is sub esp,0x50, the original's sub esp,0x124 and it loads 0x62365c / 0x623660 / 0x623664 into slots first; the block order of the whole body differs from the original at 0x5194c0). The draft's meaning (the SCENARIO header, the screen shot 0xe100 via 0x4223c0) is right, the shape is not: a rewrite from the disassembly, not a base reading fix. Q2/Q7: the callees vfs_fwrite, mem_Alloc, dd_SplitRGB, gm_GetWorth, gm_ConvertMoney are unnamed; Q5 ROUGH. */


int ls_WriteSaveHeader(int f, unsigned flags)
{
    char scen[0x10];
    char b0, b1, b2, b3;
    int fields[2];
    unsigned id;
    char *shot;
    int i, j, k, rgb, who;
    char name[0x23];
    int v[2];
    char c1, c2;
    int ref;

    strcpy(scen, "SCENARIO");
    b0 = b1 = b2 = b3 = 0;
    ref = -1;
    shot = (char *)mem_Alloc(0xe100, "ls:tmp");
    id = 0x1005a;
    ls_Version = 0x1005a;
    if (vfs_fwrite(&id, 4, 1, f) <= 0 || vfs_fwrite(&flags, 1, 1, f) <= 0)
        return 0;
    who = ai_OwnPlayer;
    strcpy(name, ai_PlayerSlotsV[who]);
    v[0] = *(int *)(ai_PlayerSlotsV[who] + 0x40);
    v[1] = *(int *)(ai_PlayerSlotsV[who] + 0x44);
    c1 = ai_PlayerSlotsV[who][0x48];
    c2 = ai_NumPlayers;
    if (vfs_fwrite(name, 0x20, 1, f) <= 0 || vfs_fwrite(v, 8, 1, f) <= 0 || vfs_fwrite(&c1, 1, 1, f) <= 0
        || vfs_fwrite(&c2, 1, 1, f) <= 0 || vfs_fwrite(ai_Time, 0xe, 1, f) <= 0)
        return 0;
    if (ai_PlayerId == -1) {
        if (vfs_fwrite(scen, 0x10, 1, f) <= 0 || vfs_fwrite(&b1, 1, 1, f) <= 0 || vfs_fwrite(&b3, 1, 1, f) <= 0
            || vfs_fwrite(&b0, 1, 1, f) <= 0 || vfs_fwrite(&ref, 4, 1, f) <= 0)
            return 0;
        if (vfs_fwrite(&b2, 1, 1, f) <= 0)
            return 0;
    } else {
        char *c = ai_Chars[(unsigned short)ai_PlayerId];

        if (vfs_fwrite(c + 0x30, 0x10, 1, f) <= 0 || vfs_fwrite(c + 0x40, 0x10, 1, f) <= 0
            || vfs_fwrite(c + 0xc, 1, 1, f) <= 0 || vfs_fwrite(c + 0x54, 4, 1, f) <= 0
            || vfs_fwrite(c + 0x164, 1, 1, f) <= 0 || vfs_fwrite(c + 0x166, 1, 1, f) <= 0
            || vfs_fwrite(c + 0xd, 1, 1, f) <= 0)
            return 0;
        ref = ((int (*)(int))gm_ConvertMoney)(gm_GetWorth(ai_PlayerId));
        if (vfs_fwrite(&ref, 4, 1, f) <= 0 || vfs_fwrite(c + 9, 1, 1, f) <= 0)
            return 0;
    }
    if (vfs_fwrite(&ls_SaveSlot, 1, 1, f) <= 0)
        return 0;
    ref = ai_PlayerBuildingI ? *(int *)(ai_PlayerBuildingI + 1) : (ai_FallbackBuildingI ? *(int *)(ai_FallbackBuildingI + 1) : -1);
    vfs_fwrite(&ref, 4, 1, f);
    ref = sv_PanelShown ? *(int *)(sv_PanelShown + 2) : -1;
    vfs_fwrite(&ref, 4, 1, f);
    for (i = 0; i < 8; i++)
        vfs_fwrite(ai_Unk76fa24 + i * 8, 4, 1, f);
    k = 0;
    for (j = 0x78; j != 0; j--) {
        int px = k * 3 + 1 + (int)shot;

        for (i = 0xa0; i != 0; i--) {
            dd_SplitRGB(ai_ScreenImage[k], px - 1, px, px + 1);
            k++;
            px += 3;
        }
    }
    rgb = vfs_fwrite(shot, 0xe100, 1, f);
    if (rgb == 0)
        return 0;
    if (shot != 0)
        mem_Free(shot);
    vfs_fwrite(&ai_Difficulty, 4, 1, f);
    vfs_fwrite(ls_SavePath, 0x60, 1, f);
    return 1;
}
