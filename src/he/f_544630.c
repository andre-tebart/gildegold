/* 0x544630, starts the duel event: the two figures (+0xac / +0xb0) must be of different parties (+9), else it is retried; clears the "own" links (+0x5c), re-uses a running duel record 0x41 of the same figure, else sets the clock, draws the slots (0x544550) and posts the event (ROUGH) */

void he_Unk544630(int ev)
{
    int a, b, r;
    int m1;

    *(int *)(ev + 0x70) = 0;
    if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
        a = gm_FindChar(*(int *)(ev + 0xac));
        b = gm_FindChar(*(int *)(ev + 0xb0));
        if (a != 0 && b != 0 && *(char *)(a + 9) != *(char *)(b + 9)) {
            if (*(int *)(a + 0x5c) != -1) {
                m1 = -1;
                cm_BeginFieldBlock(a, *(int *)(a + 4));
                cm_AddFieldDelta(4, 1, 0x5c, &m1);
                cm_SendFieldDeltas();
            }
            if (*(int *)(b + 0x5c) != -1) {
                m1 = -1;
                cm_BeginFieldBlock(b, *(int *)(b + 4));
                cm_AddFieldDelta(4, 1, 0x5c, &m1);
                cm_SendFieldDeltas();
            }
            r = (int)he_FindEvent(1, 0, 0x41);
            while (1) {
                if (r == 0) {
                    *(HETIME *)(ev + 0x52) = ai_Time;
                    gm_AddTime(ev + 0x52, 0x18, 0, 0);
                    *(int *)(ev + 0x58) = 0;
                    *(int *)(ev + 0x70) = 0;
                    *(char *)(ev + 0xb8) = 0;
                    *(short *)(ev + 0x56) = 8;
                    *(char *)(ev + 0xb9) = (char)ai_Unk5c6620(*(int *)(ev + 0xac), *(int *)(ev + 0xb0));
                    *(char *)(ev + 0xba) = 0;
                    he_Unk544550(ev, (unsigned short *)a, (unsigned short *)b);
                    *(char *)(ev + 0xbb) = 0;
                    *(int *)(ev + 0x84) = -1;
                    cm_SendExSetHE(0, ev);
                    return;
                }
                if (*(int *)(r + 0xb0) == *(int *)(a + 4))
                    break;
                r = (int)he_NextEvent();
            }
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
    }
}
