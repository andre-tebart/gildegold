/* 0x4eee00, starts a part in the slot p[0]: p[1] is taken off the float +8 + 4 * p[2] of the slot (not below 0), the 0x68-byte record at p + 3 is copied to the first idle part (+0x150 + 4 * j = 0)
   as part j (state +0x74 = 1, time +0x78 = 0x6333cc, +0x7c = 0), flag 2 of +4 is set (flag 1 when w); 1 when started */

int unk_Unk4eee00(int *p, int w)
{
    float f;
    int id, k, j, i;
    char *e;
    int *q;
    int rec[26];
    int *s, *d;
    char *part;

    f = *(float *)&p[1];
    id = p[0];
    k = p[2];
    s = p + 3;
    d = rec;
    for (i = 0x1a; i != 0; i--)
        *d++ = *s++;
    e = unk_Unk4eedd0(id);
    if (e != 0) {
        j = 0;
        q = (int *)(e + 0x150);
        do {
            if (*q == 0) {
                *(int *)(e + 0x150 + j * 4) = 1;
                f = *(float *)(e + 8 + k * 4) - f;
                *(float *)(e + 8 + k * 4) = f;
                if (f <= 0.0f)
                    f = 0.0f;
                *(float *)(e + 8 + k * 4) = f;
                part = e + j * 0x68;
                s = rec;
                d = (int *)(part + 0x18);
                for (j = 0x1a; j != 0; j--)
                    *d++ = *s++;
                *(unsigned *)(e + 4) |= 2;
                *(int *)(part + 0x78) = ai_Unk6333cc;
                *(int *)(part + 0x74) = 1;
                *(int *)(part + 0x7c) = 0;
                if (w != 0)
                    *(unsigned *)(e + 4) |= 1;
                return 1;
            }
            j++;
            q++;
        } while (j < 3);
    }
    return 0;
}
