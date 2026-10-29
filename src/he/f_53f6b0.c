/* 0x53f6b0, the daily advisor of the player's town (day % 5 picks one of five themes; the first 4 / 3 / 2 / 3 options use the kinds 4 / 0x13 / ?, 0x12 / 0x15 / 0x14 / 0xe, 8 / 0x16, 9 / 7 / 5, 0x20 / 0x1f / 0x1d): counts the figures by kind (all / the player's own), picks the theme's options that are short of figures (by the share < 0.5 or < 3 of them, zero first; with the probability gate x / (x + 0x5eebc4[level]), x = day / 4 + 1) and shows for each a message with a reward 0x4c47e0 (ROUGH: the 5 branches are written from the pseudocode, many details of the choice may differ) ; the per-theme tables kinds / ftop / fbarg are automatic arrays here, the original has their values as immediates in per-option code (push 0x29 at 0x53fd01, 0x32 at 0x53fdae, ..), it has no such tables in its data */

int gm_TitleNumber();
int gm_BuildingKindValue();
void cm_SendCreateCharacter(unsigned char a, int b, int c, unsigned short d, int e, unsigned char f, unsigned char g, unsigned char h);
int cm_GetSlotValue(int h);
float util_RandomF();

/* the advice option of 0x53f6b0 (a macro, so that no extra function is emitted): when more than 2 figures of the kind `kind` exist, the first one that belongs to the player (owner 0xffff or the player of 0x6f1bf8) gives the amount (0x509b10 * 2) and the topic 0x5043d0(topic, level * 2 - 1); with fewer (or `nosearch`) the amount is 0x509d30(fbarg) * 2 and the topic 0x5043d0(fbtopic, fb2) (the table-less defaults) */
#define HE_ADV_FIND(kind, found, fbarg, fbtopic, fb2, tv, amtv) \
    do { \
        int _k; \
        unsigned char *_p = (unsigned char *)U_I((unsigned)&ai_Blds); \
        amtv = -1; \
        for (_k = 0; _k < 0x100; _k++, _p += 0xa9) { \
            if (*_p != 0 && *(char *)(U_I((unsigned)&ai_BTypes) + *_p * 0x24d) == (kind) \
                && (*(unsigned short *)(_p + 0x27) == 0xffff || *(unsigned short *)(_p + 0x27) == **(unsigned short **)(unsigned)&ai_PlayerChar)) { \
                amtv = gm_EquipmentValue(_p, 100) * 2; \
                tv = gm_TitleNumber(found, *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *_p * 0x24d + 0x247) * 2 - 1); \
                break; \
            } \
        } \
        if (amtv == -1) \
            return ev; \
    } while (0)

#define HE_ADV_FB(fbarg, fbtopic, fb2, tv, amtv) \
    do { \
        amtv = gm_BuildingKindValue(fbarg) * 2; \
        tv = gm_TitleNumber(fbtopic, fb2); \
    } while (0)

/* the shared tail: builds the text (0x42ec00 .. topic, rnd), shows it as a quick message (0x4c4650) and, when it came up (result 1), pays amt (0x4c47e0) and marks it (0x4c3c00 flag 0xd = 2) */
#define HE_ADV_SHOW(tv, amtv, rnd, extra) \
    do { \
        int _t, _h, _b; \
        int _two = 2; \
        _t = ((int (*)(int, int, int, int, int))util_Random)(4, -1, tv, 0, rnd); \
        _h = ((int (*)(int, int, int, int))cm_SendCreateCharacter)(4, -1, -1, _t + 0xf); \
        if (_h != -1) { \
            while (cm_GetSlotState(_h) == 0) \
                sv_PumpNet(); \
        } \
        if (cm_GetSlotState(_h) == 1) { \
            _b = cm_GetSlotValue(_h); \
            cm_SendMoveGoods(*(int *)(_b + 4), -1, U_I((unsigned)&ai_OwnPlayer), amtv); \
            cm_BeginFieldBlock(_b, *(int *)(_b + 4)); \
            cm_AddFieldDelta(1, 1, 0xd, &_two); \
            extra; \
            cm_SendFieldDeltas(); \
        } \
    } while (0)

unsigned he_Unk53f6b0(void)
{
    int cnt[0x21];
    int own[0x21];
    int lvl, i, k, ev = 0;
    int x, nai = 0, nplay = 0;
    float gate, ratio[8];
    unsigned mask, zero, pick, elig;
    unsigned char *pb;
    int amt, tp;
    unsigned short us;
    unsigned char kind;

    lvl = U_I((unsigned)&ai_Difficulty);
    if (lvl >= 5)
        lvl = 4;
    else if (lvl < 1)
        lvl = 0;
    x = (U_I((unsigned)&ai_Time) + ((U_I((unsigned)&ai_Time) >> 31) & 3)) / 4 + 1;
    gate = (float)x / ((float)x + ((float *)((unsigned)&ai_Unk5eeab8 + 0x10c))[lvl]);
    he_Unk53f5b0();
    memset(cnt, 0, sizeof(cnt));
    memset(own, 0, sizeof(own));
    pb = (unsigned char *)U_I((unsigned)&ai_Blds);
    for (i = 0; i < 0x100; i++, pb += 0xa9) {
        if (*pb != 0 && (kind = *(unsigned char *)(U_I((unsigned)&ai_BTypes) + *pb * 0x24d)) < 0x21) {
            cnt[kind]++;
            us = *(unsigned short *)(pb + 0x27);
            if (us == 0xffff || us == **(unsigned short **)(unsigned)&ai_PlayerChar)
                own[kind]++;
            else if (kind == 7) {
                if (*(char *)(((unsigned)&ai_Unk770b89 + 0x3) + us * 0x21c) == 0)
                    nai++;
                else
                    nplay++;
            }
        }
    }
    k = U_I((unsigned)&ai_Time) % 5;
    if (k == 1) {
        int a = cnt[0x10] - own[0x10], b = cnt[4] - own[4], c = cnt[0x13] - own[0x13];

        nai = 0;
        mask = (b < 3) | ((c < 3) << 1) | ((a < 3) << 2);
        zero = (b == 0) | ((c == 0) << 1) | ((a == 0) << 2);
        if (zero == 0) {
            if (util_RandomF() >= gate)
                return mask;
            if (c * 2 < b + a && (mask & 2))
                pick = 2;
            else if (b + a < c * 2) {
                pick = 0;
                if (b < a) {
                    if (mask & 1)
                        pick = 1;
                } else if (mask & 4)
                    pick = 4;
            } else
                pick = 1 << (util_Random(3) & 0xff);
        } else {
            us = util_Random(3);
            pick = 0;
            for (i = 3; i != 0; i--) {
                if (zero & (1 << (us & 0x1f))) {
                    pick = 1 << (us & 0x1f);
                    break;
                }
                us = (us + 1) % 3;
            }
        }
        if (pick == 0)
            return pick;
        if (pick == 1) {
            if (cnt[4] > 2)
                HE_ADV_FIND(4, 0xb, 0, 0, 0, tp, amt);
            else
                HE_ADV_FB(0xb, 6, 0, tp, amt);
        } else if (pick == 2) {
            if (cnt[0x13] > 2)
                HE_ADV_FIND(0x13, 10, 0, 0, 0, tp, amt);
            else
                HE_ADV_FB(0x2c, 0x39, 0, tp, amt);
        } else {
            HE_ADV_FB(0x27, 0x2d, 0, tp, amt);
        }
        HE_ADV_SHOW(tp, amt, 0, (void)0);
        return pick;
    }
    if (k == 2) {
        int kinds[4] = { 0x12, 0x15, 0x14, 0xe };
        int ftop[4] = { 1, 3, 2, 5 };
        int fbarg[4] = { 0x29, 0x32, 0x2f, 0x21 };

        elig = 0;
        zero = 0;
        for (i = 0; i < 4; i++) {
            ratio[i] = cnt[kinds[i]] < 1 ? 0.0f : (float)own[kinds[i]] / (float)cnt[kinds[i]];
            if (cnt[kinds[i]] < 3 || ratio[i] >= 0.5f) {
                elig |= 1 << i;
                if (cnt[kinds[i]] == 0)
                    zero |= 1 << i;
            }
        }
        if (zero == 0) {
            if (util_RandomF() >= gate)
                return elig;
            us = util_Random(4);
            zero = 0;
            for (i = 4; i != 0; i--) {
                if (elig & (1 << (us & 0x1f))) {
                    zero = 1 << (us & 0x1f);
                    break;
                }
                us = (us + 1) & 3;
            }
        }
        for (i = 0; i < 4 && zero != 0; i++) {
            if (!(zero & (1 << i)))
                continue;
            if (cnt[kinds[i]] > 2)
                HE_ADV_FIND(kinds[i], ftop[i], 0, 0, 0, tp, amt);
            else
                HE_ADV_FB(fbarg[i], ftop[i], 1, tp, amt);
            HE_ADV_SHOW(tp, amt, util_Random(2), (void)0);
        }
        return elig;
    }
    if (k == 3) {
        int kinds[2] = { 8, 0x16 };
        int ftop[2] = { 6, 4 };
        int fbarg[2] = { 0x17, 0x35 };

        elig = 0;
        zero = 0;
        for (i = 0; i < 2; i++) {
            ratio[i] = cnt[kinds[i]] < 1 ? 0.0f : (float)own[kinds[i]] / (float)cnt[kinds[i]];
            if (cnt[kinds[i]] < 3 || ratio[i] >= 0.5f) {
                elig |= 1 << i;
                if (cnt[kinds[i]] == 0)
                    zero |= 1 << i;
            }
        }
        if (zero == 0) {
            if (util_RandomF() >= gate)
                return elig;
            us = util_Random(2);
            zero = 0;
            for (i = 2; i != 0; i--) {
                if (elig & (1 << (us & 0x1f))) {
                    zero = 1 << (us & 0x1f);
                    break;
                }
                us = (us + 1) & 1;
            }
        }
        for (i = 0; i < 2 && zero != 0; i++) {
            if (!(zero & (1 << i)))
                continue;
            if (cnt[kinds[i]] > 2)
                HE_ADV_FIND(kinds[i], ftop[i], 0, 0, 0, tp, amt);
            else
                HE_ADV_FB(fbarg[i], ftop[i], 1, tp, amt);
            HE_ADV_SHOW(tp, amt, util_Random(2), (void)0);
        }
        return elig;
    }
    if (k == 0) {
        int kinds[3] = { 9, 7, 5 };
        int ftop[3] = { 8, 7, 9 };
        int fbarg[3] = { 0x1a, 0x14, 0xe };

        elig = 0;
        zero = 0;
        ratio[0] = cnt[9] < 1 ? 0.0f : (float)own[9] / (float)cnt[9];
        ratio[2] = cnt[5] < 1 ? 0.0f : (float)own[5] / (float)cnt[5];
        if (cnt[9] < 2 || 0.5f <= ratio[0]) {
            elig |= 1;
            if (cnt[9] == 0)
                zero |= 1;
        }
        if (cnt[7] < 2 || (cnt[7] < 3 && (nai == 0 || nplay == 0))) {
            elig |= 2;
            if (cnt[7] == 0)
                zero |= 2;
        }
        if (cnt[5] < 2 || ratio[2] >= 0.5f) {
            elig |= 4;
            if (cnt[5] == 0)
                zero |= 4;
        }
        if (zero == 0) {
            if (util_RandomF() >= gate)
                return elig;
            us = util_Random(3);
            for (i = 3; i != 0; i--) {
                if (elig & (1 << (us & 0x1f))) {
                    zero = elig;
                    break;
                }
                us = (us + 1) % 3;
            }
        }
        for (i = 0; i < 3 && zero != 0; i++) {
            if (!(zero & (1 << i)))
                continue;
            if (i == 1) {
                int fl = ((int)nai < nplay) ? 0 : (nplay < nai) ? 1 : 0;

                HE_ADV_FB(0x14, 7, 1, tp, amt);
                HE_ADV_SHOW(tp, amt, util_Random(2), cm_AddFieldDelta(1, 1, 0xc, &fl));
            } else {
                if (cnt[kinds[i]] > (i == 0 ? 1 : 0))
                    HE_ADV_FIND(kinds[i], ftop[i], 0, 0, 0, tp, amt);
                else
                    HE_ADV_FB(fbarg[i], ftop[i], 1, tp, amt);
                HE_ADV_SHOW(tp, amt, util_Random(2), (void)0);
            }
        }
        return elig;
    }
    if (k == 4) {
        int kinds[3] = { 0x20, 0x1f, 0x1d };
        int ftop[3] = { 0xf, 0xe, 0xd };
        int fbarg[3] = { 0x51, 0x4e, 0x48 };

        elig = 0;
        zero = 0;
        for (i = 0; i < 3; i++) {
            ratio[i] = cnt[kinds[i]] < 1 ? 0.0f : (float)own[kinds[i]] / (float)cnt[kinds[i]];
            if (cnt[kinds[i]] < 3 || ratio[i] < 0.5f == 0) {
                elig |= 1 << i;
                if (cnt[kinds[i]] == 0)
                    zero |= 1 << i;
            }
        }
        if (zero == 0) {
            if (util_RandomF() >= gate)
                return elig;
            us = util_Random(3);
            zero = elig;
            for (i = 3; i != 0; i--) {
                if (elig & (1 << (us & 0x1f))) {
                    zero = 1 << (us & 0x1f);
                    break;
                }
                us = (us + 1) % 3;
            }
        }
        for (i = 0; i < 3 && zero != 0; i++) {
            if (!(zero & (1 << i)))
                continue;
            if (cnt[kinds[i]] > 2)
                HE_ADV_FIND(kinds[i], ftop[i], 0, 0, 0, tp, amt);
            else
                HE_ADV_FB(fbarg[i], ftop[i], 1, tp, amt);
            HE_ADV_SHOW(tp, amt, util_Random(2), (void)0);
        }
        return elig;
    }
    return ev;
}
