/* 0x5cb4f0, the espionage window (1440 B, ROUGH SKELETON: no Ghidra function; from the disassembly):
   param_1 is the request record (+2 the type 6 / 7, +0x194 the amount, +0x214 / +0x218 the two character
   ids), param_2 the player slot (0x509e60). Types other than 6 / 7: the amount < 4 returns 0x20; the two
   characters 0x505df0 (missing -> 0x20); the price (int)((float)(0x50a640 (one) + 0x50a640 (the other)) *
   0x5eb78c) against the money 0x509e60 (param_2) (too little -> 0x20); the two rolls 0x42ec00 (0x1e) + 3
   and the relations 0x50c520 (both directions), then the message 0x53dda0 (the text 0x1dd3) and the
   second message (0x6269ac, 0x5fb), return 0x10. Type 6 / 7: the config (0x400, 0x5eed68, 0x5eed6c, 6)
   and the texts 0x1dcf / 0x1dd0 open the menu 0x5c1130 twice; the payment and the texts after it are
   left as a comment. ROUGH SKELETON. */

int ai_EspionageWindow(int param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int cfg[0x14];
    char text[0x100];
    int chr1;
    int chr2;
    int price;
    int r;
    int i;

    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7) {
        if (*(int *)(param_1 + 0x194) < 4)
            return 0x20;
        chr1 = gm_FindChar(*(int *)(param_1 + 0x214));
        chr2 = gm_FindChar(*(int *)(param_1 + 0x218));
        if (chr1 == 0 || chr2 == 0)
            return 0x20;
        price = (int)((float)(gm_GetWorth(*(unsigned short *)chr2) +
                              gm_GetWorth(*(unsigned short *)chr1)) * (0.0199999996f));
        if (gm_GetGoodsValue(param_2) < price)
            return 0x20;
        gm_Opinion(*(unsigned short *)chr1, *(unsigned short *)chr2);
        gm_Opinion(*(unsigned short *)chr2, *(unsigned short *)chr1);
        /* ROUGH SKELETON: the two rolls 0x42ec00 (0x1e) + 3, the payment and the messages 0x53dda0
           (the text 0x1dd3 and the key 0x6269ac / 0x5fb) are not written yet */
        return 0x10;
    }
    for (i = 0; i < 0x14; i++)
        cfg[i] = 0;
    cfg[1] = 0x400;
    cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
    cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
    cfg[4] = 6;
    txt_Format(text, (char *)0x1dcf);
    r = ai_AmountMenu((int)cfg, (int)text, 0, 0);
    if (r == 0)
        return 0;
    txt_Format(text, (char *)0x1dd0, *(unsigned short *)r);
    r = ai_AmountMenu((int)cfg, (int)text, 0, 0);
    if (r == 0)
        return 0;
    /* ROUGH SKELETON: the menu result, the payment and the texts 0x1dd1 .. are not written yet */
    return 0x10;
}
