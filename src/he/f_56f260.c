/* 0x56f260, the first free meeting slot of 0x1d (two rounds from a random start): the record 0x4f2720 (slot) has +0x18 == 1, +1 == 2 and +0xc == the wanted id `c` + 3 (one more each round); -1 when none */

int he_Unk56f260(int c)
{
    char rec[0x24];
    int want = c + 3;
    int round;
    int i;
    int slot;

    round = 2;
    do {
        slot = (unsigned short)util_Random(0x1d);
        for (i = 0x1d; i != 0; i--) {
            gv_GetTableRecord(slot, rec);
            if (*(int *)(rec + 0x18) == 1 && rec[1] == 2 && *(int *)(rec + 0xc) == want)
                return slot;
            slot = (slot + 1) % 0x1d;
        }
        want++;
    } while (--round != 0);
    return -1;
}
