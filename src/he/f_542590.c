/* 0x542590, a Ghidra miss (disassembled): the run handler of the tavern event (state +0x70: 0 = pick the next step by the owner / the 0x1b5 record of the building (+0x10), 1 = wait 1 .. 2 units, 2 = the tavern record's helper figure (+0x1e) is sent off with the news 0x19a7 and paid); the next due time is always 0x18 units (state word 7) or 1 unit away (ROUGH) */

int ai_Unk493be0();
int unk_Unk4fae50();
void ai_Unk5a8e10(char *param_1, char *param_2, int param_3);

void he_Unk542590(int ev)
{
    int bld = 0;
    int r, f, idx, st;
    int out[2];
    char buf[0x100];
    unsigned char z = 0;
    unsigned char ff = 0xff;
    int m1 = -1;
    int v69 = 0;

    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    gm_FindById(&bld, 0, 0, *(int *)(ev + 0x10));
    if (bld == 0) {
        cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(unsigned short *)(bld + 0x27) == 0xffff) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 7;
        cm_SendExSetHE(0, ev);
        return;
    }
    r = gm_Query(*(int *)(bld + 0x5d), 1, 0, 0x1b5);
    st = *(int *)(ev + 0x70);
    if (st == 0) {
        idx = *(unsigned short *)(bld + 0x27) * 0x21c;
        if (*(char *)(((unsigned)&ai_Chars + 0x2) + idx) != 6 && *(char *)(((unsigned)&ai_Chars + 0x2) + idx) != 7) {
            if (r != 0 && ai_Unk493be0((unsigned)&ai_Chars + idx, r) != 0) {
                gm_AddTime(ev + 0x52, 1, 0, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
                return;
            }
            gm_AddTime(ev + 0x52, 0x18, 0, 0);
            *(short *)(ev + 0x56) = 7;
            *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
            return;
        }
        if (r != 0 && *(char *)(r + 0x22) != 0) {
            gm_AddTime(ev + 0x52, 1, 0, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 7;
        cm_SendExSetHE(0, ev);
        return;
    }
    if (st == 1) {
        gm_AddTime(ev + 0x52, (util_Random(2) & 0xffff) + 1, 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    }
    if (st == 2) {
        if (r != 0 && *(char *)(r + 0x22) != 0) {
            f = gm_FindChar(*(int *)(r + 0x1e));
            if (f != 0)
                ai_Unk5a8e10((unsigned)&ai_Chars + *(unsigned short *)(bld + 0x27) * 0x21c, f, bld);
            cm_BeginFieldBlock(r, *(int *)(r + 2));
            cm_AddFieldDelta(1, 1, 0x22, &z);
            cm_AddFieldDelta(1, 1, 0x1c, &ff);
            cm_AddFieldDelta(1, 1, 0x1d, &z);
            cm_AddFieldDelta(4, 1, 0x1e, &m1);
            cm_SendFieldDeltas();
            if (unk_Unk4fae50(bld, out) != 0) {
                idx = *(unsigned short *)(bld + 0x27) * 0x21c;
                if (*(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 6 || *(char *)(((unsigned)&ai_Chars + 0x2) + idx) == 7) {
                    txt_Format(buf, 0x19a7, out[0], out[1]);
                    he_ShowNews(U_I((unsigned)&ai_CharInts + idx), -1, buf, 0, 0x5ff, (char *)(unsigned)&ch_EmptyName);
                }
                cm_SendMoveGoods(U_I((unsigned)&ai_CharInts + *(unsigned short *)(bld + 0x27) * 0x21c), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, out[1]);
                cm_BeginFieldBlock(f, *(int *)(f + 1));
                cm_AddFieldDelta(4, 1, 0x69, &v69);
                cm_SendFieldDeltas();
            }
        }
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(short *)(ev + 0x56) = 7;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
