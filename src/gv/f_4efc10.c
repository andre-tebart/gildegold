/* 0x4efc10, allocates the graveyard record of the object o (0x24 bytes, "gv_AllocGraveyardField"; kind 1 .. 3 of the building kind 0x1d): +0 the object id, +4 the kind, +8 the entries (0x14 bytes: the list number,
   id -1, short 0xffff, the record at +0x10) of 8 / 12 / 16 small (0x44 bytes) graves, then 4 / 6 / 8 of 0x88, 2 / 2 / 4 of 0x110 and (kind 3) 2 of 0x220 bytes, the entry tables +0xc / +0x10 / +0x14 for the
   larger lists, the graves +0x1c ("gv_Alloc:_all_graves") and zombies +0x20 ("gv_Alloc:_all_zombies"); stored at +0x65 of o; 0 when out of memory. ROUGH */

int *gv_AllocGraveyard(unsigned char *o)
{
    int *g;
    int *p;
    int *recs;
    char *b;
    int cnt[4];
    int nl, i, j, n, total;

    b = ai_BTypes + o[0] + o[0] * 0x24c;
    if (b[0] == 0x1d && (unsigned char)b[0x247] != 0 && (unsigned char)b[0x247] < 4
        && (g = (int *)mem_Alloc(0x24, "gv_AllocGraveyardField")) != 0) {
        memset(g, 0, 0x24);
        g[0] = *(int *)(o + 1);
        g[1] = (unsigned char)b[0x247];
        g[7] = (int)mem_Alloc(unk_Tab5ee064[(unsigned char)b[0x247]] * 0x14, "gv_Alloc:_all_graves");
        if (g[7] != 0) {
            memset((void *)g[7], 0, unk_Tab5ee064[(unsigned char)b[0x247]] * 0x14);
            g[8] = (int)mem_Alloc(unk_Tab5ee074[(unsigned char)b[0x247]] * 0x44, "gv_Alloc:_all_zombies");
            if (g[8] != 0) {
                memset((void *)g[8], 0, unk_Tab5ee074[(unsigned char)b[0x247]] * 0x44);
                for (i = 0; i < unk_Tab5ee074[(unsigned char)b[0x247]]; i++)
                    *(int *)((char *)g[8] + i * 0x44) = -1;
                p = (int *)g[7];
                recs = (int *)g[8];
                g[2] = (int)p;
                *(char *)(g + 6) = 1;
                *((char *)g + 0x19) = 0;
                *((char *)g + 0x1a) = 0;
                *((char *)g + 0x1b) = 0;
                if (g[1] == 1) {
                    cnt[0] = 8; cnt[1] = 4; cnt[2] = 0; cnt[3] = 0;
                    nl = 2;
                    g[4] = 0;
                    g[5] = 0;
                } else if (g[1] == 2) {
                    cnt[0] = 12; cnt[1] = 6; cnt[2] = 2; cnt[3] = 0;
                    nl = 3;
                    g[5] = 0;
                } else {
                    cnt[0] = 16; cnt[1] = 8; cnt[2] = 4; cnt[3] = 2;
                    nl = 4;
                }
                total = 0;
                for (j = 0; j < nl; j++) {
                    if (j >= 1)
                        g[2 + j] = (int)(p + total * 5);
                    n = cnt[j];
                    for (i = 0; i < n; i++) {
                        p[0] = j;
                        p[4] = (int)recs;
                        p[1] = -1;
                        *(short *)(p + 2) = (short)0xffff;
                        recs = (int *)((char *)recs + (0x44 << j));
                        p += 5;
                    }
                    total += n;
                }
                *(int **)(o + 0x65) = g;
                return g;
            }
        }
        if (g[7] != 0) {
            mem_Free((void *)g[7]);
            g[7] = 0;
        }
        if (g[8] != 0) {
            mem_Free((void *)g[8]);
            g[8] = 0;
        }
        mem_Free(g);
    }
    return 0;
}
