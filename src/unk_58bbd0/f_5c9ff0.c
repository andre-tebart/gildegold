/* 0x5c9ff0, the blackmail contact variant (342 B): param_1 is the message record (AIMSG: +2 the type, +4
   the handle), param_2 the building (+0x214 the target id). Types 6 / 7: the request record local_188 (20
   dwords, +4 = 0x400, +0x10 = 6, +8 = 0xa7fdfbb6, +0xc = 1) with the text 0x5176d0 (0x1d61) through the
   window 0x5c1130 (record, text, 0, the callback 0x5c9df0); returns whether the window opened. Other types:
   the target 0x505df0 (none -> 0x60), the close 0x4c5f60 (-2) and the message record (kind byte 0x4b /
   type 2, the clock 0x770ae0 .., the handle, the target handle) sent by 0x4c4b70, return 0x10. */

unsigned char ai_BlackmailContactVariant(int param_1, int param_2)  /* @original: the callers push (c, m) (0x4f52e0); the body reads byte offsets of the int params, which the draft keeps */
{
    int rec[0x3e];
    int cfg[20];
    char text[64];
    int i;
    int target;

    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7) {
        target = (int)gm_FindChar(*(int *)(param_2 + 0x214));
        if (target == 0)
            return 0x60;
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffe);
        for (i = 0; i < 0x3e; i++)
            rec[i] = 0;
        *(char *)((char *)rec + 4) = 0x4b;
        rec[2] = *(int *)(param_1 + 4);
        rec[3] = *(int *)(param_1 + 4);
        rec[0x16] = *(int *)(param_1 + 4);
        rec[10] = *(int *)(unsigned)&ai_Time;
        rec[11] = *(int *)(unsigned)&ai_TimeHour;
        rec[12] = *(int *)((unsigned)&ai_Time + 0x8);
        *(short *)((char *)rec + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        *(char *)((char *)rec + 0x36) = 2;
        rec[0x17] = *(int *)(target + 4);
        cm_SendLongMessage(rec);
        return 0x10;
    }
    for (i = 0; i < 20; i++)
        cfg[i] = 0;
    cfg[1] = 0x400;
    cfg[4] = 6;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    txt_Format(text, (char *)0x1d61);
    return ai_AmountMenu(cfg, text, 0, (void *)ai_Unk5c9df0) != 0;
}
