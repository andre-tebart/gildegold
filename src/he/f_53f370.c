/* 0x53f370, posts the event 0x98 for every building of kind 0x1e (workshop; +0xc = its id) that has none and a record */

void he_Unk53f370(void)
{
    char ev[0xf8];
    int f, r, o;

    f = gm_BuildQuery(1, 5, 0x1e);
    while (f != 0) {
        r = (int)he_FindEvent(2, 0, 0x98, 3, *(int *)(f + 1));
        if (r == 0 && (o = ahm_OwnerChar(f)) != 0) {
            memset(ev, 0, 0xf8);
            HM_B(ev, 4) = 0x98;
            HM_I(ev, 8) = *(int *)(o + 4);
            HM_I(ev, 0xc) = *(int *)(f + 1);
            HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
            HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
            HM_B(ev, 0x36) = 2;
            HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
            *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
            cm_SendLongMessage(ev);
        }
        f = gm_BuildNext();
    }
}
