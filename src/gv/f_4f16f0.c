/* 0x4f16f0, a rate of the object o: the sum of 10 / 10 / 10 / 15 / 10 / 10 / 15 for the items 0x1ff, 0x202, 0x208, 0x20d, 0x214, 0x216, 0x219 in its list, times 0.0125 */

float unk_Unk4f16f0(char *o)
{
    int sum;

    sum = 0;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x1ff) != 0)
        sum = 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x202) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x208) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x20d) != 0)
        sum += 15;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x214) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x216) != 0)
        sum += 10;
    if (gm_Query(*(int *)(o + 0x5d), 2, 6, 0, 0x219) != 0)
        sum += 15;
    return (float)sum * 0.0125f;
}
