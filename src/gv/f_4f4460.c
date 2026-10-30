/* 0x4f4460, executes a character action (a = 0 .. 9) for the character c: 0 / 2 message 0x5c (value 3, kind 1 / 3), 1 / 3 message 0x5b (value 0.03 * the 0x50a640 value, kind 1 / 4), 4 / 6 / 8 0x4c5fa0 (id, 1 / 2 / 3, 0), 5 0x4e6680 (byte
   +0x166, kind 4), 7 sets byte 0xd of the character's record (2 or 1) and sends it, 9 a message to the players near (0x18 targets) and 0x4e6680 (kind 3). ROUGH */

void unk_Unk4f4460(unsigned short *c, int a)
{
    char m[0x240];
    char kind;
    int v, i, k;
    unsigned id;
    char *p;
    unsigned ids[6];
    int tg[0x12];

    v = gm_GetWorth(*c);
    switch (a) {
    case 0:
    case 1:
    case 2:
    case 3:
        *(int *)(m + 0x28) = *(int *)ai_Time;
        *(int *)(m + 0x2c) = *(int *)(ai_Time + 4);
        *(int *)(m + 0x30) = *(int *)(ai_Time + 8);
        *(short *)(m + 0x34) = *(short *)(ai_Time + 12);
        m[0x36] = 1;
        m[4] = (a & 1) ? 0x5b : 0x5c;
        gm_AddTime(m + 0x28, 1, 0, 0);
        *(int *)(m + 8) = *(int *)(c + 2);
        *(int *)(m + 0xc) = -1;
        *(int *)(m + 0x58) = (a & 1) ? (int)(v * 0.03f) : 3;
        *(int *)(m + 0x5c) = (a == 0) ? 1 : (a == 1) ? 1 : (a == 2) ? 3 : 4;
        cm_SendLongMessage(m);
        return;
    case 4:
        ai_Unk4c5fa0(*(int *)(c + 2), 1, 0);
        return;
    case 5:
        ids[0] = *(unsigned char *)(c + 0xb3);
        amt_SendOfficeMessage(ids[0], 0, 0, 4, 0xff);
        return;
    case 6:
        ai_Unk4c5fa0(*(int *)(c + 2), 2, 0);
        return;
    case 7:
        kind = (*((unsigned char *)c + 0xd) > 2) + 1;
        cm_BeginFieldBlock(c, *(int *)(c + 2));
        cm_AddFieldDelta(1, 1, 0xd, &kind);
        cm_SendFieldDeltas();
        return;
    case 8:
        ai_Unk4c5fa0(*(int *)(c + 2), 3, 0);
        return;
    case 9:
        ai_Unk4c5fa0(*(int *)(c + 2), 0xff, 1);
        tg[0] = *(int *)(c + 2);
        tg[1] = *(int *)((char *)ai_JudgeCatholic + 4);
        tg[2] = *(int *)((char *)ai_JudgeProtestant + 4);
        i = 3;
        for (p = ai_Chars[0] + 2; (int)p <= ((unsigned)((unsigned)ai_Chars + 0x65400) + 0x1) && i < 0x10; p += 0x21c) {
            if (tg[0] != *(int *)(p + 2) && (*p == 6 || *p == 7 || *p == 5))
                tg[i++] = *(int *)(p + 2);
        }
        *(char *)(m + 0x100) = (char)i;
        *(int *)(m + 0x120) = *(int *)(ai_Chars[ai_PlayerId] + 4);
        *(int *)(m + 0x114) = *(int *)ai_Time;
        *(int *)(m + 0x118) = -1;
        gm_AddTime(m + 0x114, 0x18, 0, 0);
        m[0x124] = 5;
        m[0xb0] = (char)util_RandomU(5);
        ai_Unk4c4fa0(m + 0x12c);
        if (amt_FindOffice(c, m + 0x18, ids) == 0)
            return;
        amt_SendOfficeMessage(ids[0], 0, 0, 3, 0xff);
        return;
    }
}
