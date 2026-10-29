/* 0x53a250, per-player effect update of figure `i` (by the kind byte at +2; ROUGH) */

#define PL(i) ((unsigned)&ai_Chars + (i) * 0x21c)
float gm_Unk504510(int n, int *out);

void main_Unk53a250(int i)
{
    int base = i * 0x21c;
    unsigned char kind = *(unsigned char *)(((unsigned)&ai_Chars + 0x2) + base);
    int one;
    int pr, iv;
    float f;
    int ev[0x3e];
    char txt[256];

    if (kind != 6) {
        if (kind == 7 || kind > 9) {
            if (kind == 6 || kind == 7)
                goto L404;
        } else {
            one = 1;
            if (U_I((unsigned)&ai_Unk770cec + base) != 0 || kind == 5 || kind == 1 || kind == 2 || kind == 4) {
                cm_BeginFieldBlock(PL(i), U_I((unsigned)&ai_CharInts + base));
                cm_AddFieldValues(4, 1, 0x174, &one);
                cm_SendFieldDeltas();
            }
            if (*(char *)(((unsigned)&ai_CharInts + 0x160) + base) != 0) {
                pr = gm_TitlesLeft(*(char *)(((unsigned)&ai_CharInts + 0x160) + base));
                iv = he_FindEvent(2, 2, U_I((unsigned)&ai_Chars + base), 0, 0x24);
                U_I(((unsigned)&ai_Unk770cec + 0x8) + base) += 1;
                f = gm_Unk504510(i, ev);
                U_I(((unsigned)&ai_Unk770cec + 0x8) + base) -= 1;
                if (iv == 0 && pr != 0 && pr < 6 && 1.0 <= f && U_I(((unsigned)&ai_Selected + 0xc) + base) > -1) {
                    memset(ev, 0, 0xf8);
                    ev[2] = U_I((unsigned)&ai_CharInts + base);
                    ev[0x1f] = 0;
                    ev[0x1d] = 0;
                    ((char *)ev)[0x00 + 4] = 0x24;
                    ev[3] = -1;
                    ((char *)ev)[0x36] = 0x11;
                    ev[0x1e] = 1;
                    ev[0x1c] = ev[2];
                    cm_SendLongMessage(ev);
                    sprintf(txt, "Meisterpr\xfc" "fung started: player_id:%i beruf:%i grad:%i", U_I((unsigned)&ai_CharInts + base), *(char *)(((unsigned)&ai_CharInts + 0x160) + base), pr + 1);
                    cm_SendMoneyDelta(U_I((unsigned)&ai_CharInts + base), -4);
                }
            }
        }
        return;
    }
L404:
    one = 1;
    cm_BeginFieldBlock(PL(i), U_I((unsigned)&ai_CharInts + base));
    cm_AddFieldValues(4, 1, 0x174, &one);
    cm_SendFieldDeltas();
}
