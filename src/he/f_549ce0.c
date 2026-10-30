/* 0x549ce0, the init of the festival event: a random building (0x889f74 table, 0xa9 bytes each) of kind 7 is looked for (up to 0x100 tries), +0xb0 = its index, +0xb4 = a random entry of the table 0x5ed140, a message record (type 0x78) goes to every figure of kind 6 / 7, four 0x4c59f0 tasks are started (+0xbc .. +0xc8), +0xd0 = the building, the event is posted (state 0) and 0x5a91c0 is called; none found: it is posted as failed (ROUGH) */

void he_Unk549ce0(int ev)
{
    char msg[0xf8];
    int n = 0x100, idx, b, rec, t;

    *(HETIME *)(ev + 0x52) = ai_Time;
    idx = (unsigned short)util_Random(0x100);
    do {
        b = U_I((unsigned)&ai_Blds) + idx * 0xa9;
        if (*(char *)(*(unsigned char *)b * 0x24d + U_I((unsigned)&ai_BTypes)) == 7) {
            *(int *)(ev + 0xb0) = (unsigned short)util_Random(0x100);
            *(int *)(ev + 0xb4) = U_I((unsigned)&ai_Unk5ed140 + (unsigned short)util_Random(0x10) * 4);
            memset(msg, 0, sizeof(msg));
            *(int *)(msg + 0x58) = *(int *)(b + 1);
            *(int *)(msg + 0x5c) = *(int *)(b + 1);
            *(int *)(msg + 0x60) = *(int *)(ev + 0xb0);
            *(int *)(msg + 0x64) = *(int *)(ev + 0xb4);
            *(int *)(msg + 0x68) = *(int *)(ev + 0xac);
            *(int *)(msg + 0x5c + 4) = -1;
            *(int *)(msg + 0xc) = -1;
            msg[4] = 0x78;
            msg[0x36 - 0x28] = 9;
            *(HETIME *)(msg + 0x28) = ai_Time;
            for (rec = (unsigned)&ai_Chars; rec < (unsigned)((unsigned)ai_CharsV + 0x65400); rec += 0x21c) {
                if (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7) {
                    *(int *)(msg + 8) = *(int *)(rec + 4);
                    cm_SendLongMessage(msg);
                }
            }
            *(int *)(ev + 0xbc) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xe, *(int *)(b + 1), -1, 0x12, 0, 0, "RndGeselle");
            *(int *)(ev + 0xc0) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xe, *(int *)(b + 1), -1, 0x12, 0, 0, "RndGeselle");
            *(int *)(ev + 0xc4) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xe, *(int *)(b + 1), -1, 0x12, 0, 0, "RndGeselle");
            *(int *)(ev + 0xc8) = ((int (*)(int, int, int, int, int, int, const char *))ai_Unk4c59f0)(0xe, *(int *)(b + 1), -1, 0x12, 0, 0, "RndGeselle");
            *(int *)(ev + 0xd0) = *(int *)(b + 1);
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            ai_Unk5a91c0();
            return;
        }
        n--;
        idx = (idx + 1) & 0xff;
    } while (n != 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
