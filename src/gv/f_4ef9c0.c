/* 0x4ef9c0, fills n (up to the free ones) free entries of the list 0 (entry id +4 = -1; 0x4ef760) of the graveyard of the object o (building kind 0x1d, kind byte 1 .. 3) with new dead persons: id from
   ai_NextObjectId, a record (+0x10: 0x44 bytes) with five random bytes (+0x24 .. : rand(8) * 21 + 42), age fields, a random name pair (0x143ac5c / 0x143af70 and 0x143b178) and the grave kind (0x4fb330); returns how many. ROUGH */

int unk_Unk4ef9c0(unsigned char *o, int n)
{
    unsigned char kind;
    char *e;
    char *z;
    char *g;
    int i, cnt, k;
    unsigned r;

    kind = (unsigned char)ai_BTypes[o[0] + o[0] * 0x24c + 0x247];
    if (ai_BTypes[o[0] + o[0] * 0x24c] != 0x1d || kind == 0 || kind > 3)
        return 0;
    i = 0;
    cnt = 0;
    if (n > 0) {
        do {
            e = (char *)gv_FindEntry(0, o, 0, gv_EntryIsFree, 0);
            if (e == 0)
                return cnt;
            z = *(char **)(e + 0x10);
            *(int *)z = ai_NextObjectId;
            ai_NextObjectId++;
            *(int *)(e + 4) = *(int *)z;
            *(short *)(e + 8) = 0x7fff;
            e[0xc] = (char)0xff;
            *(short *)(e + 10) = (short)(*(int *)ai_Time + 5);
            for (k = 0; k < 5; k++) {
                z[0x24 + k] = (char)(int)((float)util_Random(8) * 21.0f + 42.0f);
            }
            *(int *)(z + 0x2c) = util_Random(9) + 1;
            *(int *)(z + 0x30) = util_Random(2) + 1;
            *(int *)(z + 0x34) = util_Random(0x5e) + 1;
            *(int *)(z + 0x40) = *(int *)ai_Time - util_Random(4) - 1;
            *(int *)(z + 0x38) = util_Random(2);
            strncpy(z + 0x14, ai_Unk143b178[util_Random(0x95)], 0xf);
            g = (char *)unk_Unk4fb330(*(int *)(z + 0x38), *(int *)(z + 0x34));
            *(int *)(z + 0x3c) = 0xff;
            r = util_Random(4);
            k = 4;
            do {
                if (*(int *)(g + 0x34 + r * 4) != 0xff) {
                    *(int *)(z + 0x3c) = *(int *)(g + 0x34 + r * 4) + 0x639;
                    break;
                }
                k--;
                r = (r + 1) & 3;
            } while (k != 0);
            if (*(int *)(z + 0x38) == 0)
                strncpy(z + 4, ai_Unk143ac5c[util_Random(0xbf)], 0xf);
            else
                strncpy(z + 4, ai_Unk143af70[util_Random(0x70)], 0xf);
            cnt++;
            i = cnt;
        } while (cnt < n);
    }
    return i;
}
