/* 0x4cda00, the first record (ai_Unk4cd9d0) of the entries of kind 5 of the list +0x178 of the character */

char *ai_Unk4cda00(char *c)
{
    char *q, *r;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        r = ai_Unk4cd9d0(*(unsigned short *)q);
        if (r != 0)
            return r;
    }
    return ai_Unk4cd9d0(0);
}
