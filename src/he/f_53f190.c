/* 0x53f190, posts the event 0x2b for every figure of kind 5 / 7 without one (+0x10 = its id; flag byte +0x5c) */

void he_PostEvent2b(void)
{
    char ev[0xf8];
    int ok = 0;
    int r, f;

    memset(ev, 0, 0xf8);
    f = gm_BuildQuery(1, 5, 7);
    do {
        if (f == 0)
            return;
        r = (int)he_FindEvent(1, 0, 0x2b);
        while (r != 0) {
            if (*(int *)(r + 0x10) == *(int *)(f + 1)) {
                ok = 1;
                goto L_next;
            }
            r = (int)he_NextEvent();
        }
        if (!ok) {
            HM_B(ev, 4) = 0x2b;
            HM_B(ev, 0x36) = 2;
            HM_I(ev, 8) = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 4);
            HM_I(ev, 0xc) = *(int *)(f + 1);
            HM_B(ev, 0x5c) = 0;
            HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
            HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
            HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
            cm_SendLongMessage(ev);
        }
L_next:
        f = gm_BuildNext();
    } while (1);
}
