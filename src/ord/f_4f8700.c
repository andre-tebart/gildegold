/* 0x4f8700, picks a random object id for the character c (-1 without one): the ids (+1) of the two objects c[0x16c] / c[0x170] (when they have a +0x61 record) and, up to 0x20 times, of up to 3 random objects (3 of 4 chances each) of a random one of the 20 object kinds of the table (4 x 5) as the kind 5 of the list 0x501190 (1, 5, kind), until 16 are collected; then one of them at random */

int unk_Unk4f8700(char *c)
{
    int kinds[20];
    int list[16];
    int n;
    int tries;
    int k;
    int idx;
    char *o;

    kinds[0] = 1;
    kinds[1] = 2;
    kinds[2] = 5;
    kinds[3] = 7;
    kinds[4] = 10;
    kinds[5] = 6;
    kinds[6] = 4;
    kinds[7] = 0x13;
    kinds[8] = 0x16;
    kinds[9] = 9;
    kinds[10] = 0xf;
    kinds[11] = 0x17;
    kinds[12] = 0x19;
    kinds[13] = 0x1a;
    kinds[14] = 0x18;
    kinds[15] = 8;
    kinds[16] = 0xe;
    kinds[17] = 0x12;
    kinds[18] = 0x14;
    kinds[19] = 0x15;
    if (c == 0)
        return -1;
    n = 0;
    tries = 0x20;
    if (*(char **)(c + 0x16c) != 0 && *(int *)(*(char **)(c + 0x16c) + 0x61) != 0) {
        list[0] = *(int *)(*(char **)(c + 0x16c) + 1);
        n = 1;
    }
    if (*(char **)(c + 0x170) != 0 && *(int *)(*(char **)(c + 0x170) + 0x61) != 0) {
        list[n] = *(int *)(*(char **)(c + 0x170) + 1);
        n++;
    }
    while (n < 16 && tries != 0) {
        tries--;
        idx = util_RandomU(4) * 5 + util_RandomU(5);
        k = 0;
        o = (char *)gm_BuildQuery(1, 5, kinds[idx]);
        while (o != 0 && k < 3) {
            if (n >= 16)
                goto done;
            if (util_RandomU(4) < 3) {
                list[n] = *(int *)(o + 1);
                n++;
                k++;
            }
            o = (char *)gm_BuildNext();
        }
    }
done:
    return list[util_RandomU(n)];
}
