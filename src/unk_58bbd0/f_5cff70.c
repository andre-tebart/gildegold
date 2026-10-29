/* 0x5cff70, the espionage owner window (2384 B, ROUGH SKELETON): param_1 the message record (+2 the
   type, +4 the handle), param_2 the request record. Type 6: the config cfg (20 dwords, [0] = 1, [1] =
   -1, [2] = 0xa0744300, [3] = 1, [4] = 6, [7 .. 14] = -1) with the text 0x5176d0 (0x9b) creates the
   spy request 0x5b2180 (1, cfg, text, 0, 0); other types: the building 0x501190 (1, 1, param_2[1]).
   Then a 0x76 record (zeroed, +8 the handle, +0xc = -1, the clock +0x28 .. +0x34, +0x36 = 10, +0x58
   the +4 dword of the request record (the Ghidra +1 dword view), +0x6e = 0x178) is sent by 0x4c4b70
   with the object block 0x4c3ba0 / 0x4c3c00 (+0x25 the request +4) / 0x4c49a0 and the flag 0x4c4a60
   (the request +4, 0x5a, 2, 0x20); then a 0x77 record (+0x36 = 1) the same way, 0x4c4a60 (0x5a, 2,
   0x20) and, when the owner word +0x27 is not 0xffff and of kind 6 / 7, the text 0x5176d0 (0xff2, the
   record) through 0x53dda0 (the owner's character 0x770b84 + owner * 0x21c, -1, the key 0x5fb).
   Return 1 (0 when a request could not be created). ROUGH: the disassembly shows a second 0x5b2180
   block at 0x5d07f4 and a scan from 0x770d45 after the messages (a candidate loop) that Ghidra folded
   away; not written yet. */

int ai_Unk5cff70(int param_1, int *param_2)
{
    int cfg[20];
    char text[0x100];
    char buf[0x800];
    int rec[0x3e];
    int h;
    int i;
    int owner;

    if (*(char *)(param_1 + 2) == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        for (i = 7; i < 15; i++)
            cfg[i] = -1;
        txt_Format(text, (char *)0x9b);
        cfg[0] = 1;
        cfg[1] = -1;
        cfg[2] = 0xa0744300;
        cfg[3] = 1;
        cfg[4] = 6;
        h = ai_Unk5b2180(1, (int)cfg, text, 0, 0);
        if (h == 0)
            return 0;
    } else {
        h = gm_BuildQuery(1, 1, param_2[1]);
        if (h == 0)
            return 0;
    }
    for (i = 0; i < 0x3e; i++)
        rec[i] = 0;
    *(char *)((char *)rec + 4) = 0x76;
    rec[2] = *(int *)(param_1 + 4);
    rec[3] = -1;
    rec[10] = U_I((unsigned)&ai_Time);
    rec[11] = U_I((unsigned)&ai_TimeHour);
    rec[12] = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)((char *)rec + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
    *(char *)((char *)rec + 0x36) = 10;
    rec[0x16] = *(int *)(h + 4);
    *(short *)((char *)rec + 0x6e) = 0x178;
    cm_SendLongMessage(rec);
    cm_BeginFieldBlock((char *)*param_2, *(int *)(*param_2 + 2));
    cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x25, (void *)(h + 4));
    cm_SendFieldDeltas();
    cm_SendSetField(*(int *)(h + 4), 0x5a, 2, 0x20, 0);
    for (i = 0; i < 0x3e; i++)
        rec[i] = 0;
    *(char *)((char *)rec + 4) = 0x77;
    rec[2] = *(int *)(param_1 + 4);
    rec[3] = -1;
    rec[10] = U_I((unsigned)&ai_Time);
    rec[11] = U_I((unsigned)&ai_TimeHour);
    rec[12] = U_I(((unsigned)&ai_Time + 0x8));
    *(short *)((char *)rec + 0x34) = U_W(((unsigned)&ai_Time + 0xc));
    *(char *)((char *)rec + 0x36) = 1;
    rec[0x16] = *(int *)(h + 4);
    cm_SendLongMessage(rec);
    cm_BeginFieldBlock((char *)*param_2, *(int *)(*param_2 + 2));
    cm_AddFieldDelta(4, 1, (short)(*param_2 - *(unsigned short *)(unsigned)&cm_FieldObject) + 0x25, (void *)(h + 4));
    cm_SendFieldDeltas();
    cm_SendSetField(*(int *)(h + 4), 0x5a, 2, 0x20, 0);
    owner = U_W(h + 0x27);
    if (owner != 0xffff &&
        (*(unsigned char *)(((unsigned)&ai_Chars + 0x2) + owner * 0x21c) == 6 ||
         *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + owner * 0x21c) == 7)) {
        txt_Format(buf, (char *)0xff2, h);
        he_ShowNews(U_I((unsigned)&ai_CharInts + owner * 0x21c), -1, buf, 0, 0x5fb, 0);
    }
    return 1;
}
