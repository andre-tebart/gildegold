/* 0x51cee0, the header of a save game: the id (kept in 0x623650, the version), the flag byte, the name, two words, the language and further bytes (each from the version noted), the 8 words of the party table, the screen shot (0xe100 bytes, converted back to the 16 bit bitmap 0x766320 with 0x41e670), the mode word (+0x98, 2 for versions below 0x10038) and the title (+0x9c, "Savegame" below 0x10039). Returns 1. ROUGH @deep B3 code hunks 354: the version gates are now aligned to the original's >= forms (0x10033/0x10028/0x10054/0x10034/0x10025), what remains is the register/stack shape - the draft keeps the header buffer and the loop counters in memory (mov [esp+0x18],0; mov esi,eax after the malloc) where the original holds them in esi/edi/ebx (xor esi,esi at 0x51cef3, the screenshot pointer in ebx, k in esi), the header stores at +0x70/+0x74/+0x98 are raw int casts (Q4) and the title is a strcpy at +0x9c. Read the header fields (version 0x623650, flag +4, name +5 0x20, the two words +0x28, language +0x30, the version gates, the 8 party words +0x78, the 0xe100 screenshot through 0x41e670 into 0x766320, the mode at +0x98 and the title +0x9c). Needs the rewrite in the original's register shape. Q1/Q2/Q3/Q4/Q5 open */


int ls_ReadSaveHeader(int f, int *h)
{
    char *hb = (char *)h;
    char *shot;
    int i, j, k = 0;
    unsigned char *px;

    shot = (char *)mem_Alloc(0xe100, "ls:tmp");
    LS_R(h, 4)
    ls_Version = *h;
    LS_R(hb + 4, 1)
    LS_R(hb + 5, 0x20)
    LS_R(hb + 0x28, 8)
    LS_R(hb + 0x30, 1)
    if (ls_Version >= 0x10033)
        LS_R(hb + 0x31, 1)
    if (ls_Version >= 0x10028) {
        LS_R(hb + 0x32, 0xe)
        LS_R(hb + 0x40, 0x10)
        if (ls_Version >= 0x10054) {
            LS_R(hb + 0x50, 0x10)
            LS_R(hb + 99, 1)
            LS_R(hb + 0x64, 4)
        }
        LS_R(hb + 0x60, 1)
        LS_R(hb + 0x61, 1)
        LS_R(hb + 0x62, 1)
        LS_R(hb + 0x68, 4)
        LS_R(hb + 0x6c, 1)
        LS_R(hb + 0x6d, 1)
        if (ls_Version < 0x1002b) {
            *(int *)(hb + 0x70) = -1;
            *(int *)(hb + 0x74) = -1;
        } else {
            LS_R(hb + 0x70, 4)
            LS_R(hb + 0x74, 4)
        }
        if (ls_Version >= 0x10034) {
            for (i = 0; i < 8; i++)
                LS_R(hb + 0x78 + i * 4, 4)
        }
        LS_R(shot, 0xe100)
        px = (unsigned char *)shot;
        for (j = 0x78; j != 0; j--) {
            for (i = 0xa0; i != 0; i--) {
                ((unsigned short *)(unsigned)&ai_ScreenImage)[k] = dd_RGB(px[0], px[1], px[2]);
                px += 3;
                k++;
            }
        }
    }
    if (shot != 0)
        mem_Free(shot);
    if (ls_Version >= 0x10025) {
        if (ls_Version < 0x10038)
            *(int *)(hb + 0x98) = 2;
        else
            LS_R(hb + 0x98, 4)
        if (ls_Version < 0x10039)
            strcpy(hb + 0x9c, "Savegame");
        else
            LS_R(hb + 0x9c, 0x60)
        return 1;
    }
    return 0;
}
