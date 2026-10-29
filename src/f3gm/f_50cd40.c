/* 0x50cd40, the wage of 0x508740 times 3 (9 for the jobs 10 .. 12 of the building kind k) */

float gm_Unk50cd40(char *c, int k, int a, int b)
{
    int j;

    j = gm_BuildingJob((unsigned char)ai_BTypes[k * 0x24d]);
    if (j != 10 && j != 0xb && j != 0xc)
        return gm_Unk508740(c, k, a, b) * 3.0f;
    return gm_Unk508740(c, k, a, b) * 9.0f;
}
