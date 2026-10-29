/* 0x5a8710, a weight added to one good of a setting (306 B): the record of 0x5a84b0 (the base record +4 of
   param_1); the weight param_3 of the good param_2 (the record's table +0xc, the weight +0x10, the lock byte
   +0x11) is offered the difference against the free total (100 minus the weights of the locked / used slots),
   taken one at a time from the other unlocked slots (the rotation index +8); the chosen weight is set and the
   rotation index advanced */

unsigned char *ai_Unk5a8710(int param_1, int good, unsigned char weight)
{
    char *rec;
    unsigned char *p;
    int total;
    int count;
    int i;
    int slot;
    int delta;
    int step;
    int v;
    int rest;
    unsigned char old;

    rec = (char *)ai_Unk5a84b0(*(int *)(param_1 + 1));
    p = 0;
    if (good == 0)
        return p;
    count = 0;
    for (i = 0; i < 7; i++)
        if (*(int *)(rec + 0xc + i * 8) != 0)
            count++;
    total = 100;
    for (i = 0; i < 7; i++) {
        if (*(int *)(rec + 0xc + i * 8) != 0 && *(unsigned char *)(rec + 0x11 + i * 8) != 0) {
            total -= *(unsigned char *)(rec + 0x10 + i * 8);
            count--;
        }
    }
    p = (unsigned char *)rec;
    if (weight <= total && count > 1) {
        slot = 0;
        p = (unsigned char *)(rec + 0x10);
        while (*(int *)(p - 4) != good || weight == *p) {
            slot++;
            p += 8;
            if (slot > 6)
                return p;
        }
        old = *(unsigned char *)(rec + 0x10 + slot * 8);
        delta = (int)old - (int)weight;
        step = old < weight ? -1 : 1;
        rest = delta < 0 ? -delta : delta;
        i = *(unsigned char *)(rec + 8);
        while (rest != 0) {
            if (slot != i && *(int *)(rec + 0xc + i * 8) != 0 &&
                *(unsigned char *)(rec + 0x11 + i * 8) == 0) {
                old = *(unsigned char *)(rec + 0x10 + i * 8);
                v = (int)old + step;
                if (v > -1 && v < total + 1) {
                    rest--;
                    *(unsigned char *)(rec + 0x10 + i * 8) = (unsigned char)(old + step);
                }
            }
            i = (i + 1) % 7;
        }
        *(unsigned char *)(rec + 0x10 + slot * 8) = weight;
        *(unsigned char *)(rec + 8) = (unsigned char)i;
    }
    return p;
}
