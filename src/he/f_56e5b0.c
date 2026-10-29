/* 0x56e5b0, the run of the event 0x99 (the construction run): the states -1 / -2 free the event (with the text 0x628dd4 to the player when the state is -1); paused or with the handle +0x84 still active it does nothing. State 0 waits for the notification +0xcc (0x4c3ae0 / 0x4c3b10: the building kind into +0xb8); kind 3 schedules state 6, else the text 0x628dc8 (0x4c52e0) and state 1. States 1 / 3 look up the building of +0xb8 (0x505df0, done = [0x184]+0x128); while it is not done they wait (0 / 4 minutes); when it is, the checks 0x56f2e0 / 0x56f1b0 / 0x56f3f0 pick the texts 0x4004 .. 0x401a (0x5176d0 / 0x53dda0) and state 4. State 4 the text 0x628dc8 and state 5. State 5 waits until the building is done, then waits the rest of the run time (0x21c / 0xb40 / 0x5a0 seconds by the kind +0xbc, at least 5) and frees. State 6 the domicile search 0x56f600 (state 7; else the action 0x4a63f0 and state 6), state 7 the search 0x56f740 (state 6 / 7) */

int gm_MinutesBetween();                 /* 0x4fe280 */
void cm_SendKillCharacter(int a, int b); /* 0x4c4e00 */
void ai_Unk4c5230(int a); /* 0x4c5230 */
int ai_Unk4a63f0(struct AIPER *p, int a, __int64 mask, int c, float f, int *out);   /* 0x4a63f0 (am.h); the 7-dword callers go through AI_4A63F0_7 */
int he_Unk56f1b0();                 /* 0x56f1b0 */
int he_Unk56f260();                 /* 0x56f260 */
int gv_GetTableRecord(int i, int *out);                 /* 0x4f2720 (the record of a meeting slot) */
int he_Unk56f2e0();                 /* 0x56f2e0 */
int he_Unk56f360();                 /* 0x56f360 */
int he_Unk56f3b0();                 /* 0x56f3b0 */
int he_Unk56f3f0();                 /* 0x56f3f0 */
int he_Unk56f3f0();                 /* 0x56f3f0 */
int he_Unk56f600();                 /* 0x56f600 */
int he_Unk56f740();                 /* 0x56f740 */
int gm_FindById();                 /* 0x4fea50 */
void ai_Unk4c5230(int a); /* 0x4c5230 */
void ai_Unk56f040(int *r);          /* 0x56f040 */

void he_Unk56e5b0(int ev)
{
    char text[0x100];
    char *per;
    int r, e, k;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        if (*(unsigned char *)(ev + 0x78) & 2) {
            if (gm_FindChar(HM_I(ev, 0xb8)) != 0)
                cm_SendKillCharacter(HM_I(ev, 0xb8), 1);
            if (HM_I(ev, 0x70) == -1)
                he_ShowNews(U_I((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, (char *)U_I((unsigned)&full_data_144a1a4), 1, 0x5fa,
                             (char *)(unsigned)&full_data_628dd4);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if (*(unsigned char *)(ev + 0x78) & 4)
        return;
    if (HM_I(ev, 0x84) != -1 && cm_GetSlotState(HM_I(ev, 0x84)) == 0)
        return;
    HM_I(ev, 0x84) = -1;
    per = (char *)((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
    switch (HM_I(ev, 0x70)) {
    case 0:
        if (cm_GetSlotState(HM_I(ev, 0xcc)) == 1 && (r = cm_GetSlotValue(HM_I(ev, 0xcc))) != 0) {
            HM_I(ev, 0xb8) = HM_I(r, 4);
            if (HM_I(ev, 0xbc) == 3) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
            } else if ((e = gm_BuildQuery(1, 1, HM_I(ev, 0xac))) != 0) {
                *(int *)(ev + 0xcc) = ai_Unk4c52e0(HM_I(r, 4), HM_I(e, 1), -1, 0, 1, (char *)(unsigned)&full_data_628dc8);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            } else {
                *(HETIME *)(ev + 0x52) = ai_Time;
                *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            }
        } else
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 1:
        HM_I(ev, 0xcc) = -1;
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r != 0 && HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (gm_BuildQuery(1, 1, HM_I(ev, 0xac)) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
            return;
        }
        /* falls through to case 3 */
    case 3:
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        k = gm_FindChar(HM_I(ev, 0xb4));
        e = gm_BuildQuery(1, 1, HM_I(ev, 0xb0));
        if (per != 0 && k != 0 && e != 0) {
            switch (HM_I(ev, 0xbc) - 1) {
            case 0:
                if (he_Unk56f2e0(per, k, HM_I(ev, 0xc0)) != 0) {
                    txt_Format(text, 0x4004, *(unsigned short *)k);
                    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628e28);
                    txt_Format(text, 0x4006, HM_I(ev, 0xc0) + 0x4007);
                    he_ShowNews(HM_I(k, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                } else {
                    txt_Format(text, 0x4005, *(unsigned short *)k);
                    he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, (char *)(unsigned)&full_data_628df8);
                    txt_Format(text, 0x400a);
                    he_ShowNews(HM_I(k, 4), -1, text, 1, 0x5fb, (char *)(unsigned)&full_data_628e10);
                }
                break;
            case 1:
                if (he_Unk56f1b0(per, k, HM_I(ev, 0xc0)) != 0)
                    txt_Format(text, 0x4019, 0x400c, *(unsigned short *)k, *(unsigned short *)k);
                else
                    txt_Format(text, 0x401a, 0x400c, *(unsigned short *)k, *(unsigned short *)k);
                he_ShowNews(HM_I(per, 4), -1, text, 1, 0x5fa, 0);
                break;
            case 2:
                *(HETIME *)(ev + 0x52) = ai_Time;
                HM_I(ev, 0xc4) = -1;
                HM_I(ev, 0xc8) = 0;
                *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
                break;
            case 3:
                he_Unk56f3f0(HM_I(e, 1), k);
                break;
            }
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 4:
        r = gm_FindChar(HM_I(ev, 0xb8));
        *(HETIME *)(ev + 0x52) = ai_Time;
        if (r == 0)
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        else {
            *(int *)(ev + 0xcc) = ai_Unk4c52e0(HM_I(ev, 0xb0), HM_I(ev, 0xb8), -1, 0, 0, (char *)(unsigned)&full_data_628dc8);
            *(int *)(ev + 0x84) = cm_SendExSetHE(5, ev);
        }
        return;
    case 5:
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (HM_I(HM_I(r, 0x184), 0x128) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 4, 0);
            return;
        }
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        switch (HM_I(ev, 0xbc) - 1) {
        case 1:
            k = 0x21c;
            break;
        case 2:
            k = 0xb40;
            break;
        default:
            k = 0x5a0;
            break;
        }
        k -= e;
        if (k <= 5)
            k = 5;
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, k / 60, k % 60, 0);
        cm_SendKillCharacter(HM_I(ev, 0xb8), 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 6:
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        if (e > 0x5a0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
            return;
        }
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if ((k = he_Unk56f600(per, r, HM_I(ev, 0xc4))) != 0) {
            HM_I(ev, 0xac) = HM_I(k, 0xb4);
            HM_I(ev, 0xc4) = HM_I(k, 4);
            HM_I(ev, 0xc8) = 1;
            ai_Unk4c5230(HM_I(r, 4));
            ai_Unk4c52e0(HM_I(r, 4), HM_I(ev, 0xac), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(7, ev);
        } else {
            AI_4A63F0_7(per, 0, 0xa7f4c7a0, 1, 0, 100.0f, (int *)(ev + 0xac));
            HM_I(ev, 0xc4) = -1;
            HM_I(ev, 0xc8) = 0;
            ai_Unk4c5230(HM_I(r, 4));
            ai_Unk4c52e0(HM_I(r, 4), HM_I(ev, 0xac), -1, 0, 1, (char *)(unsigned)&full_data_628dec);
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 0xf, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        }
        return;
    case 7:
        e = gm_MinutesBetween(ev + 0x44, &ai_Time);
        r = gm_FindChar(HM_I(ev, 0xb8));
        if (r == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (e > 0x5a0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
            return;
        }
        if (he_Unk56f740(r, ev, he_FindEvent(2, 0, 0x18, 1, HM_I(ev, 0xc4))) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(6, ev);
        } else {
            gm_AddTime(ev + 0x52, 0, 0, 0);
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(7, ev);
        }
        return;
    case 2:
        return;
    }
}
