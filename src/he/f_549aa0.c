/* 0x549aa0, the run handler of the training result event (+0xac = base, +0xad = random range, +0xae = the ability): state 0 raises the ability (0x4c6020) of the player by base - random, with a news 0x16f2 .. when the value cap 252 is hit; a player of kind 6 whose value rises past the old one gets a message record (type 0x6a, 0x4c4b70), then 0x4c5870 and free (ROUGH) */

void he_Unk549aa0(int ev)
{
    char text[0x800];
    char name[0x20];
    char msg[0xf8];
    int st, rec, v, ab;

    st = *(int *)(ev + 0x70);
    if (st == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (st != 0)
        return;
    rec = (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c;
    v = *(unsigned char *)(ev + 0xac) - (unsigned short)util_Random(*(unsigned char *)(ev + 0xad));
    ab = *(unsigned char *)(ev + 0xae);
    if ((float)(*(unsigned char *)(rec + 0x80 + ab) + v) > 252.0f)
        v = (int)(252.0f - (float)*(unsigned char *)(rec + 0x80 + ab));
    ai_Unk4c6020(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), ab, v);
    if (*(char *)(rec + 2) == 6) {
        if ((int)(252.0f - (float)(*(unsigned char *)(rec + 0x80 + ab) + v)) < 0) {
            memset(msg, 0, sizeof(msg));
            msg[0] = 0x6a;
            *(int *)(msg + 4) = *(int *)(rec + 4);
            *(int *)(msg + 8) = -1;
            *(HETIME *)(msg + 0x1c) = ai_Time;
            msg[0x24] = 9;
            msg[0x2a] = (char)ab;
            cm_SendLongMessage(msg);
        } else {
            sprintf(name, "tal_0_%i", ab);
            txt_Format(text, (unsigned)"%s%s", 0x16f2, ab + 0x16f3);
            he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), text, 0, 0x5fd, name);
        }
    }
    ai_Unk4c5870(*(int *)(rec + 4));
    he_Unk53e1f0((int *)ev);
}
