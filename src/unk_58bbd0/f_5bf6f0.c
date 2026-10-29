/* 0x5bf6f0, the carrier list of a special-amount page (195 B): up to 4 records 0x505df0 of the carriers
   0x53e400 (1, 0, 0x6f) owned by param_1 + 4; then the param_2 records are zeroed and the first min(param_2,
   count) get the character, the 0x5be1d0 init and the 0x5c33c0 result at +0x30; returns the carrier count */

unsigned int ai_Unk5bf6f0(int param_1, unsigned int param_2, int *param_3, int param_4)
{
    int ids[4];
    unsigned int n;
    unsigned int i;
    char *car;
    int *rec;
    int x;

    n = 0;
    car = (char *)he_FindEvent(1, 0, 0x6f);
    while (car != 0 && n < 4) {
        if (U_I(car + 0xac) == U_I(param_1 + 4)) {
            x = gm_FindChar(U_I(car + 0xb0));
            if (x != 0)
                ids[n++] = x;
        }
        car = (char *)he_NextEvent();
    }
    if (param_2 != 0) {
        for (i = 0; i < param_2 * 0x38 / 4; i++)
            param_3[i] = 0;
        i = param_2 < n ? param_2 : n;
        rec = param_3;
        for (x = 0; x < (int)i; x++) {
            rec[0] = ids[x];
            ai_AmountRowStatus(rec);
            rec[0xc] = ai_Unk5c33c0(*(unsigned short *)param_1, *(unsigned short *)rec[0],
                                    (unsigned int *)param_4, 0);
            rec += 0xe;
        }
    }
    return n;
}
