/* 0x49f4f0, eval callback 0x34: only for a kind-5 character without 0x169; with 3..5 entries in its list 0x164
   a random chance of the plain rating, with 6 or more the doubled / tripled rating */

int ai_Unk49f4f0(float *a, float *b, int i, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    int n;

    *a = -100.0f;
    *b = -100.0f;
    if (f != 0) {
        *a = -1e30f;
        *b = -1e30f;
        return 0;
    }
    if (((unsigned char *)p)[2] != 5 || ((unsigned char *)p)[0x169] != 0)
        return 0;
    n = gm_TitlesLeft(((unsigned char *)p)[0x164]);
    if (n < 3)
        return 0;
    if (n < 6) {
        if (util_RandomU(n - 1) == 0)
            return 0;
        return ai_Unk4a49a0(a, b, i, (AIPER *)p, 0, o1, o2);
    }
    G8_4A4AE0(a, b, i, p, 0, o1, o2);
    *a += *a;
    *b *= 3.0f;
    return 1;
}
