/* 0x4fa580, the daily upkeep of the characters (kind 0, active, with a building at +0x170): rate = 0x5085f0 (0xe - the record 5 rate, 0x6f1bc5); a character whose 0x509e60 is negative: with a master (+0x16c) 0x4c4b00 (-10) tells the master player; without a master and partner (+0x5c = -1) and a debt above twice the rate 0x4fca40 (i, -5000); else (or not negative) a building of kind 1 gets 0x4c4830 (the rate) for the player. Flushes the messages every 0x40. ROUGH */

struct AMTKIND; int amt_GetKind(int k, struct AMTKIND *out);                   /* 0x4e6f80 */
float gm_AddHealth();                  /* 0x4fca40 */
float gm_GetSkill(char *c, int skill);                     /* 0x504ec0 */
float gm_Mix(int a, int b, int c);                     /* 0x504e20 */
void ai_Unk4c3e70(char *p);                     /* 0x4c3e70 */
int ai_Unk4c3eb0(unsigned char k, float v);                     /* 0x4c3eb0 */
void ai_Unk4c4a20(void);                     /* 0x4c4a20 */
void unk_Unk4fb0e0();                   /* 0x4fb0e0 */
int gm_FindRichest(short *id, int *rate);
#define I50A640 ((int (*)())gm_GetWorth)

void unk_Unk4fa580(void)
{
    char *p;
    char *b;
    char *rec;
    int rate, i, v, d;
    int r[9];

    rec = (char *)gm_PlayerSeat(1);
    gv_GetTableRecord(5, r);
    rate = gm_PlayerFactor(0xe - r[6], ai_OwnPlayer);
    ai_Unk6f1b98 = 0;
    for (i = 0; i < 0x300; i++) {
        p = ai_Chars[i];
        if (ai_Unk6f1b98 > 0x40) {
            sv_PumpNet();
            ai_Unk6f1b98 = 0;
        }
        if (*(short *)p != -1 && p[8] != 0 && p[2] == 0 && *(unsigned char **)(p + 0x170) != 0) {
            b = ai_BTypes + **(unsigned char **)(p + 0x170) * 0x24d;
            v = gm_GetGoodsValue(p);
            if (v < 0) {
                if (*(int *)(p + 0x16c) != 0) {
                    cm_SendRelations(*(int *)(ai_Chars[*(unsigned short *)(*(int *)(p + 0x16c) + 0x27)] + 4), *(int *)(p + 4), -10, 1);
                    ai_Unk6f1b98++;
                    goto A;
                }
                d = v < 0 ? -v : v;
                if (*(int *)(p + 0x5c) != -1 || d <= rate * 2)
                    goto A;
                gm_AddHealth(i, -5000);
            } else {
            A:
                if (*b != 1)
                    continue;
                cm_SendMoveGoodsUnchecked(*(int *)(rec + 2), *(int *)(p + 4), 0, rate);
            }
            ai_Unk6f1b98++;
        }
    }
    ai_Unk6f1b98 = 0;
}
