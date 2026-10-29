/* 0x4e51b0, adds the character who to the party id (+0x30 count, +0x34 ids, at most 16) unless he is in it: 1 when added */

int ai_Unk4e51b0(int id, int who)
{
    G28REC *r;
    int i, n;

    r = (G28REC *)cm_PartyById(id);
    i = 0;
    if (r != 0) {
        n = r->used;
        for (; i < n; i++) {
            if (*(int *)((char *)r + 0x34 + i * 4) == who)
                return 0;
        }
        if (r->used < 0x10) {
            *(int *)((char *)r + 0x34 + n * 4) = who;
            r->used++;
            return 1;
        }
    }
    return 0;
}
