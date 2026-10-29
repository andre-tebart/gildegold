/* 0x5414a0, an accident of the figure p2 (in the event p4): when a (float, > 0) is below half of its health 0x4fc890 and there is a place to carry it (0x55eba0), the scene "Unfall" with the news 0x1baf, else the news 0x1bae / 0x4149 (grave) / 0x1bb0 / 0x414a by the sign of p5 and p6 (ROUGH) */

void he_Unk5414a0(int p1, unsigned short *p2, int p3, int p4, float p5, int p6)
{
    char ev[0xf8];
    char buf[2048];
    int l900, l8fc;
    int u1;
    char *t;

    if (p5 > 0.0f) {
        if ((float)p5 < unk_Unk4fc890(p2) * 0.5 && ((int (*)(int, int *, int *, int))he_Unk55eba0)(p2, &l900, &l8fc, 0) != 0 && *(int *)(p2 + 0xc2) != 0) {
            sv_MoveToGroup(0, 0, *p2, 0);
            if (*(int *)(*(int *)(p2 + 0xc2) + 0x28) != -1) {
                u1 = (int)evt_FindById(*(int *)(*(int *)(p2 + 0xc2) + 0x28));
                evt_Stop((EVTSCRIPT *)u1);
            }
            cm_SendSetField(*(int *)(p2 + 2), 0x1c8, 4, 0x800, 0);
            ai_Unk4c52e0(*(int *)(p2 + 2), l900, l8fc, 0, l8fc == -1, "Unfall");
            if (*(char *)(p3 + 2) != 6 && *(char *)(p3 + 2) != 7)
                return;
            txt_Format(buf, 0x1baf, *p2, p4);
            he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x60a, *(int *)(p4 + 1), -1, 0, "_NACHRICHTEN_HS_61");
            return;
        }
        if (*(char *)(p3 + 2) != 6 && *(char *)(p3 + 2) != 7)
            return;
        if (*(char *)(p2 + 1) == 0x14) {
            txt_Format(buf, 0x4149, *p2, p4);
            t = "_NACHRICHTEN_FRIEDHOF_04";
            he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x60a, *(int *)(p4 + 1), -1, 0, t);
            return;
        }
        txt_Format(buf, 0x1bae, *p2, p4);
        t = "_NACHRICHTEN_HS_61";
    } else {
        sv_MoveToGroup(0, 0, *p2, 0);
        if (*(int *)(p2 + 0xc2) != 0 && (u1 = *(int *)(*(int *)(p2 + 0xc2) + 0x28)) != -1) {
            u1 = (int)evt_FindById(u1);
            evt_Stop((EVTSCRIPT *)u1);
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
        if (*(char *)(p3 + 2) != 6 && *(char *)(p3 + 2) != 7)
            return;
        if (p6 != 0)
            return;
        if (*(char *)(p2 + 1) != 0x14) {
            txt_Format(buf, 0x1bb0, *p2, p4);
            t = "_NACHRICHTEN_HS_62";
            he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x60a, *(int *)(p4 + 1), -1, 0, t);
            return;
        }
        txt_Format(buf, 0x414a, *p2, p4);
        t = "_NACHRICHTEN_FRIEDHOF_05";
    }
    he_MessageBoxQuickjump(*(int *)(p3 + 4), *(int *)(p4 + 1), buf, 0, 0x60a, *(int *)(p4 + 1), -1, 0, t);
}
