/* 0x5cba90, the espionage payment handler (880 B, ROUGH SKELETON: no Ghidra function; from the
   disassembly): param_1 is the message record (the AIMSG: +0 the id, +2 the type, +4 the handle, +0x194
   the amount, +0x1c8 the flags with 0x8000), param_2 the request (+0x214 the target id). Types other
   than 6: the amount < 2 or the flag 0x8000 clear returns 0x20; the target 0x505df0 (missing -> 0x60);
   the close 0x4c5f60 (-2 the message, -3 the target) and the carrier 0x4c4a60 (0x1c8, 4, 0x8000); a
   target of the type 6 / 7 gets the text 0x5176d0 (0x1dda, the id, 3) and the message 0x53dda0 (the
   target +4 twice, 0, the text, 0x5fb, the key 0x6269ac); return 0x10. Type 6: the flag 0x8000 clear
   shows the text 0x5176d0 (0x1ce0) / 0x521150 and returns 0x81; else the config (0x400, 0x5eed68,
   0x5eed6c, 6) and the text 0x1dd7 open the menu 0x5c1130; the rest of the window is left as a comment.
   ROUGH SKELETON. */

int ai_EspionagePayment(int param_1, int param_2)  /* @original: the callers push (c, m); the draft body reads its own byte offsets of the parameters */
{
    int cfg[0x14];
    char text[0x100];
    int target;
    int ret;
    int i;

    ret = 2;
    if (*(char *)(param_1 + 2) != 6) {
        if (*(int *)(param_1 + 0x194) < 2)
            return 0x20;
        if (!(*(int *)(param_1 + 0x1c8) & 0x8000))
            return 0x20;
        target = gm_FindChar(*(int *)(param_2 + 0x214));
        if (target == 0)
            return 0x60;
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffe);
        cm_SendMoneyDelta(*(int *)(target + 4), 0xfffffffd);
        cm_SendSetField(*(int *)(param_1 + 4), 0x1c8, 4, 0x8000, 0);
        if (*(char *)(target + 2) == 6 || *(char *)(target + 2) == 7) {
            txt_Format(text, (char *)0x1dda, *(unsigned short *)param_1, 3);
            he_ShowNews(*(int *)(target + 4), *(int *)(target + 4), 0, text, 0x5fb,
                         (char *)((unsigned)&ai_Unk625820 + 0x118c));
        }
        return 0x10;
    }
    for (i = 0; i < 0x14; i++)
        cfg[i] = 0;
    if (!(*(int *)(param_1 + 0x1c8) & 0x8000)) {
        txt_Format(text, (char *)0x1ce0);
        sv_MessageBox(text, 0);
        return 0x81;
    }
    cfg[1] = 0x400;
    cfg[2] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b0));
    cfg[3] = U_I(((unsigned)&ai_Unk5eeab8 + 0x2b4));
    cfg[4] = 6;
    txt_Format(text, (char *)0x1dd7);
    ret = ai_AmountMenu((int)cfg, (int)text, 0, 0);
    /* ROUGH SKELETON: the menu result and the window body after it are not written yet */
    return ret;
}
