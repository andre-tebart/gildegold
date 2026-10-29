/* 0x54ebd0, a Ghidra miss (disassembled; 928 B): the run handler of the disease simulation ("RunSimKrankheiten: calculating at <Day = %i Time = %i.2:%i.2>"), the twin of the accident simulation 0x54c570: the first run picks the step (+0xb0, one of 16 primes) and the start figure (+0xac); state 0 walks the 0x300 figures of the ring in 12 steps per run (0x54eb80 as the test) and rolls for every figure the chance 25 - ... : the strength by its boss (kind 2: 0x5092e0 (.., 8, 1) * 0.0125 * 20, else 0.02 * 20 * ..) times 0x5ba6b0 (0x2b3) 0.75 against 100 * random, for a hit the sickness is looked up (0x505230) and passed to 0x5417d0 (the accident report) or, without a boss, the news 0x163c (0x1632 + the kind) is shown (ROUGH) */

void he_Unk54ebd0(int ev)
{
    char text[0x100];
    int i, n, cur, rec, boss, k1, k2, r;
    float chance, thr, res, base;
    int pr0[16] = { 1, 3, 5, 7, 0xb, 0xd, 0x11, 0x13, 0x2ed, 0x2ef, 0x2f3, 0x2f5, 0x2f9, 0x2fb, 0x2fd, 0x2ff };

    if (*(int *)(ev + 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    sprintf(text, "-------> RunSimKrankheiten: calculating at <Day = %i Time = %i.2:%i.2>", *(int *)(unsigned)&ai_Time, *(unsigned short *)(unsigned)&ai_TimeHour, *(unsigned short *)((unsigned)&ai_Time + 0x6));
    if (*(int *)(ev + 0xb0) == 0) {
        *(int *)(ev + 0xb0) = pr0[(unsigned short)util_Random(0x10)];
        *(int *)(ev + 0xac) = (unsigned short)util_Random(0x300);
    }
    if (*(int *)(ev + 0x70) != 0)
        return;
    cur = *(int *)(ev + 0xac);
    for (n = 12; n > 0; n--) {
        rec = (unsigned)&ai_Chars + cur * 0x21c;
        if (he_Unk54eb80(rec) != 0) {
            thr = util_RandomF() * 100.0f;
            boss = *(int *)(rec + 0x170);
            k1 = 0;
            k2 = 0;
            if (boss != 0) {
                if (*(char *)(U_I((unsigned)&ai_BTypes) + *(unsigned char *)boss * 0x24d) == 2)
                    base = (float)gm_BuildingSkill(boss, 8, 1) * 0.0125f * 20.0f;
                else
                    base = (float)gm_BuildingSkill(boss, 8, 1) * 0.02f * 20.0f;
            } else {
                base = (float)(25.0 - (double)util_RandomF() * 20.0 * 0.5);
            }
            chance = ai_InventoryHas(rec, 0x2b3) != 0 ? base * 0.75f : base;
            if (!(thr + 0.0f > chance)) {
                if (gm_Unk505230(rec, &k1, &k2) != 0) {
                    boss = *(int *)(rec + 0x16c);
                    if (boss != 0 && *(unsigned short *)(boss + 0x27) != 0xffff && (unsigned)&ai_Chars + *(unsigned short *)(boss + 0x27) * 0x21c != 0) {
                        he_Unk5417d0(ev, (unsigned short *)((unsigned)&ai_Chars + *(unsigned short *)(boss + 0x27) * 0x21c), boss, rec, k1, (float)k2);
                    } else if (gm_CharacterHealth(rec) <= thr && (*(char *)(rec + 2) == 6 || *(char *)(rec + 2) == 7)) {
                        txt_Format(text, 0x163c, k1 + 0x1632);
                        he_ShowNews(*(int *)(rec + 4), -1, text, 0, 0x5f7, "");
                    }
                }
            }
        }
        cur = (*(int *)(ev + 0xb0) + cur) % 0x300;
        *(int *)(ev + 0xb4) = *(int *)(ev + 0xb4) + 1;
        if (*(int *)(ev + 0xb4) == 0)
            break;
    }
    *(int *)(ev + 0xac) = cur;
    if (*(int *)(ev + 0xb4) >= 0x300) {
        *(short *)(ev + 0x56) = 0xa;
        *(int *)(ev + 0xac) = 0;
        *(int *)(ev + 0xb0) = 0;
        *(int *)(ev + 0xb4) = 0;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
        return;
    }
    gm_AddTime(ev + 0x52, 0, 0x1e, 0);
    if (*(unsigned short *)(ev + 0x56) >= 0x12) {
        *(int *)(ev + 0x58) = 0;
        *(short *)(ev + 0x56) = 0xa;
        *(int *)(ev + 0x52) = *(int *)(ev + 0x52) + 1;
    }
}
