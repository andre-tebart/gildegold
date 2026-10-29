/* 0x4cdac0, the entry (not the record) of that search, 0: none */

char *ai_Unk4cdac0(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_Unk4cd9d0(*(unsigned short *)q);
        if (r != 0) {
            if ((*(int *)(r + 0x58) == 1 || *(int *)(r + 0x58) == 2) && *(int *)(q + 0x20) <= 0)
                return 0;
            return q;
        }
    }
    return 0;
}
