/* 0x5c9c50, the blackmail contact (406 B): param_1 is the message record (AIMSG: +2 the type, +4 the
   handle), param_2 the building (+0x214 the target id). Types 6 / 7: the request record local_188 (20
   dwords, +4 = 0x400, +0x10 = 6, +8 = 0xa7fdfbb6, +0xc = 1) with the text 0x5176d0 (0x1d66) through the
   window 0x5c1130 (record, text, 0, the callback 0x5c9990), 0x81 when the record's +0x1c8 bit 0x400 is set,
   else 2. Other types: the target 0x505df0; no target -> 0x60; the two rolls 0x42ec00 (9), the message
   record (the clock 0x770ae0 .., the handle, the target handle, the kinds 0x47 / 2, the texts 0x1d6d /
   0x1d76 + the rolls) sent by 0x4c4b70 and the close 0x4c5f60 (-2), return 0x10. ROUGH: record by offset. */

int ai_BlackmailContact(int param_1, int param_2)  /* @original: the callers push (c, m) (0x4f5350); the draft body reads its own offsets of the two params */
{
    int rec[0x3e];
    int cfg[20];
    char text[64];
    int i;
    int h;
    int ret;
    int target;
    short r1;
    short r2;

    ret = 2;
    if (*(char *)(param_1 + 2) != 6 && *(char *)(param_1 + 2) != 7) {
        target = (int)gm_FindChar(*(int *)(param_2 + 0x214));
        if (target == 0)
            return 0x60;
        r1 = util_Random(9);
        r2 = util_Random(9);
        for (i = 0; i < 0x3e; i++)
            rec[i] = 0;
        *(char *)((char *)rec + 4) = 0x47;
        rec[2] = *(int *)(param_1 + 4);
        rec[3] = *(int *)(param_1 + 4);
        rec[0x16] = *(int *)(param_1 + 4);
        rec[10] = *(int *)(unsigned)&ai_Time;
        rec[11] = *(int *)(unsigned)&ai_TimeHour;
        rec[12] = *(int *)((unsigned)&ai_Time + 0x8);
        *(short *)((char *)rec + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        *(char *)((char *)rec + 0x36) = 2;
        rec[0x17] = *(int *)(target + 4);
        *(short *)((char *)rec + 0x60) = r2 + 0x1d6d;
        *(short *)((char *)rec + 0x62) = r1 + 0x1d76;
        h = cm_SendLongMessage(rec);
        cm_SendMoneyDelta(*(int *)(param_1 + 4), 0xfffffffe);
        return 0x10;
    }
    for (i = 0; i < 20; i++)
        cfg[i] = 0;
    cfg[1] = 0x400;
    cfg[4] = 6;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    txt_Format(text, (char *)0x1d66);
    i = ai_AmountMenu(cfg, text, 0, (void *)ai_Unk5c9990);
    if (i != 0 && (*(unsigned int *)(i + 0x1c8) & 0x400) != 0)
        ret = 0x81;
    return ret;
}
