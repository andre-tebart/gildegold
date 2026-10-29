/* 0x4e36a0, resets the 16 member slots of 0x92cae0 for the party m (+0x34 the ids, +0x30 the count, +8 the kind): used, nothing announced, the id
   (-1 without a party); for a kind 3 party the 16 sub entries (+0x98.., 0x2c bytes) of its used slots are cleared. ROUGH */

int ai_Unk4e36a0(char *m)
{
    int j, i, q;
    char *s, *t;
    int *ids;

    ids = (int *)(m + 0x34);
    for (j = 0, s = ai_Unk92cae0; j < 16; j++, s += 0x35c, ids++) {
        *(int *)s = 1;
        *(int *)(s + 8) = 0;
        *(int *)(s + 4) = 0;
        *(int *)(s + 0xc) = 1;
        if (m != 0) {
            *(int *)(s + 0x10) = *ids;
            if (m[8] == 3 && (unsigned char)m[0x30] > 0) {
                for (i = 0, t = ai_Unk92cae0; i < (unsigned char)m[0x30]; i++, t += 0x35c) {
                    if (*(int *)(t + 8) == -1)
                        continue;
                    for (q = 0; q < 16; q++) {
                        *(int *)(t + 0x94 + q * 0x2c) = -1;
                        *(int *)(t + 0x98 + q * 0x2c) = 0;
                    }
                }
            }
        } else
            *(int *)(s + 0x10) = -1;
        s[0x14] = 0;
    }
}
