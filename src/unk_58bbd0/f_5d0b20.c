/* 0x5d0b20, a message callback (335 B): param_1 the message record (+2 the type, +4 the handle, +0
   the word), param_2 the request record (+4 the target id). Type 6: the config cfg (20 dwords,
   [0] = 1, [1] = 0x401 / 0x402 by the player's byte +9 of its character record, [2] / [3] = U_I
   (0x5eed68) / U_I (0x5eed6c), [4] = 6, [7 .. 14] = -1) with the text 0x5176d0 (0x9c) opens the window
   0x5c1130 (cfg, text, 0, 0); other types: the target 0x505df0 (param_2 + 4); no record -> 0. Then
   0x4c4b00 (the handle, the target + 4, the amount 25 / 50, 1) with the amount by 0x5ba6b0 (the record,
   0x2c5) (2 -> 50, else 25); when the target's type is 6 / 7 the text 0x5176d0 (0xfe9, the record's
   word) goes through 0x53dda0 (the target + 4, -1, 0, 0x5ec). Returns 1 (0 without a record / target). */

int ai_Unk5d0b20(int param_1, int param_2)
{
    int cfg[20];
    char text[0x200];
    char text2[0x400];
    int target;
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
        cfg[0] = 1;
        cfg[1] = *(char *)((unsigned)&ai_Unk770b89 + U_W((unsigned)&ai_PlayerId) * 0x21c) != 0 ? 0x402 : 0x401;
        cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
        cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
        cfg[4] = 6;
        target = ai_AmountMenu(cfg, text, 0, 0);
        if (target == 0)
            return 0;
    } else {
        target = gm_FindChar(*(int *)(param_2 + 4));
        if (target == 0)
            return 0;
    }
    ret = 1;
    amount = (ai_InventoryHas((void *)param_1, 0x2c5) != 0 ? 2 : 1) * 25;
    cm_SendRelations(*(int *)(param_1 + 4), *(int *)(target + 4), amount, 1);
    if (*(char *)(target + 2) == 6 || *(char *)(target + 2) == 7) {
        txt_Format(text2, (char *)0xfe9, *(unsigned short *)param_1);
        he_ShowNews(*(int *)(target + 4), -1, 0, text2, 0x5ec, 0);
    }
    return ret;
}
