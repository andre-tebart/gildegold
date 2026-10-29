/* 0x5090b0, a rank byte (+0x247) of the building b */

char gm_BuildingRank(unsigned char *b)
{
    char *p;

    p = ai_BTypes + (unsigned)*b * 0x24d;
    switch (*p) {
    case 2:
    case 5:
    case 7:
        return p[0x247];
    default:
        return p[0x247];
    }
}
