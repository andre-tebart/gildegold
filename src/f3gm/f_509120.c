/* 0x509120, 1 when the rank of c is below the limit of the building b (rank 1: < 2, 2: < 3, 3: <= 7, else <= 0) */

int gm_Unk509120(unsigned char *b, void *c)
{
    char *p;
    int r, lim;

    lim = 0;
    p = ai_BTypes + (unsigned)*b * 0x24d;
    r = gm_BuildingRank(c);
    if (p[0x247] == 1)
        return r < 2;
    if (p[0x247] == 2)
        return r < 3;
    if (p[0x247] == 3)
        lim = 7;
    return r <= lim;
}
