/* 0x5cacd0, the embezzlement contract handler (302 B, a char result): param_1 is the message record (the
   AIMSG: +2 the type 6 / 7, +4 the handle, +9 a flag, +0x194 the amount), param_2 the request (dword +4
   the amount 1 .. 5, +0x214 the target id). Types 6 / 7: the config (0x400, the globals 0x5eed68 /
   0x5eed6c, 6) and the text 0x5176d0 (0x1dae, 0x1daf / 0x1db0 by the flag +9) open the menu 0x5c1130
   with the callback 0x5caab0; nonzero -> 1. Other types: the amount 1 .. 5 and <= the record +0x194, the
   target 0x505df0, the payment 0x494d40 (the record, the target, the amount), the close 0x4c5f60 (-the
   amount) and the line 0x4c4b00 (the handle, the target +4, the payment, 1), return 0x10; else 0x60. */

char ai_EmbezzlementContract(int param_1, int param_2)  /* @original: the callers push (c, m) (0x4f66b0 / 0x4f67c0); the draft body reads its own offsets of the two params */
{
    int cfg[20];
    char text[64];
    int i;
    int n;
    int chr;
    int v;

    if (*(char *)(param_1 + 2) == 6 || *(char *)(param_1 + 2) == 7) {
        for (i = 0; i < 0x14; i++)
            cfg[i] = 0;
        cfg[1] = 0x400;
        cfg[4] = 6;
        cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
        cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
        txt_Format(text, (char *)0x1dae,
                     *(char *)(param_1 + 9) == 0 ? 0x1daf : 0x1db0);
        return ai_AmountMenu((int)cfg, (int)text, 0, (int)ai_Unk5caab0) != 0;
    }
    n = *(int *)(param_2 + 4);
    if (n > *(int *)(param_1 + 0x194) || n < 1 || n > 5)
        return 0x60;
    chr = gm_FindChar(*(int *)(param_2 + 0x214));
    if (chr == 0)
        return 0x60;
    v = ai_Unk494d40(param_1, (void *)chr, n);
    cm_SendMoneyDelta(*(int *)(param_1 + 4), -n);
    cm_SendRelations(*(int *)(param_1 + 4), *(int *)(chr + 4), v, 1);
    return 0x10;
}
