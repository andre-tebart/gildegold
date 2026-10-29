/* 0x547750, a Ghidra miss (disassembled): the run handler of the insult event (the figures +0xac a and +0xb0 b, flag 0x400 of +0x1c8 = in a quarrel): sets the clock to 0x18 units with +0x56 = 6, +0x58 = 0x1e; with the relation 0x4f2e30 (4 ..) it is decided: when b is of kind 6 / 7 a message record of 0x f8 bytes (type 0x46, 0x4fe310) is posted and the event goes on (state 1), else a chance (0x504e20 + 0x42ec20 against 1.0) lets a pay (the news 0x1d81 / 0x1d83, the ability 0x4ea550, the relation -0x14, the same as 0x5472d0) or sets the flags and goes to the state 2 (ROUGH) */

void he_Unk547750(int ev)
{
    char text[0x100];
    char msg[0xf8];
    int a1[6];
    int a2[8];
    int a, b, x, r, bx;
    float f;
    int amt;

    *(int *)(ev + 0x84) = -1;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0x18, 0, 0);
    *(short *)(ev + 0x56) = 6;
    *(int *)(ev + 0x58) = 0x1e;
    if ((*(unsigned char *)(ev + 0x78) & 2) == 0)
        return;
    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    if (a == 0 || b == 0) {
        if ((*(unsigned *)(a + 0x1c8) & 0x400) != 0 || (*(unsigned *)(b + 0x1c8) & 0x400) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
    }
    x = -1;
    r = *(int *)(a + 0x170);
    if (r == 0)
        r = *(int *)(b + 0x170);
    if (r != 0)
        x = *(int *)(r + 1);
    unk_Unk4f2e30(4, 1, *(int *)(a + 4), *(int *)(b + 4), x);
    if (*(char *)(b + 2) == 6 || *(char *)(b + 2) == 7) {
        memset(msg, 0, sizeof(msg));
        *(int *)(msg + 8) = *(int *)(b + 4);
        *(int *)(msg + 0xc) = *(int *)(b + 4);
        msg[4] = 0x46;
        msg[0x36] = 9;
        *(int *)(msg + 0x58) = *(int *)(ev + 4);
        gm_AddTime2(msg + 0x24, *(unsigned short *)b, 6, 0, 0);
        cm_SendLongMessage(msg);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 6;
        *(int *)(ev + 0x58) = 0x1e;
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0x400, 0);
        cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0x400, 0);
        return;
    }
    f = gm_Mix(b, a, 3);
    if (!(util_RandomF() + f < 1.0f)) {
        cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0x400, 0);
        cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0x400, 0);
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 6;
        *(int *)(ev + 0x58) = 0x1e;
        if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 7) {
            txt_Format(text, 0x1d83, *(unsigned short *)b, ev + 0x52, *(unsigned short *)b);
            he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), text, 0, 0x5fa, "_DUELL_INTRO_EVENT_DUELLANTEN_HS_00");
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    }
    bx = 0;
    if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 7) {
        txt_Format(text, 0x1d81, *(unsigned short *)b, *(unsigned short *)b, (*(char *)(b + 9) != 0) + 0x1d68);
        he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), text, 0, 0x5fa, (char *)0);
    }
    if (*(char *)(b + 0x166) != 0) {
        if (amt_FindOffice(b, a1, a2) != 0)
            bx = a1[1];
        else
            bx = (unsigned short)util_Random(3) + 1;
    }
    amt = -9 - (int)(gm_GetSkill(a, 4) * 8.0f);
    amt -= (unsigned short)util_Random(8);
    amt_ChangeDistrictOpinion(b, bx, amt);
    cm_SendRelations(*(int *)(b + 4), *(int *)(a + 4), -0x14, 1);
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 2, 0);
    *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
}
