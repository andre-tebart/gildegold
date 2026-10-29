/* 0x5cfc00, the espionage owner message (869 B): param_1 the message record (+2 the type, +4 the
   handle), param_2 the request record. Type 6: the config cfg (20 dwords, [0] = 1, [1] = -1, [2] = 4,
   [4] = 6, [7 .. 14] = -1) with the text 0x5176d0 (0x9a) creates the spy request 0x5b2180 (1, cfg,
   text, 0, 0); other types: the building 0x501190 (1, 1, param_2 + 4). No request / no owner word +0x27
   -> 0. The 0x76 message (+8 the handle, +0xc = -1, the clock +0x28 .. +0x34, +0x36 = 10, +0x58 the
   dword at the request pointer + 1, +0x6e = 0x177) is sent by 0x4c4b70 with the object block 0x4c3ba0 /
   0x4c3c00 (+0x25 the request pointer + 1) / 0x4c49a0. Then 0x505230 (the owner's character, 0, 0) - in
   the other-type path only when 0x5ba6b0 (the character, 0x2c4) == 0 or 0x42ec20 () > 0x5eb94c -; when
   the owner kind is 6 / 7 the text 0x5176d0 (0xff1) goes through 0x53dda0 (the owner's character, -1,
   0, 0x5fb). Returns 1 (0 without a request). */

int ai_Unk5cfc00(int param_1, int *param_2)
{
    int cfg[20];
    char text[0x300];
    int rec[0x3e];
    char *p;
    int req;
    int i;
    int owner;

    if (*(char *)(param_1 + 2) == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        for (i = 7; i < 15; i++)
            cfg[i] = -1;
        txt_Format(text, (char *)0x9a);
        cfg[0] = 1;
        cfg[1] = -1;
        cfg[2] = 4;
        cfg[4] = 6;
        req = ai_Unk5b2180(1, (int)cfg, text, 0, 0);
        if (req == 0)
            return 0;
    } else {
        req = gm_BuildQuery(1, 1, param_2[1]);
        if (req == 0)
            return 0;
    }
    if (*(unsigned short *)(req + 0x27) == 0)
        return 0;
    p = (char *)req + 1;
    *(char *)((char *)rec + 4) = 0x76;
    rec[2] = *(int *)(param_1 + 4);
    rec[3] = -1;
    rec[10] = U_I((unsigned)&ai_Time);
    rec[11] = U_I((unsigned)&ai_TimeHour);
    rec[12] = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)((char *)rec + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
    *(char *)((char *)rec + 0x36) = 10;
    rec[0x16] = *(int *)p;
    *(short *)((char *)rec + 0x6e) = 0x177;
    cm_SendLongMessage(rec);
    cm_BeginFieldBlock((char *)*param_2, *(int *)(*param_2 + 2));
    cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x25, p);
    cm_SendFieldDeltas();
    owner = *(unsigned short *)(req + 0x27);
    if (*(char *)(param_1 + 2) != 6) {
        if (ai_InventoryHas((char *)((unsigned)&ai_Chars + owner * 0x21c), 0x2c4) == 0 ||
            util_RandomF() > *(float *)(unsigned)&ai_Unk5eb94c)
            gm_Unk505230((char *)((unsigned)&ai_Chars + owner * 0x21c), 0, 0);
    } else {
        gm_Unk505230((char *)((unsigned)&ai_Chars + owner * 0x21c), 0, 0);
    }
    if (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + owner * 0x21c) == 6 ||
        *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + owner * 0x21c) == 7) {
        txt_Format(text, (char *)0xff1);
        he_ShowNews(U_I((unsigned)&ai_CharInts + owner * 0x21c), -1, text, 0, 0x5fb, 0);
    }
    return 1;
}
