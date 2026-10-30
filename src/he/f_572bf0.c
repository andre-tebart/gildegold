/* 0x572bf0, the run of the event 0x93 (the working of a building 0x1d): -2 / -1 free. State 0: the building 0x501190 (1, 5, 0x1d) with a player word; the value 0x5092e0 (+0x15) * 0x5eb950 + 0x5eb94c; the characters +0x770b80 (0x21c) whose +0x16c is the building are collected; each one: 0x42ec20 over the value -> when over: 0x587c40 (char, 1) for the kinds 4 / 0x8c / 0x8d without +0x1b4 bit 0; when under and 0x42ec20 <= 0x5eb7e8: 0x50c520 (building +0x27, char) against -(0x42ec00 (0x4a)), then the message 0x94 (+4 = 0x94, +8 = the player word of the event, +0xc = building +4, +0x28 clock, +0x36 = 1, +0x58 = event +4) via 0x4c4b70. Tail: due now + 0x14 + 0x42ec00 (0x14), state + 1. State 1: the same collection with 0x50a810 / 0x501b50, a random pick 0x42ec00 (count), 0x5089c0, 0x4c4880, 0x5190a0 and the texts 0x414b / 0x229, the message 0x53dee0 (0x5f2, 0x629000), 0x501030 / 0x4ffcc0; then the tail. State 2: due now + 0x1e, state + 1. State 3: due now + 0x1e, state 0. ROUGH */

int gm_Opinion(int a, int b); /* 0x50c520 */
unsigned char * gm_BuildNext(void); /* 0x501030 */
int gm_BuildingMainObject(unsigned char *b); /* 0x501b50 */
unsigned short *gm_QueryNext(void); /* 0x4ffcc0 */
void ai_EndCharExtra(char *o, int f); /* 0x587c40 */
float gm_GoodsPrice(unsigned id, unsigned char p); /* 0x5089c0 */
int ls_GetLanguage();                 /* 0x5190a0 */

void he_Unk572bf0(int ev)
{
    char *chars[0x20];
    char msg[0xf8];
    char text[0x100];
    char *per, *bld, *ch;
    float f;
    int i, n, cnt, v, k;

    switch (HM_I(ev, 0x70)) {
    case -2:
    case -1:
        he_Unk53e1f0((int *)ev);
        return;
    case 0:
        bld = (char *)gm_BuildQuery(1, 5, 0x1d);
        if (bld == 0)
            goto tail;
        if (*(unsigned short *)(bld + 0x27) != 0xffff) {
            n = gm_BuildingSkill(bld, 0x15, 1);
            f = (float)n * (0.00249999994f);
            cnt = 0;
            for (ch = (char *)(unsigned)&ai_Chars; (int)ch < (unsigned)((unsigned)ai_CharsV + 0x65400); ch += 0x21c) {
                if (*(unsigned short *)ch != 0xffff && HM_I(ch, 0x16c) == (int)bld)
                    chars[cnt++] = ch;
            }
            if (cnt > 0) {
                f += *(float *)(unsigned)&ai_Unk5eb94c;
                for (i = 0; i < cnt; i++) {
                    if (util_RandomF() <= f) {
                        if (util_RandomF() <= *(double *)(unsigned)&full_rdata_5eb7e8) {
                            ch = chars[i];
                            if (HM_I(ch, 0x17c) == 0) {
                                v = gm_Opinion(*(unsigned short *)(bld + 0x27), *(unsigned short *)ch);
                                k = -(util_Random(0x4a) & 0xffff);
                                if (k < v) {
                                    memset(msg, 0, 0xf8);
                                    HM_B(msg, 4) = 0x94;
                                    HM_I(msg, 8) = *(unsigned short *)(ev + 8);
                                    HM_I(msg, 0xc) = HM_I(bld, 4);
                                    *(HETIME *)(msg + 0x28) = ai_Time;
                                    HM_B(msg, 0x36) = 1;
                                    HM_I(msg, 0x58) = HM_I(ev, 4);
                                    cm_SendLongMessage(msg);
                                }
                            }
                        }
                    } else {
                        ch = chars[i];
                        if (!(HM_B(ch, 0x1b4) & 1) &&
                            (HM_B(ch, 0x180) == 4 || HM_B(ch, 0x180) == 0x8c || HM_B(ch, 0x180) == 0x8d))
                            ai_EndCharExtra(ch, 1);
                    }
                }
            }
        }
tail:
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x14 + (util_Random(0x14) & 0xffff), 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 1:
        bld = (char *)gm_BuildQuery(1, 5, 0x1d);
        if (bld == 0)
            goto tail;
        per = (char *)gm_FindCharacterByMaster(bld);
        cnt = 0;
        for (ch = (char *)(unsigned)&ai_Chars; (int)ch < (unsigned)((unsigned)ai_CharsV + 0x65400); ch += 0x21c) {
            if (*(unsigned short *)ch != 0xffff && HM_I(ch, 0x16c) == (int)bld)
                chars[cnt++] = ch;
        }
        gm_BuildingSkill(bld, 0x15, 1);
        for (i = 0; i < cnt; i++) {
            ch = chars[i];
            if (!(HM_B(ch, 0x1b4) & 1))
                continue;
            if (!(HM_B(ch, 0x180) == 4 || HM_B(ch, 0x180) == 0x8c || HM_B(ch, 0x180) == 0x8d || HM_B(ch, 0x180) == 0))
                continue;
            if (per != 0 && per == ch)
                continue;
            if (util_RandomF() <= *(float *)(unsigned)&ai_Unk5eb94c) {
                if (per != 0 && (HM_B(per, 0x1b4) & 1) && HM_I(ch, 0x184) != 0) {
                    ai_EndCharExtra(ch, 0);
                    continue;
                }
            } else if (per == 0 || !(HM_B(per, 0x1b4) & 1) || HM_I(ch, 0x184) == 0) {
                ai_EndCharExtra(ch, 0);
                continue;
            }
            ai_EndCharExtra(ch, 0);
        }
        gm_BuildNext();
        per = (char *)gm_BuildingMainObject(bld);
        if (cnt > 0) {
            n = util_Random(cnt) & 0xffff;
            ch = chars[n];
            if (ch != 0) {
                gm_GoodsPrice(*(unsigned short *)ch, *(unsigned char *)(unsigned)&ai_OwnPlayer);
                cm_SendExSellObject(-1, HM_I(bld, 4), *(unsigned short *)ch, 1, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
                if (ls_GetLanguage() == 0 || ls_GetLanguage() == 1) {
                    txt_Format(text, 0x414b, 0x229, HM_I(ch, 0x16c), *(unsigned short *)ch * 2 + 0x9a8);
                }
                he_MessageBoxQuickjump(*(int *)((unsigned)&ai_CharInts + *(unsigned short *)(ev + 8) * 0x21c), -1, text, 0, 0x5f2, 0, *(int *)(per + 2), (char *)(unsigned)&full_data_629000, 0);
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    case 2:
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
        return;
    case 3:
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0x1e, 0);
        HM_I(ev, 0x70) = 0;
        return;
    }
}
