/* 0x54f160, a Ghidra miss (disassembled; 2500 B): the run handler of the "Dunkle Ecke" robbery event (+0xac the carrier (handle, then figure id), +0xb0 the victim building, +0xb8 the next target, +0xc4 / +0xc8 the walk over the 0x100 buildings, +0xc0 the loot level 0 .. 3, +0xbc the steps left, +0xb4 the amount): state 0 waits for the carrier, state 1 picks the next target building (kinds in the mask 0xa7fcc3a6, not the victim's own; "Dunkle Ecke: next target = %G") and sends the carrier there, state 2 waits until it arrives (0x524770), then counts the robbery steps (+0xbc, +0xc0 rises by the chance 0x504ec0 against 0.5 * random) and sends the carrier on to the victim, state 3 at the victim: the loot level is clamped to 0 .. 3 and the thief takes up to three items from 0x91f700 (0x2d bytes each, 0x200 ring, +0x25 = 1, other owner, not due: given with 0x4c4ed0 and written into the job table of the building record), the news 0x18a9 / 0x18aa (0x5ec); state 4 repeats the check of the loot; a free of the event (flag 2) sends the carrier home (0x4c4e00) (ROUGH) */

void he_Unk54f160(int ev)
{
    char text[0x400];
    int st, o = 0, v, i, n, idx, found, b, h, a, kind, loot[3], cnt, k, w, t;
    float f;

    st = *(int *)(ev + 0x70);
    if (st == -1 || st == -2) {
        if ((*(unsigned char *)(ev + 0x78) & 2) != 0) {
            a = gm_FindChar(*(int *)(ev + 0xac));
            if (a != 0) {
                ai_Unk4c5230(*(int *)(a + 4));
                cm_SendKillCharacter(*(int *)(a + 4), 1);
            }
        }
        he_Unk53e1f0((int *)ev);
        return;
    }
    if ((*(unsigned char *)(ev + 0x78) & 4) != 0)
        return;
    if (*(int *)(ev + 0x84) != -1 && cm_GetSlotState(*(int *)(ev + 0x84)) == 0)
        return;
    switch (*(int *)(ev + 0x70)) {
    case 0:
        if (cm_GetSlotState(*(int *)(ev + 0xac)) == 1) {
            *(int *)(ev + 0xac) = *(int *)(cm_GetSlotValue(*(int *)(ev + 0xac)) + 4);
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(4, ev);
        return;
    case 1:
        n = 0x100;
        idx = *(int *)(ev + 0xc4);
        found = 0;
        do {
            unsigned lo = 0, hi = 0;
            int e2, k2;

            found = U_I((unsigned)&ai_Blds) + idx * 0xa9;
            e2 = U_I((unsigned)&ai_BTypes) + *(unsigned char *)found * 0x24d;
            k2 = *(unsigned char *)e2;
            if (k2 > 0x1f)
                hi = 1u << (k2 - 0x20);
            else
                lo = 1u << k2;
            if (((lo & 0xa7fcc3a6) != 0 || (hi & 1) != 0) && *(int *)(found + 1) == *(int *)(ev + 0xb0))
                ;
            else
                goto next_t;
            break;
        next_t:
            idx = (idx + *(int *)(ev + 0xc8)) & 0xff;
            found = 0;
            n--;
        } while (n != 0);
        *(int *)(ev + 0xc4) = (idx + *(int *)(ev + 0xc8)) & 0xff;
        if (found == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        sprintf(text, "Dunkle Ecke: next target = %G", found);
        a = gm_FindChar(*(int *)(ev + 0xac));
        if (a != 0) {
            ai_Unk4c5230(*(int *)(a + 4));
            ai_Unk4c52e0(*(int *)(a + 4), *(int *)(found + 1), -1, 0, 1, "Dunkle Ecke");
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0xb8) = *(int *)(found + 1);
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        gm_AddTime(ev + 0x52, 0, 1, 0);
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 2:
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb8));
        gm_AddTime(ev + 0x52, 0, 5, 0);
        if (o == 0) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        a = gm_FindChar(*(int *)(ev + 0xac));
        if (a == 0) {
            gm_AddTime(ev + 0x52, 0, 1, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (sv_Unk524770(a, o) == 0) {
            *(int *)(ev + 0x70) = 2;
            gm_AddTime(ev + 0x52, 0, 5, 0);
            *(int *)(ev + 0x84) = cm_SendExSetHE(2, ev);
            return;
        }
        *(int *)(ev + 0xbc) = *(int *)(ev + 0xbc) - 1;
        f = gm_GetSkill((unsigned)&ai_Chars + *(unsigned short *)(o + 0x27) * 0x21c, 2);
        if (*(int *)(ev + 0xc0) < 1)
            *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) + 1;
        else
            *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) + (util_RandomF() * 0.5f < f ? 1 : 0);
        if (*(int *)(ev + 0xc0) > 2)
            *(int *)(ev + 0xbc) = 0;
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb0));
        if (o != 0 && *(unsigned short *)(o + 0x27) != 0xffff) {
            if (*(int *)(ev + 0xbc) <= 0) {
                ai_Unk4c5230(*(int *)(a + 4));
                ai_Unk4c52e0(*(int *)(a + 4), *(int *)(o + 1), *(int *)(ev + 0xb4), 0, 0, "Dunkle Ecke");
                *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
                return;
            }
            *(int *)(ev + 0x84) = cm_SendExSetHE(1, ev);
            return;
        }
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    case 3:
    case 4:
        gm_AddTime(ev + 0x52, 0, 5, 0);
        gm_FindById(&o, 0, 0, *(int *)(ev + 0xb0));
        if (o == 0 || *(unsigned short *)(o + 0x27) == 0xffff) {
            *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
            return;
        }
        if (*(int *)(ev + 0x70) == 3) {
            a = gm_FindChar(*(int *)(ev + 0xac));
            if (a != 0 && sv_Unk524770(a, o) == 0) {
                *(int *)(ev + 0x70) = 3;
                gm_AddTime(ev + 0x52, 0, 5, 0);
                *(int *)(ev + 0x84) = cm_SendExSetHE(3, ev);
                return;
            }
            *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) >= 3 ? 3 : (*(int *)(ev + 0xc0) > 0 ? *(int *)(ev + 0xc0) : 0);
        } else {
            *(int *)(ev + 0xc0) = 0;
            for (k = 3; k > 0; k--) {
                f = gm_GetSkill((unsigned)&ai_Chars + *(unsigned short *)(o + 0x27) * 0x21c, 2);
                if (util_RandomF() * 0.002f < f)
                    *(int *)(ev + 0xc0) = *(int *)(ev + 0xc0) + 1;
            }
        }
        cnt = *(int *)(ev + 0xc0);
        if (cnt != 0) {
            idx = (unsigned short)util_Random(0x200);
            w = U_I((unsigned)&ai_Unk5ed100 + (unsigned short)util_Random(8) * 4);
            n = 0;
            for (i = 0x200; i > 0 && n < cnt; i--) {
                int e3 = (unsigned)&ai_Unk91f700 + idx * 0x2d;

                if (*(int *)e3 != -1 && *(int *)(e3 + 0x25) == 1 && *(int *)(e3 + 0x16) != U_I((unsigned)&ai_CharInts + *(unsigned short *)(o + 0x27) * 0x21c)
                    && gm_CompareTime(ev + 0x44, e3 + 4) != 1)
                    loot[n++] = *(int *)e3;
                idx = (idx + w) & 0x1ff;
            }
            *(int *)(ev + 0xc0) = n;
            for (i = 0; i < n; i++)
                cm_SendProofExecuted(loot[i], U_I((unsigned)&ai_CharInts + *(unsigned short *)(o + 0x27) * 0x21c));
            b = gm_Query(*(int *)(o + 0x5d), 2, 6, 0x12c);
            if (b != 0) {
                cm_BeginFieldBlock(b, *(int *)(b + 2));
                for (i = 0; i < 3; i++)
                    cm_AddFieldDelta(4, 1, i * 4 + 0x1c - U_I((unsigned)&cm_FieldObject) + b, &loot[i]);
                cm_SendFieldDeltas();
            }
        }
        if (*(int *)(ev + 0xc0) != 0)
            txt_Format(text, 0x18a9);
        else
            txt_Format(text, 0x18aa);
        he_MessageBoxQuickjump(U_I((unsigned)&ai_CharInts + *(unsigned short *)(o + 0x27) * 0x21c), -1, text, 0, 0x5ec, *(int *)(o + 1), *(int *)(ev + 0xb4), 0, "");
        *(HETIME *)(ev + 0x52) = ai_Time;
        *(int *)(ev + 0x84) = cm_SendExSetHE(-1, ev);
        return;
    default:
        return;
    }
}
