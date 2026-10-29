/* 0x4f28d0, writes the graveyard game state to the file f: the tag 0x1d, the ints +0x18 of the 0x1d table records (0x611270), 0x6f1b88, the counts of the used 0x2d-byte records at 0x91f700 (first int not -1) and of the used pairs at 0x91b6e0, then
   the used records (fields 4, 0xe, 4, 4, 2, 1, 4, 4, 4 bytes) and the used pairs; 1 when all was written. ROUGH */

int unk_Unk4f28d0(void *f)
{
    int tag, cnt, cnt2;
    char *p;
    int *q;

    tag = 0x1d;
    cnt = 0;
    cnt2 = 0;
    if (f == 0)
        return 0;
    if (UNK_FW(&tag, 4, 1, f) == 0)
        return 0;
    for (p = unk_Tab611258 + 0x18; (int)p < ((unsigned)&unk_Tab611258 + 0x42c); p += 0x24) {
        if (UNK_FW(p, 4, 1, f) == 0)
            return 0;
    }
    if (UNK_FW(unk_Var6f1b88, 4, 1, f) == (unsigned)-1)
        return 0;
    for (q = (int *)gv_Graveyard; (int)q < (int)(gv_Graveyard + 0x5a00); q = (int *)((char *)q + 0x2d)) {
        if (*q != -1)
            cnt++;
    }
    for (q = (int *)(unk_Tab91b6e0 + 4); (int)q < ((unsigned)unk_Tab91b6e0 + 0x4004); q += 2) {
        if (q[-1] != -1 && *q != -1)
            cnt2++;
    }
    if (UNK_FW(&cnt, 4, 1, f) == (unsigned)-1)
        return 0;
    if (UNK_FW(&cnt2, 4, 1, f) == (unsigned)-1)
        return 0;
    for (q = (int *)gv_Graveyard; ; ) {
        if (*q != -1) {
            if (UNK_FW(q, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW(q + 1, 0xe, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x12, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x16, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x1a, 2, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW(q + 7, 1, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x1d, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x21, 4, 1, f) == (unsigned)-1)
                return 0;
            if (UNK_FW((char *)q + 0x25, 4, 1, f) == (unsigned)-1)
                return 0;
        }
        q = (int *)((char *)q + 0x2d);
        if ((int)q > ((unsigned)&ai_Unk91f716 + 0x59e9)) {
            q = (int *)(unk_Tab91b6e0 + 4);
            do {
                if (q[-1] != -1 && *q != -1) {
                    if (UNK_FW(q - 1, 4, 1, f) == (unsigned)-1)
                        return 0;
                    if (UNK_FW(q, 4, 1, f) == (unsigned)-1)
                        return 0;
                }
                q += 2;
                if ((int)q > ((unsigned)unk_Tab91b6e0 + 0x4003))
                    return 1;
            } while (1);
        }
    }
}
