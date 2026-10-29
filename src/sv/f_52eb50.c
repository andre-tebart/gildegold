/* 0x52eb50, draws the two-sided bar (current value 0x504e00 (a, b) against the byte at b + 0x80 + a): equal (within 1 %) a single bar, the picture k (+1 for an odd tile), smaller / larger the base bar
   in k and the difference in k + 2 / k + 3 or k + 4 / k + 5 (the float operands deciding the tile counts are lost in the pseudocode: n = placeholders); ROUGH */

void sv_Unk52eb50(int x, int y, int a, int b, unsigned k)
{
    unsigned u1, u2, n, odd;
    int pic, w, i;
    double dd;

    pic = k;
    w = *(short *)(SV_I((unsigned)&d2_Pics) + 0x50 + k * 0x54) + 2;
    u1 = gm_Unk504e00(a, b);
    u2 = *(unsigned char *)(b + 0x80 + a);
    k = u1;
    if ((int)u1 <= (int)u2)
        k = u2;
    dd = (double)(int)(u1 - u2);
    if (dd < 0)
        dd = -dd;
    if (dd / (double)(int)k <= 0.01) {
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
        pic++;
    } else if ((int)u1 < (int)u2) {
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic + 2, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd != 0)
            d2_WinOpenPic(w * i + x, y, pic + 3, SV_I((unsigned)&d2_LastWindow));
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
        pic++;
    } else {
        if ((int)u1 <= (int)u2)
            return;
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic + 4, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd != 0)
            d2_WinOpenPic(w * i + x, y, pic + 5, SV_I((unsigned)&d2_LastWindow));
        n = (int)0.0f;
        odd = n & 0x80000001;
        if ((int)odd < 0)
            odd = (odd - 1 | 0xfffffffe) + 1;
        i = 0;
        if (0 < (int)n / 2) {
            do {
                d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
                i++;
            } while (i < (int)n / 2);
        }
        if (odd == 0)
            return;
    }
    d2_WinOpenPic(w * i + x, y, pic, SV_I((unsigned)&d2_LastWindow));
}
