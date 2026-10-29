/* 0x4cda50, as 0x4cda00; a record of kind 1 / 2 whose entry has nothing left (+0x20) is not taken */

char *ai_Unk4cda50(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_Unk4cd9d0(*(unsigned short *)q);
        if (r != 0) {
            if ((*(int *)(r + 0x58) == 1 || *(int *)(r + 0x58) == 2) && *(int *)(q + 0x20) <= 0)
                return ai_Unk4cd9d0(0);
            return r;
        }
    }
    return ai_Unk4cd9d0(0);
}
