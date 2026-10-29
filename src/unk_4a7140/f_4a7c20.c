/* 0x4a7c20, each NPC decides whether to fold (seat->unk28 = the phase) against the open cards of the others */

void ai_Unk4a7c20(void)
{
    int s, j, n, r, m, k, y;
    int x8, xc;
    G10SEAT *st;

    for (s = 1; s < 3; s++) {
        st = &ai_Unkf83654->seat[s];
        if (st->unk2c == 0 && st->unk20 == 0 && ai_Unkf83654->unkb5 > 3)
            st->unk28 = ai_Unkf83654->phase;
        for (j = 0; j < 2; j++) {
            if (j != s && ai_Unkf83654->seat[j].unk28 < 0) {
                ai_Unk4a7e70(&ai_Unkf83654->seat[j], &n, &r, &x8, &xc);
                switch (st->unk2c) {
                case 0:
                    if (st->unk20 == 0) {
                        if (n > 1 || util_Random(4) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    } else if (n > 2) {
                        st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 1:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &k);
                        if (ai_Unkf83654->phase > 5 && m <= n && (m < n || k < r))
                            st->unk28 = ai_Unkf83654->phase;
                    } else {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (m < n && ai_Unkf83654->phase > 5 && ai_Unkf83654->unkb5 > 4)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 2:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (ai_Unkf83654->unkb5 < 5 && m == 3 && util_Random(4) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                case 3:
                    if (st->unk20 == 0) {
                        m = ai_Unk4a7650(st->card, 0, 7, &y);
                        if (ai_Unkf83654->unkb5 < 5 && m == 3 && util_Random(5) == 1)
                            st->unk28 = ai_Unkf83654->phase;
                    }
                    break;
                }
            }
        }
    }
}
