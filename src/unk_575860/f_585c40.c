/* 0x585c40, fills the eight seance formulas (101 B): n / a / b the two arrays of ints, base = 0x40d7; each pair
   i, i+1 gets the random 0x42ec00 (3) & 0xffff r: a[i] = base + r, a[i+1] = base + r + 3, b[i] = b[i+1] = r;
   base += 6 per pair */

void ai_Unk585c40(int n, int *a, int *b, int base)
{
    int i;
    int r;

    for (i = 0; i < n; i += 2) {
        r = util_Random(3) & 0xffff;
        b[i] = r;
        b[i + 1] = r;
        a[i] = base + r;
        a[i + 1] = base + r + 3;
        base += 6;
    }
}
