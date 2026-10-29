/* 0x4d60b0, the speech texts of a fight (a: the fight record): for both leaders (the characters of +0x34 / +0x38) one
   line each, by the kind of the fight (bit 1: 0x11e0 / 0x11e1, bit 2: 0x11e3 / 0x11e4 with the role of the target
   building 0x4d4050, bit 4: 0x11e6 / 0x11e7), the suffix of the kind (0x143e948 / 0x143e954 / 0x143e960) appended and
   handed to 0x4e48b0. 0 when a leader is missing or the leaders are the same, a cart has no goods (0x50aa00) or the
   target is gone. Rough draft. */

int ai_Unk4d60b0(char *a)
{
    char buf[0x400];
    int rc[0x2e];
    char *ca, *cb, *edi, *p;
    int i, role;

    ca = gm_FindChar(*(int *)(a + 0x34));
    cb = gm_FindChar(*(int *)(a + 0x38));
    if (ca == 0 || cb == 0 || ca == cb)
        return 0;
    if (a[0x7c] & 1) {
        for (i = 0; i < 3; i++) {
            if (*(int *)(a + 0x100 + i * 4) != -1) {
                p = (char *)gm_FindObjectInst(*(int *)(a + 0x100 + i * 4));
                if (p == 0)
                    return 0;
                gm_StallGoodsList(p, rc);
                if (rc[0x2c] <= 0)
                    return 0;
            }
        }
    }
    edi = gm_FindBuildingById(*(int *)(a + 0x14));
    if (gm_BuildQuery(1, 1, *(int *)(a + 0x10c)) == 0)
        return 0;
    if (a[0x7c] & 1) {
        txt_Format(buf, 0x11e0, G20_US(cb));
        strcat(buf, ai_Unk143e948);
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e1, G20_US(ca));
        strcat(buf, ai_Unk143e948);
    } else if (a[0x7c] & 2) {
        role = ai_Unk4d4050((unsigned char *)gm_FindBuildingById(*(int *)(a + 0x10c))) + 0x1125;
        txt_Format(buf, 0x11e3, role, G20_US(cb), edi);
        strcat(buf, ai_Unk143e954);
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e4, G20_US(ca), role, edi);
        strcat(buf, ai_Unk143e954);
    } else if (a[0x7c] & 4) {
        txt_Format(buf, 0x11e6, G20_US(cb));
        strcat(buf, ai_Unk143e960);
        ai_Unk4e48b0(ca, a, buf, &ch_EmptyName);
        txt_Format(buf, 0x11e7, G20_US(ca));
        strcat(buf, ai_Unk143e960);
    } else
        return 1;
    ai_Unk4e48b0(cb, a, buf, &ch_EmptyName);
    return 1;
}
