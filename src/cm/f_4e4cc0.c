/* 0x4e4cc0, the per-frame update of the scene parties (the records of 0x114 bytes from 0x925538: +0x18 used, +0xe flags (2: the owner answers, 5: idle, 0x80:
   a member run), +0x10 state, +4 the age in ticks; the party m itself starts 0x18 before the record: +0 the id, -0x14 done, -0x10 the kind, -0xc the owner):
   the timers (0x4fe140) are fed, the owner's own party asks "ready?" (0x4e4b50: -1 gives the timer 5, 0 the timers 0x1e / 9 (old ones cancel the party
   0x4c5ef0), else the callback of the kind (0x92c510) runs), a finished party is run (0x4e4060) and dropped (0x4e5180). ROUGH */

int ai_Unk4e4cc0(void)
{
    char *r;
    G17T14 tmp;
    int x, k;

    for (r = (char *)ai_Unk925520 + 0x18; (unsigned int)r < ((unsigned)ai_Unk925520 + 0x6780); r += 0x114) {
        if (r[0x18] == 0 || (r[0xe] & 5))
            continue;
        tmp = *(G17T14 *)r;
        gm_AddTime(&tmp, 0, -60, 0);
        if (*(int *)(r - 0xc) == -1 && ((cm_NetFlags & 8) || (ai_GameFlags & 8)))
            *(int *)(r - 0xc) = ai_CharInts[ai_PlayerId][0];
        if (ai_CharInts[ai_PlayerId][0] == *(int *)(r - 0xc) && *(int *)(r + 0x10) == 0 && gm_CompareTime(ai_Time, (char *)&x) > 0) {
            k = ai_Unk4e4b50(r - 0x18);
            if (k == -1) {
                gm_AddTime(r, 0, 5, 0);
                if (*(unsigned short *)(r + 4) >= 0x16)
                    gm_AddTime(r, 9, 0, 0);
                continue;
            }
            if (k == 0) {
                gm_AddTime(r, 0, 0x1e, 0);
                if (*(unsigned short *)(r + 4) >= 0x16)
                    gm_AddTime(r, 9, 0, 0);
                continue;
            }
            if (ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5] != 0 && ((int (*)())ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5])(r - 0x18) == 0) {
                ai_Unk4c5ef0(*(int *)(r - 0x18));
                continue;
            }
        }
        if (ai_Unk92c510[*(unsigned char *)(r - 0x10) * 5] == 0 && gm_CompareTime(ai_Time, r) > 0)
            *(int *)(r - 0x14) = 1;
        if (gm_CompareTime(ai_Time, r) > 0 && *(int *)(r - 0x14) == 0) {
            if (ai_Unk4e4870(ai_CharInts[ai_PlayerId][0], r - 0x18) != 0 && (signed char)r[0xe] < 0) {
                if (ai_CharInts[ai_PlayerId][0x208 / 4] == *(int *)(r - 0x18))
                    *(int *)(r - 0x14) = 1;
            } else if (*(int *)(r - 0xc) == ai_CharInts[ai_PlayerId][0] && (r[0xe] & 2))
                *(int *)(r - 0x14) = 1;
            else
                ai_Unk4e5180(*(int *)(r - 0x18));
        }
        if (gm_CompareTime(ai_Time, (char *)&x) > 0 && *(int *)(r - 0x14) != 0) {
            sv_PanelFlag = 0;
            ai_Unk4e4060(r - 0x18);
            ai_Unk4e5180(*(int *)(r - 0x18));
        }
    }
    return 0;
}
