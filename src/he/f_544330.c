/* 0x544330, the run handler of a message / reward event: when due (state not -1 / -2, bit 4 of +0x78 clear, no pending 0x4c3ae0 result) pays the message `+0x70` rewards (0x4c4b00, amount from 0x504ec0 / 0x504e80 times the multiplier 1 or 2 by 0x5ba6b0 of the receiver) to up to 0x10 players in the ring of 0x300 starting at +0xc4 with step +0xc8, then re-posts itself (ROUGH) */

/* ---- drafted out of address order (the big 0x53f6b0 / 0x541b40 / 0x5437c0 are still missing above this point; move the functions below into address order when those exist) ---- */

int ai_InventoryHas(void *ch, int id);
float gm_GetSkill(char *c, int skill);
float gm_Mix2(float u, float v);          /* 0x504e80, f3gm.h */
int gm_Unk505e60(unsigned id, int again);
int gm_GetWorth(unsigned short id);
void gm_Shuffle(unsigned n, int *out);
int ai_Unk5c6620();

void he_Unk544330(int ev)
{
    int sv = ev;
    int a, b, mul, cnt, pl, i, amt;
    float f;

    a = gm_FindChar(*(int *)(ev + 0xbc));
    b = gm_FindChar(*(int *)(ev + 0xc));
    mul = 1;
    if (*(int *)(ev + 0x70) == -1 || *(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
    } else if ((*(unsigned char *)(ev + 0x78) & 4) == 0 && (*(int *)(ev + 0x84) == -1 || cm_GetSlotState(*(int *)(ev + 0x84)) != 0)) {
        *(int *)(ev + 0x84) = -1;
        if (a == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            cm_SendExSetHE(-1, ev);
            return;
        }
        if (b != 0) {
            i = ai_InventoryHas(b, 0x2c5);
            mul = (i != 0) + 1;
        }
        cnt = *(int *)(ev + 0xcc);
        pl = *(int *)(ev + 0xc4);
        for (i = 0; cnt != 0 && i < 0x10; i++) {
            if (U_I((unsigned)&ai_Chars + pl * 0x21c) != -1 && U_I((unsigned)&ai_CharInts + pl * 0x21c) != *(int *)(sv + 0xbc)
                && *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + pl * 0x21c) < 10 && *(char *)(((unsigned)&ai_CharInts + 0x4) + pl * 0x21c) != 0) {
                f = gm_GetSkill((unsigned)&ai_Chars + pl * 0x21c, 4);
                amt = (int)(-gm_Mix2(*(float *)(sv + 0xd0), f) * 10.0f + 1.0f);
                cm_SendRelations(*(int *)(sv + 0xbc), U_I((unsigned)&ai_CharInts + pl * 0x21c), amt * mul, 1);
            }
            cnt = *(int *)(sv + 0xcc) - 1;
            *(int *)(sv + 0xcc) = cnt;
            pl = (*(int *)(sv + 200) + pl) % 0x300;
        }
        *(int *)(sv + 0xc4) = pl;
        *(HETIME *)(sv + 0x52) = ai_Time;
        if (gm_CompareTime(sv + 0xac, (unsigned)&ai_Time) != 1) {
            gm_AddTime(sv + 0x52, 0, 1, 0);
            *(int *)(sv + 0x84) = cm_SendExSetHE(-1, sv);
            return;
        }
        gm_AddTime(sv + 0x52, 1, 0, 0);
    }
}
