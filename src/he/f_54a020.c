/* 0x54a020, a Ghidra miss (disassembled; 1936 B): the run handler of the plague event ("Pest", +0xbc .. +0xc8 the four carriers (job handles, then figure ids), +0xb0 / +0xb4 the walk over the 0x100 buildings (0xa9 bytes each, table 0x889f74; +0xb4 the step), +0xcc / +0xd0 the target building): state 0 turns the handles into ids, state 1 picks the next target building of a kind in the mask 0x807c4110 (debug text "EvtPest: next target = %G") and sends the carriers there (0x4c52e0 "Pest"), state 2 waits for the carriers at the target (0x524770), then infects the figures near it (0x5417d0 with the strengths 0x505440, 0x42ec70) and counts +0xac down, state 3 waits until a carrier is away from the target again; a free of the event (flag 2) lets the carriers stop (0x4c4e00) (ROUGH) */

void he_Unk54a020(int ev)
{
    char text[0x100];
    int st, i, n, idx, esi, o = 0, p, h, r;
    float strength[16];
    int ids[16];

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            for (i = 0; i < 4; i++) {
                p = gm_FindChar(*(int *)(ev + 0xbc + i * 4));
                if (p != 0) {
                    ai_Unk4c5230(*(int *)(p + 4));
                    cm_SendKillCharacter(*(int *)(p + 4), 1);
                }
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    if (*(int *)(ev + 0x70) == 5)
        he_Unk549ce0(ev);
    switch (*(int *)(ev + 0x70)) {
    case 0:
        for (i = 0; i < 4; i++) {
            h = *(int *)(ev + 0xbc + i * 4);
            if (cm_GetSlotState(h) == 1 && (r = cm_GetSlotValue(h)) != 0)
                *(int *)(ev + 0xbc + i * 4) = *(int *)(r + 4);
            else
                *(int *)(ev + 0xbc + i * 4) = -1;
        }
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
        return;
    case 1:
        n = 0x100;
        idx = *(int *)(ev + 0xb0);
        esi = 0;
        do {
            unsigned lo = 0, hi = 0;
            int e, k;

            esi = U_I((unsigned)&ai_Blds) + idx * 0xa9;
            e = U_I((unsigned)&ai_BTypes) + *(unsigned char *)esi * 0x24d;
            k = *(unsigned char *)e;
            if (k > 0x1f)
                hi = 1u << (k - 0x20);
            else
                lo = 1u << k;
            if (*(unsigned short *)(esi + 0x27) != 0xffff && ((lo & 0x807c4110) != 0 || (hi & 1) != 0))
                break;
            idx = (idx + *(int *)(ev + 0xb4)) & 0xff;
            esi = 0;
            n--;
        } while (n != 0);
        *(int *)(ev + 0xb0) = (*(int *)(ev + 0xb4) + idx) & 0xff;
        if (esi == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        sprintf(text, "EvtPest: next target = %G", esi);
        n = 0;
        for (i = 0; i < 4; i++) {
            p = gm_FindChar(*(int *)(ev + 0xbc + i * 4));
            if (p != 0) {
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), *(int *)(esi + 1), -1, 0, 1, "Pest");
                n++;
            }
        }
        gm_AddTime(ev + 0x52, 0, n <= 0 ? 0x14 : 5, 0);
        *(int *)(ev + 0xcc) = *(int *)(esi + 1);
        *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
        return;
    case 2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xcc));
        gm_AddTime(ev + 0x52, 0, 5, 0);
        if (o == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            p = gm_FindChar(*(int *)(ev + 0xbc + i * 4));
            if (p != 0 && sv_Unk524770(p, o) == 0) {
                *(int *)(ev + 0x70) = 2;
                gm_AddTime(ev + 0x52, 0, 5, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
                return;
            }
        }
        if (*(unsigned short *)(o + 0x27) != 0xffff) {
            /* the figures of the target building: strength by 0x505440, one random one gets the plague (0x5417d0) */
            int cnt = 0, first = 0, rec2, j, any = 0;
            float big = 1000.0f;
            for (rec2 = (unsigned)&ai_Unk770cec, j = 0; j < 16 && rec2 < ((unsigned)((unsigned)ai_Unk770cec + 0x65296) + 0x16a); rec2 += 0x21c) {
                if (*(char *)(rec2 - 0x16a) == 0 && *(int *)rec2 == o) {
                    strength[j] = big;
                    ids[j] = j;
                    j++;
                } else if (*(char *)(rec2 - 0x16a) == 1 && *(int *)rec2 == o) {
                    first = rec2 - 0x16c;
                }
            }
            cnt = j;
            if (cnt != 0) {
                float a = util_RandomF2();
                int m;

                if (a > 1.0f)
                    a = 1.0f;
                a *= (float)cnt;
                m = (int)(a < 1.0f ? 1.0f : a);
                for (j = 0; j < m; j++) {
                    strength[j] = gm_Unk505440((unsigned)&ai_Chars + ids[j] * 0x21c);
                    any = 1;
                }
                if (first != 0 && (short)util_Random(8) == 0) {
                    strength[0] = gm_Unk505440(first);
                    any = 1;
                }
                if ((short)util_Random(0x100) == 0)
                    gm_Unk505440(0);
                if (any)
                    ai_Unk5a9240K(0, o);
                for (j = 0; j < m; j++)
                    if (strength[j] > 0.0f)
                        he_Unk5417d0(ev, (unsigned short *)((unsigned)&ai_Chars + ids[j] * 0x21c), 0, o, 1, strength[j]);
            }
        }
        if (*(int *)(ev + 0xac) > 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xd0));
        if (o == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        n = 0;
        for (i = 0; i < 4; i++) {
            p = gm_FindChar(*(int *)(ev + 0xbc + i * 4));
            if (p != 0) {
                ai_Unk4c5230(*(int *)(p + 4));
                ai_Unk4c52e0(*(int *)(p + 4), *(int *)(o + 1), -1, 0, 1, "Pest");
                n++;
            }
        }
        gm_AddTime(ev + 0x52, 0, n <= 0 ? 10 : 2, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
        return;
    case 3:
        gm_AddTime(ev + 0x52, 0, 5, 0);
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xd0));
        if (o == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        for (i = 0; i < 4; i++) {
            p = gm_FindChar(*(int *)(ev + 0xbc + i * 4));
            if (p == 0 || sv_Unk524770(p, o) == 0) {
                *(int *)(ev + 0x70) = 3;
                gm_AddTime(ev + 0x52, 0, 5, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
                return;
            }
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    default:
        return;
    }
}
