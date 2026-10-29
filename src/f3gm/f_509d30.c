/* 0x509d30, the sum of the prices of the building kind's negative object words (+0x23, high bit set) */

int gm_BuildingKindValue(int kind)
{
    unsigned short w;
    unsigned short *p;
    int i;
    float tot;

    tot = 0.0f;
    p = (unsigned short *)(ai_BTypes + kind * 0x24d + 0x23);
    i = 0;
    do {
        w = *p;
        if (w == 0)
            break;
        if ((short)w < 0)
            tot += gm_ItemPrice((unsigned char)ai_Goods[(w & 0x7fff) * 0x41], 100);
        i++;
        p++;
    } while (i < 0x40);
    return (int)tot;
}
