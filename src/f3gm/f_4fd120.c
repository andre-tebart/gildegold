/* 0x4fd120, the yearly population step: counts the active characters (n); every kind 3 (child?) character that is old enough (age +10 at least 0x2c + random 4, or n >= 652.8) with no job / title bytes (+0x164, +0x166, +0x167) and removable (0x4fd090) is removed by 0x4fca40 (id, ..); the others (age above 1): kind 8 (no 0x4fd7e0, partners 0x5e / 0x62 gone) get the event messages 3 / 2 and a payment 0x4c4830 ((random + 0x800) * the mark * 0x20); the other aging characters 0x47dd70 (counted; relatives +0x1ee); then a random new arrival (0x4c4650 (0x10 ..) and a gift of (random + 10000) * 0x40) and, below 652.8 characters, up to (0x28 - the events) / 2 newcomers (0x4c4650 (3, .., 0x14 ..), random wage 0x400 .. + 0x400). ROUGH */

void unk_Unk4fd120(void)
{
    int n, events, kids, i, t, m;
    char *c;
    char *a, *b;
    int h, r, loc[2];
    unsigned v;

    n = 0;
    events = 0;
    kids = 0;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0)
            n++;
    }
    m = n;
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && c[8] != 0 && c[2] == 3 && ((util_RandomU(4) & 0xffff) + 0x2c <= *(unsigned short *)(c + 10) || (float)m >= 652.80005f) && c[0x164] == 0 && c[0x166] == 0 && c[0x167] == 0 && unk_Unk4fd090(c) != 0) {
            gm_AddHealth(*(unsigned short *)c, (int)0);
            n--;
            m = n;
        }
    }
    for (i = 0; i < 0x300; i++) {
        c = ai_Chars[i];
        if (*(short *)c != -1 && (unsigned char)c[8] > 1) {
            if ((float)m <= 652.80005f) {
            L1:
                if (c[2] == 8 && unk_Unk4fd7e0(*(unsigned short *)c) == 0) {
                    a = gm_FindChar(*(int *)(c + 0x5e));
                    b = gm_FindChar(*(int *)(c + 0x62));
                    if ((a == 0 || a[2] == 0x0f) && (b == 0 || b[2] == 0x0f)) {
                        loc[0] = 3;
                        loc[1] = 2;
                        cm_BeginFieldBlock(c, *(int *)(c + 4));
                        cm_AddFieldValue(1, 1, c - cm_FieldObject + 2, &loc[0]);
                        if ((unsigned char)c[0xd] < 2)
                            cm_AddFieldValue(1, 1, c - cm_FieldObject + 0xd, &loc[1]);
                        cm_SendFieldValues();
                        v = util_RandomU(0x800);
                        cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, ((v & 0xffff) + 0x800) * loc[1] * 0x20);
                        events++;
                    }
                    continue;
                }
            } else if (c[2] == 8) {
                a = gm_FindChar(*(int *)(c + 0x5e));
                b = gm_FindChar(*(int *)(c + 0x62));
                if ((a == 0 || a[2] == 0x0f) && (b == 0 || b[2] == 0x0f)) {
                    if (unk_Unk4fd090(c) != 0) {
                        gm_AddHealth(*(unsigned short *)c, 0);
                        m--;
                    }
                    continue;
                }
                goto L1;
            }
            if (ai_WorksForMaster(c) != 0) {
                events++;
                if (*(int *)(c + 0x1f0) != 0)
                    kids++;
            }
        }
    }
    if (kids < 6 && util_RandomU(4) == 0) {
        h = ((int (*)(int, int, int, int))cm_SendCreateCharacter)(0x10, -1, -1, util_RandomU(4) + 0x10);
        if (h != -1) {
            while (cm_GetSlotState(h) == 0)
                sv_PumpNet();
        }
        if (cm_GetSlotState(h) == 1) {
            events++;
            c = (char *)cm_GetSlotValue(h);
            r = ((util_RandomU(0x2000) & 0xffff) + 10000) * 0x40;
            cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, r);
        }
    }
    if ((float)m <= 652.80005f) {
        if (events < 0x1e)
            t = (0x28 - events) / 2;
        else if (events < 0x28)
            t = (util_RandomU(2) & 0xffff) + 1;
        else
            t = 0;
        while (t > 0) {
            v = util_RandomU(2);
            v = util_RandomU((v & 0xffff) + 1);
            h = ((int (*)(int, int, int, int, int, int))cm_SendCreateCharacter)(3, -1, -1, 0x14, -1, gm_TitleNumber((v & 0xffff) + 1));
            if (h != -1) {
                while (cm_GetSlotState(h) == 0)
                    sv_PumpNet();
            }
            if (cm_GetSlotState(h) == 1) {
                loc[0] = (util_RandomU(0xc) & 0xffff) + 10;
                c = (char *)cm_GetSlotValue(h);
                cm_SendMoveGoodsUnchecked(*(int *)(c + 4), -1, ai_OwnPlayer, ((util_RandomU(0x400) & 0xffff) + 0x400) * 0x40);
                cm_BeginFieldBlock(c, *(int *)(c + 4));
                cm_AddFieldValue(1, 1, c - cm_FieldObject + 0x167, &loc[0]);
                cm_SendFieldValues();
            }
            t--;
        }
    }
}
