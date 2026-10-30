/* 0x544840, a Ghidra miss (disassembled; 3584 B): the run handler of the duel event (see 0x544630): a = +0xac, b = +0xb0 are the duellists (of different parties); the state (+0x70) 0 = the preparation (0x5c6530 decides: -0x405 / -0x406 = one of them gives up (news 0x42f6 / 0x42f7), -0x403 = the followers are told (news 0x1ceb ..), 0 = the same for the other kind, else another round: +0xb8 counts up to +0xb9 rounds, a round of a kind 6 figure says 0x1cf3 / 0x1cf4 and draws new slots 0x544550), 1 = the duel is over: both link each other (+0x5c, +0x50), 4 = clock only, 5 = the result: when they are each other's opponent the relation changes by 0x7f - table[a][b] (0x7e5360, 0x300 per row) and a message record of 0x114 bytes (type 6) is posted (0x4c4fa0), else the news 0x1ceb ..; a free (the state -1 / -2) clears the flag 0x40000 of the figure without a partner (ROUGH) */

unsigned int ai_Unk5c6530(int param_1, int param_2);
int unk_Unk4f25d0();
int gv_EntryIndex(char *r, int *e);
int unk_Unk4f0900();
void ai_Unk4c4fa0(void *p);
void cm_SendRelations(int a, int b, int c, int d);
void ai_Unk4c4ac0(int a, int b, int c);

#define HE_W(p) (*(unsigned short *)(p))
#define HE_END(b) ((b) + strlen(b))
/* the duel text of the news 0x1ceb .. 0x1cf2: `c1` the figure of +0x60 of b (with a rank above 0 and b not of kind 5), `c2` the one of +0x5c */
#define HE_DUEL_TEXT_FULL(buf, a, b, c1, c2) \
    txt_Format(buf, 0x1ceb, HE_W(c1), HE_W(a)); \
    txt_Format(HE_END(buf), 0x1cee); \
    txt_Format(HE_END(buf), 0x1cf2, HE_W(b)); \
    txt_Format(HE_END(buf), 0x1cef, HE_W(c2)); \
    txt_Format(HE_END(buf), 0x1cf0, HE_W(c1))
#define HE_DUEL_TEXT_SHORT(buf, a, b, c2) \
    txt_Format(buf, 0x1ceb, HE_W(b), HE_W(a)); \
    txt_Format(HE_END(buf), 0x1cee); \
    txt_Format(HE_END(buf), 0x1cf1); \
    txt_Format(HE_END(buf), 0x1cef, HE_W(c2)); \
    txt_Format(HE_END(buf), 0x1cf0, HE_W(b))

void he_Unk544840(int ev)
{
    char buf1[0x200];
    char buf3[0x400];
    char msg[0x114];
    int a, b, c1, c2, r, st, who, pb, pa;
    int x1, x2;

    a = gm_FindChar(*(int *)(ev + 0xac));
    b = gm_FindChar(*(int *)(ev + 0xb0));
    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            if (b != 0 && (*(unsigned *)(b + 0x1c8) & 0x40000) != 0 && a == 0)
                cm_SendSetField(*(int *)(b + 4), 0x1c8, 4, 0, 0x40000);
            else if (a != 0 && (*(unsigned *)(a + 0x1c8) & 0x40000) != 0 && b == 0)
                cm_SendSetField(*(int *)(a + 4), 0x1c8, 4, 0, 0x40000);
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (a == 0 || b == 0 || *(char *)(a + 9) == *(char *)(b + 9)) {
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    }
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    *(int *)(ev + 0x84) = -1;
    switch (st) {
    case 0:
        r = ai_Unk5c6530(*(int *)(ev + 0xac), *(int *)(ev + 0xb0));
        if (r == -0x405 || r == -0x406) {
            *(HETIME *)(ev + 0x52) = ai_Time;
            gm_AddTime(ev + 0x52, 0, 2, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            txt_Format(buf3, r == -0x405 ? 0x42f6 : 0x42f7, HE_W(b));
            he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf3, 1, 0x5f4, (char *)0);
            return;
        }
        if (r != -0x400 && r != -0x401 && r != -0x402) {
            if (r == -0x403) {
                if (*(char *)(a + 2) == 6) {
                    c1 = gm_FindChar(*(int *)(b + 0x60));
                    c2 = gm_FindChar(*(int *)(b + 0x5c));
                    if (c1 != 0 && *(unsigned char *)(c1 + 8) > 0 && *(char *)(b + 2) != 5) {
                        HE_DUEL_TEXT_FULL(buf1, a, b, c1, c2);
                    } else {
                        HE_DUEL_TEXT_SHORT(buf1, a, b, c2);
                    }
                    he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf1, 1, 0x5f4, (char *)0);
                }
            } else if (r == 0) {
                if (*(char *)(a + 2) == 6) {
                    c1 = gm_FindChar(*(int *)(b + 0x60));
                    if (c1 != 0 && *(unsigned char *)(c1 + 8) > 0 && *(char *)(b + 2) != 5) {
                        txt_Format(buf1, 0x1ceb, HE_W(c1), HE_W(a));
                        txt_Format(HE_END(buf1), 0x1cec);
                        txt_Format(HE_END(buf1), 0x1cf2, HE_W(b));
                        txt_Format(HE_END(buf1), 0x1ced);
                        txt_Format(HE_END(buf1), 0x1cf0, HE_W(c1));
                    } else {
                        txt_Format(buf1, 0x1ceb, HE_W(b), HE_W(a));
                        txt_Format(HE_END(buf1), 0x1cec);
                        txt_Format(HE_END(buf1), 0x1cf1);
                        txt_Format(HE_END(buf1), 0x1ced);
                        txt_Format(HE_END(buf1), 0x1cf0, HE_W(b));
                    }
                    he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf1, 1, 0x5f4, (char *)0);
                }
                *(HETIME *)(ev + 0x52) = ai_Time;
            } else {
                if (r == 1) {
                    *(char *)(ev + 0xb9) = (char)ai_Unk5c6620(*(int *)(ev + 0xac), *(int *)(ev + 0xb0));
                    *(char *)(ev + 0xb8) = *(char *)(ev + 0xb8) + 1;
                }
                if (*(char *)(ev + 0xb8) >= *(char *)(ev + 0xb9)) {
                    *(HETIME *)(ev + 0x52) = ai_Time;
                    gm_AddTime(ev + 0x52, 0, 2, 0);
                    *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
                    return;
                }
                if (*(char *)(a + 2) == 6) {
                    txt_Format(buf1, 0x1cf3);
                    txt_Format(HE_END(buf1), 0x1cf4, HE_W(b), 0x5d, *(char *)(ev + 0xb9) - *(char *)(ev + 0xb8), 0x5d);
                    he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf1, 0, 0x5f4, (char *)0);
                } else if (*(char *)(a + 2) == 5) {
                    *(char *)(ev + 0xb8) = *(char *)(ev + 0xb8) + (char)util_Random(2) + 1;
                } else if ((short)util_Random(4) == 0) {
                    *(char *)(ev + 0xb8) = *(char *)(ev + 0xb8) + 1;
                }
                *(HETIME *)(ev + 0x52) = ai_Time;
                gm_AddTime(ev + 0x52, 0x18, 0, 0);
                *(short *)(ev + 0x56) = 8;
                *(char *)(ev + 0xba) = 1;
                he_Unk544550(ev, (unsigned short *)a, (unsigned short *)b);
                *(int *)(ev + 0x84) = -1;
                cm_SendExSetHE(0, ev);
                return;
            }
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 1:
        cm_BeginGroup((char *)((unsigned)&ai_Unk625820 + 0x11d8));
        pb = b + 4;
        cm_BeginFieldBlock(b, *(int *)(b + 4));
        pa = a + 4;
        cm_AddFieldDelta(4, 1, 0x5c, pa);
        cm_AddFieldDelta(2, 1, 0x50, a + 0x50);
        cm_SendFieldDeltas();
        cm_BeginFieldBlock(a, *(int *)pa);
        cm_AddFieldDelta(4, 1, 0x5c, pb);
        cm_SendFieldDeltas();
        cm_SendSetField(*(int *)pb, 0x1c8, 4, 0x40000, 0);
        cm_SendSetField(*(int *)pa, 0x1c8, 4, 0x40000, 0);
        cm_EndGroup((char *)((unsigned)&ai_Unk625820 + 0x11d8));
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(5, ev);
        return;
    case 4:
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 5:
        if (*(int *)(a + 0x5c) == *(int *)(b + 4) && *(int *)(b + 0x5c) == *(int *)(a + 4)) {
            cm_SendRelations(*(int *)(a + 4), *(int *)(b + 4), 0x7f - *(signed char *)((unsigned)&ai_Relations + HE_W(a) * 0x300 + HE_W(b)), 1);
            cm_SendRelations(*(int *)(b + 4), *(int *)(a + 4), 0x7f - *(signed char *)((unsigned)&ai_Relations + HE_W(b) * 0x300 + HE_W(a)), 1);
            if (*(char *)(a + 2) == 6 || *(char *)(a + 2) == 5) {
                who = a;
                if (*(char *)(a + 9) != 1)
                    who = b;
                txt_Format(buf3, 0x1d31, HE_W(b));
                he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf3, 0, 0x5f4, (char *)0);
                ai_Unk4c4ac0(*(int *)(who + 4), 0x7c, 1.1f - *(float *)(who + 0x7c));
            }
            if (unk_Unk4f25d0(b, &x1, &x2) != 0)
                unk_Unk4f0900(0, x1, *(int *)x2, gv_EntryIndex(*(int *)(x1 + 0x65), x2));
            memset(msg, 0, sizeof(msg));
            *(int *)(msg + 0x34) = *(int *)(ev + 4);
            *(int *)(msg + 0x38) = *(int *)(b + 4);
            *(HETIME *)(msg + 0x18) = ai_Time;
            *(int *)(msg + 0xc) = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
            msg[8] = 6;
            msg[0x26] = (char)0xb2;
            *(int *)(msg + 0x14) = -1;
            msg[0x30] = 2;
            gm_AddTime(msg + 0x18, 2, 0, 0);
            *(int *)(msg + 0x1e) = 0;
            if (*(unsigned short *)(msg + 0x1c) > 0x16) {
                *(int *)(msg + 0x18) += 1;
                *(unsigned short *)(msg + 0x1c) = 0xb;
            } else if (*(unsigned short *)(msg + 0x1c) < 7) {
                *(unsigned short *)(msg + 0x1c) = 0xb;
            }
            *(HETIME *)(ev + 0x52) = *(HETIME *)(msg + 0x18);
            *(char *)(ev + 0xbb) = 1;
            *(int *)(ev + 0x84) = -1;
            cm_SendExSetHE(4, ev);
            ai_Unk4c4fa0(msg);
            return;
        }
        if (*(char *)(a + 2) == 6) {
            c1 = gm_FindChar(*(int *)(b + 0x60));
            c2 = gm_FindChar(*(int *)(b + 0x5c));
            if (c1 != 0 && *(unsigned char *)(c1 + 8) > 0 && *(char *)(b + 2) != 5) {
                HE_DUEL_TEXT_FULL(buf1, a, b, c1, c2);
            } else {
                HE_DUEL_TEXT_SHORT(buf1, a, b, c2);
            }
            he_ShowNews(*(int *)(a + 4), *(int *)(a + 4), buf1, 1, 0x5f4, (char *)0);
        }
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    default:
        return;
    }
}
