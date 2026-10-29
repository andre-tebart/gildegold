/* 0x4f4390, sorts n records of 0x2d bytes at r by the byte +0x1c, descending (a bubble sort with whole-record swaps) */

void unk_Unk4f4390(int n, char *r)
{
    int i, j;
    char *q;
    char tmp[0x2d];

    if (n != 1 && n - 1 >= 0) {
        i = 1;
        do {
            if (i < n) {
                q = r + 0x2d;
                for (j = n - i; j != 0; j--) {
                    if ((unsigned char)q[0x1c] < (unsigned char)r[0x1c]) {
                        memcpy(tmp, r, 0x2d);
                        memcpy(r, q, 0x2d);
                        memcpy(q, tmp, 0x2d);
                    }
                    q += 0x2d;
                }
            }
            r += 0x2d;
            i++;
        } while (i - 1 < n - 1);
    }
}
