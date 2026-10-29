/* 0x5d0780, a message callback (317 B): param_1 the message record (+2 the type), param_2 the request
   record. Type 6: the config cfg (20 dwords, [0] = 1, [1] = -1, [2] = 0x90010, [3] = 0, [4] = 4, [7 ..
   14] = -1) with the text 0x5176d0 (0x9b) creates the request 0x5b2180 (1, cfg, text, 0, 0); other
   types: the building 0x501190 (1, 1, param_2 + 4); 0 when absent. Then over the 0x300 character
   records 0x770b80 + i * 0x21c: when its +0x16c is the request, the object block 0x4c3ba0 (the
   character, its +4) and 0x4c3c00 (1, 1, the byte +0x1c5 of the record as (x & 1) + (x >> 1) at the
   pointer 0x770d45 + i * 0x21c) and 0x4c49a0. Returns 1 once a request was created (0 otherwise). */

int ai_Unk5d0780(int param_1, int param_2)
{
    int cfg[20];
    char text[0x100];
    char b;
    int req;
    int i;
    int ret;

    ret = 0;
    if (*(char *)(param_1 + 2) == 6) {
        for (i = 0; i < 20; i++)
            cfg[i] = 0;
        for (i = 7; i < 15; i++)
            cfg[i] = -1;
        txt_Format(text, (char *)0x9b);
        cfg[0] = 1;
        cfg[1] = -1;
        cfg[2] = 0x90010;
        cfg[3] = 0;
        cfg[4] = 4;
        req = ai_Unk5b2180(1, (int)cfg, text, 0, 0);
        if (req == 0)
            return 0;
    } else {
        req = gm_BuildQuery(1, 1, *(int *)(param_2 + 4));
        if (req == 0)
            return 0;
    }
    ret = 1;
    for (i = 0; i < 0x300; i++) {
        unsigned char x = *(unsigned char *)(((unsigned)&ai_Selected + 0x3d) + i * 0x21c);

        if (*(int *)((unsigned)&ai_Unk770cec + i * 0x21c) == req) {
            b = (char)((x & 1) + (x >> 1));
            cm_BeginFieldBlock((char *)((unsigned)&ai_Chars + i * 0x21c), *(int *)((unsigned)&ai_CharInts + i * 0x21c));
            cm_AddFieldDelta(1, 1, (short)(((unsigned)&ai_Selected + 0x3d) + i * 0x21c) - (short)U_I((unsigned)&cm_FieldObject), &b);
            cm_SendFieldDeltas();
        }
    }
    return ret;
}
