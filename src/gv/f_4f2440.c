/* 0x4f2440, the cost difference of two kinds: 0x4f2330 (c, 100, k1, 1) minus half of the one of k2; *out the difference times the factor of k2 */

int unk_Unk4f2440(unsigned short *c, int k1, int k2, int *out)
{
    int a, b, d;

    a = unk_Unk4f2330(c, 100, k1, 1, &d);
    b = unk_Unk4f2330(c, 100, k2, 1, &d);
    d = b - a / 2;
    *out = (int)(d * unk_Tab5ee034[k2]);
    return d;
}
