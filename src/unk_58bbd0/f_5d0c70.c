/* 0x5d0c70, a message callback (484 B): param_1 the message record (+2 the type, +4 the handle), param_2
   the request record (+4 / +8 the two target ids). Type 6: the config cfg (20 dwords, [1] = 0x400, [2] /
   [3] = U_I (0x5eed68) / U_I (0x5eed6c), [4] = 6, [7 .. 14] = -1) with the text 0x5176d0 (0x9c) opens
   the first window 0x5c1130 (cfg, text, 0, 0); then [5] = 3 and the word [9] = the first record's id,
   the text 0x5176d0 (0x9e) opens the second window with the same cfg; other types: the two targets
   0x505df0 (param_2 + 4 / + 8); a missing record -> 0. Then the amount -40 or -80 by 0x5ba6b0 (param_1,
   0x2c5) and two 0x4c4b00 transfers between the two records (the handles +4, the amount, 1). When the
   first record's type is 6 / 7 the text 0x5176d0 (0xfea, the second record's word) goes through
   0x53dda0 (the first + 4, -1, 1, 0x5fb); when the second's is, the text 0x5176d0 (0xfeb, the first
   word twice) goes through 0x53dda0 (the second + 4, -1, 1, 0x5fb). Returns 1 (0 without records). */

int ai_Unk5d0c70(int param_1, int param_2)
{
    int cfg[20];
    char text[0x200];
    char text2[0x400];
    int a;
    int b;
    int amount;
    int i;
    int ret;

    ret = 0;
    if (*(char *)(param_1 + 2) == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        for (i = 7; i < 15; i++)
            cfg[i] = -1;
        txt_Format(text, (char *)0x9c);
        cfg[0] = 0;
        cfg[1] = 0x400;
        cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
        cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
        cfg[4] = 6;
        a = ai_AmountMenu(cfg, text, 0, 0);
        if (a == 0)
            return 0;
        cfg[5] = 3;
        *(unsigned short *)((char *)cfg + 0x24) = *(unsigned short *)a;
        txt_Format(text, (char *)0x9e);
        b = ai_AmountMenu(cfg, text, 0, 0);
        if (b == 0)
            return 0;
    } else {
        a = gm_FindChar(*(int *)(param_2 + 4));
        b = gm_FindChar(*(int *)(param_2 + 8));
        if (a == 0 || b == 0)
            return 0;
    }
    ret = 1;
    amount = (ai_InventoryHas((void *)param_1, 0x2c5) != 0 ? 2 : 1) * -40;
    cm_SendRelations(*(int *)(a + 4), *(int *)(b + 4), amount, 1);
    cm_SendRelations(*(int *)(b + 4), *(int *)(a + 4), amount, 1);
    if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 7) {
        txt_Format(text2, (char *)0xfea, *(unsigned short *)b);
        he_ShowNews(*(int *)(a + 4), -1, 1, text2, 0x5fb, 0);
    }
    if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
        txt_Format(text2, (char *)0xfeb, *(unsigned short *)a, *(unsigned short *)a);
        he_ShowNews(*(int *)(b + 4), -1, 1, text2, 0x5fb, 0);
    }
    return ret;
}
