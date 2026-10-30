/* 0x544070, a Ghidra miss (disassembled): the init of the tribute / levy event (record fields +0xbc = the collector's id, +0xc4 = a random start slot of 0x300, +0xc8 = a random entry of the table 0x5ed100, +0xcc = 0x300 players left, +0xd0 = the 0x504ec0 amount): with more than 4 running events of type 0x35 the event is posted again (and 0x5a8d00 called), else the clocks are set, the message 0x4f2e30 (3, 1, ..) is sent and, by the 0x15c ability (0x5ba6b0) of the collector, 0x5a8d80 / 0x5a8d40 is called (ROUGH) */

float gm_GetSkill(char *c, int skill);
void gm_SendMoneyDelta(int a, int b);
int unk_Unk4f2e30();
void ai_Unk5a8d00(int param_1);
void ai_Unk5a8d40(int param_1);
void ai_Unk5a8d80(int param_1, int param_2);

void he_Unk544070(int ev)
{
    int n = 0;
    int pl, r, t, t2, idx;

    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        pl = gm_FindChar(*(int *)(ev + 0xbc));
        if (pl == 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        r = (int)he_FindEvent(1, 0, 0x35);
        if (r != 0) {
            do {
                n++;
            } while ((r = (int)he_NextEvent()) != 0);
            if (n > 4) {
                *(HETIME *)(ev + 0x52) = ai_Time;
                *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
                ai_Unk5a8d00((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
                return;
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(HETIME *)(ev + 0x44) = ai_Time;
        *(HETIME *)(ev + 0xac) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        gm_AddTime(ev + 0xac, 0x18, 0, 0);
        idx = *(unsigned short *)(ev + 8) * 0x21c;
        gm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + idx), -1);
        t = U_I(((unsigned)&ai_Unk770cec + 0x4) + idx);
        if (t != 0)
            t2 = *(int *)(t + 1);
        else
            t2 = -1;
        unk_Unk4f2e30(3, 1, U_I((unsigned)&ai_CharInts + idx), *(int *)(ev + 0xbc), t2);
        *(int *)(ev + 0xc4) = util_Random(0x300) & 0xffff;
        *(int *)(ev + 0xc8) = U_I((unsigned)&ai_Unk5ed100 + (util_Random(0x10) & 0xffff) * 4);
        *(float *)(ev + 0xd0) = gm_GetSkill((unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c, 4);
        *(int *)(ev + 0xcc) = 0x300;
        if (ai_InventoryHas(pl, 0x15c) != 0) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            ai_Unk5a8d80(pl, (unsigned)&ai_Chars + *(unsigned short *)(ev + 8) * 0x21c);
            return;
        }
        ai_Unk5a8d40(pl);
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
