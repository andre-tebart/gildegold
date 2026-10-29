/* 0x540c90, from day 8, in 1 of 4 frames: when no event 0x67 exists, posts one for a random one of the figures of the player of 0x6f1bf8 (kinds 0x20 / 0x1e / 0x1f): +0x58 = the figure, +0x5c = an amount of the 0x4f2720 table plus its +0x69 value (ROUGH) */

float util_RandomF();
int gv_GetTableRecord(int i, int *out);
int cm_AddFieldDelta(int size, int n, short field, void *v);
void cm_BeginFieldBlock(char *o, int id);
void cm_SendFieldDeltas(void);
void txt_Format(char *out, int id0, ...);
int gm_EquipmentValue(unsigned char *b, int unused);
int cm_SendExSetHE(); /* @original: the callers here push the key as an int and read the return (byte-matched he_Unk541200 / he_Unk543020 / he_Unk548110 / he_Unk5725b0), against cm.h's void (unsigned char, char *) view */

void he_Unk540c90(void)
{
    char ev[0xf8];
    int cand[3];
    float tab[9];
    int a, b, c, n, f;
    unsigned short pick;
    int amt;

    if (U_I((unsigned)&ai_Time) > 7) {
        if ((short)util_Random(4) == 0) {
            if (he_FindEvent(1, 0, 0x67) == 0) {
                a = gm_BuildQuery(1, 0, 0x20);
                b = gm_BuildQuery(1, 0, 0x1e);
                c = gm_BuildQuery(1, 0, 0x1f);
                n = 0;
                if (a != 0 && *(short *)(a + 0x27) == **(short **)(unsigned)&ai_PlayerChar) {
                    n = 1;
                    cand[0] = a;
                }
                if (b != 0 && *(short *)(b + 0x27) == **(short **)(unsigned)&ai_PlayerChar) {
                    cand[n] = b;
                    n++;
                }
                if (c != 0 && *(short *)(c + 0x27) == **(short **)(unsigned)&ai_PlayerChar) {
                    cand[n] = c;
                    n++;
                }
                if (n != 0) {
                    pick = util_Random(n);
                    gv_GetTableRecord(7, tab);
                    f = cand[pick];
                    amt = (int)tab[0];
                    memset(ev, 0, 0xf8);
                    HM_I(ev, 0x28) = U_I((unsigned)&ai_Time);
                    HM_I(ev, 0x2c) = U_I((unsigned)&ai_TimeHour);
                    HM_I(ev, 0x30) = U_I(((unsigned)&ai_Time + 0x8));
                    HM_B(ev, 4) = 0x67;
                    *(short *)(ev + 0x34) = *(short *)((unsigned)&ai_Time + 0xc);
                    HM_I(ev, 8) = *(int *)(*(int *)(unsigned)&ai_PlayerChar + 2);
                    HM_I(ev, 0xc) = *(int *)(f + 1);
                    HM_I(ev, 0x58) = *(int *)(f + 1);
                    amt = *(int *)(f + 0x69) + amt;
                    HM_I(ev, 0x5c) = amt;
                    if (amt < 1) {
                        gm_EquipmentValue(f, 100);
                        HM_I(ev, 0x5c) = (int)util_RandomF();
                    }
                    HM_B(ev, 0x36) = 2;
                    cm_SendLongMessage(ev);
                }
            }
        }
    }
}
