/* 0x4ef090, checks a purchase: the tier a (clamped 0 .. 4) gives the slot count *n = a + 2, the price *price = (2 * b + 500) * (a * 0.25 + 1) * d * 32 from b (clamped 0 .. 100) and d (1 .. 3);
   1 when the object's slot count (+0x194) allows *n and its stock (0x509e30 of +0x178) covers the price */

int unk_Unk4ef090(char *o, int a, int b, int d, int *n, int *price)
{
    double v;

    if (a >= 4)
        a = 4;
    else if (a <= 0)
        a = 0;
    if (b >= 100)
        b = 100;
    else if (b <= 0)
        b = 0;
    if (d >= 3)
        d = 3;
    else if (d <= 1)
        d = 1;
    *n = a + 2;
    v = b;
    *price = (int)((v + v + 500.0f) * (a * 0.25f + 1.0f) * d * 32.0f);
    if (*(int *)(o + 0x194) >= *n && gm_Unk509e30(*(int *)(o + 0x178), ai_OwnPlayer) >= *price)
        return 1;
    return 0;
}
