/* 0x59be90, sorts and shows the 7-int records of a credit list (656 B, ROUGH): the valid records are counted
   from the tail (record +0x186: field 3 = 0 or field 0 = -1), sorted by their name (the record's city word at
   +0xc, the name table 0x143c860; the word equal to the player's city 0x7e3f94 + player * 0x2f4 gives
   "AAAAAAAAA" 0x62bb24, 0 gives "ZZZZZZZZZ" 0x62bb18) with a bubble sort (strcmp > 0 swaps the 7 ints), then
   each record's two controls (+0 and +4) are drawn by 0x46cda0 at x = the control's +0x10 / y = 0x6ed980 + 6 +
   i * 0x4c (and + 0x30) */

void ai_Unk59be90(int *param_1)
{
    char nameA[0x80];
    char nameB[0x80];
    int tmp[7];
    int recs;
    int i;
    int j;
    int n;
    int v;
    int ctl;

    n = 0;
    for (i = 0, recs = 0; i < 16; i++) {
        if (param_1[i * 7 + 3] != 0 && param_1[i * 7] != -1)
            recs = i + 1;
    }
    n = recs;
    for (i = 0; i < n; i++)
        for (j = 0; j < n - 1 - i; j++) {
            v = param_1[j * 7 + 3];
            if (v == U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4))
                strcpy(nameA, "AAAAAAAAA");
            else if (v == 0)
                strcpy(nameA, "ZZZZZZZZZ");
            else
                strcpy(nameA, ((char **)(unsigned)&ai_Unk143c860)[v]);
            v = param_1[(j + 1) * 7 + 3];
            if (v == U_W((unsigned)&ai_Unk7e3f94 + U_I((unsigned)&ai_OwnPlayer) * 0x2f4))
                strcpy(nameB, "AAAAAAAAA");
            else if (v == 0)
                strcpy(nameB, "ZZZZZZZZZ");
            else
                strcpy(nameB, ((char **)(unsigned)&ai_Unk143c860)[v]);
            if (strcmp(nameA, nameB) > 0) {
                memcpy(tmp, param_1 + j * 7, 0x1c);
                memcpy(param_1 + j * 7, param_1 + (j + 1) * 7, 0x1c);
                memcpy(param_1 + (j + 1) * 7, tmp, 0x1c);
            }
        }
    for (i = 0; i < n; i++) {
        ctl = param_1[i * 7];
        if (ctl != -1) {
            d2_MoveObj(*(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x10 + ctl * 0x2ec),
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + (short)i * 0x4c, ctl);
            ctl = param_1[i * 7 + 1];
            d2_MoveObj(*(unsigned short *)(U_I((unsigned)&d2_Objs) + 0x10 + ctl * 0x2ec),
                         *(short *)((unsigned)&d2_LastWindowPtr + 6) + (short)i * 0x4c + 0x30, ctl);
        }
    }
}
