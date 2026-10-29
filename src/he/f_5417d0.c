/* 0x5417d0, the other accident report (the news 0x1630 / 0x1631 / 0x1632, +p5 + 0x1626): p6 <= 0 the 0x1632 news plus the message 0x71, else like 0x5414a0 with the scene "Unfall" (ROUGH) */

void he_Unk5417d0(int p1, unsigned short *p2, int p3, int p4, int p5, float p6)
{
    char ev[0xf8];
    char buf[2048];
    int l900, l8fc;
    int u1, news;

    if (p6 <= 0.0f) {
        if (*(char *)(p3 + 2) == 6 || *(char *)(p3 + 2) == 7) {
            txt_Format(buf, 0x1632, *p2, p4);
            he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x5ed, *(int *)(p4 + 1), -1, 0, 0);
        }
        memset(ev, 0, 0xf8);
        HM_B(ev, 4) = 0x71;
        if (p1 == 0)
            HM_I(ev, 8) = *(int *)(p3 + 4);
        else
            HM_I(ev, 8) = U_I((unsigned)&ai_CharInts + *(unsigned short *)(p1 + 8) * 0x21c);
        HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
        HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
        HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
        HM_I(ev, 0x58) = *(int *)(p2 + 2);
        HM_I(ev, 0xc) = -1;
        *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
        HM_B(ev, 0x36) = 2;
        cm_SendLongMessage(ev);
        return;
    }
    if (p6 >= unk_Unk4fc890(p2) * 0.5 || ((int (*)(int, int *, int *, int))he_Unk55eba0)(p2, &l900, &l8fc, 0) == 0 || *(int *)(p2 + 0xc2) == 0) {
        if (*(char *)(p3 + 2) != 6 && *(char *)(p3 + 2) != 7)
            return;
        news = 0x1630;
    } else {
        sv_MoveToGroup(0, 0, *p2, 0);
        if (*(int *)(*(int *)(p2 + 0xc2) + 0x28) != -1) {
            u1 = (int)evt_FindById(*(int *)(*(int *)(p2 + 0xc2) + 0x28));
            evt_Stop((EVTSCRIPT *)u1);
        }
        ai_Unk4c52e0(*(int *)(p2 + 2), l900, l8fc, 0, l8fc == -1, "Unfall");
        if (*(char *)(p3 + 2) != 6 && *(char *)(p3 + 2) != 7)
            return;
        news = 0x1631;
    }
    txt_Format(buf, news, *p2, p4, p5 + 0x1626);
    he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x5ed, *(int *)(p4 + 1), -1, 0, 0);
}
