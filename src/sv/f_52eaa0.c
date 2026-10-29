/* 0x52eaa0, lays n picture tiles (the picture k, width +0x50 + 2) in a row at (x, y) in the form 0x6ed938, n / 2 whole tiles and, when n is odd, a half tile (picture k + 1); n comes from a float
   argument whose scale is lost in the pseudocode (placeholder: len) */

void sv_Unk52eaa0(int x, int y, float len, int k)
{
    unsigned n, odd;
    int w, i;

    w = *(short *)(SV_I((unsigned)&d2_Pics) + 0x50 + k * 0x54) + 2;
    n = (int)len;
    odd = n & 0x80000001;
    if ((int)odd < 0)
        odd = (odd - 1 | 0xfffffffe) + 1;
    i = 0;
    if (0 < (int)n / 2) {
        do {
            d2_WinOpenPic(w * i + x, y, k, SV_I((unsigned)&d2_LastWindow));
            i++;
        } while (i < (int)n / 2);
    }
    if (odd != 0)
        d2_WinOpenPic(w * i + x, y, k + 1, SV_I((unsigned)&d2_LastWindow));
}
