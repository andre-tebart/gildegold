/* 0x4fdc10, the object of the group a building kind supplies for the building record b (kind 1: 0x12, 2: 0x13, 0x1d: 0xfd, 0x16: 0x120), through 0x4ffe90 (its object id, 1, 0, n) */

char *unk_Unk4fdc10(unsigned char *b)
{
    char k;

    k = ai_BTypes[*b * 0x24d];
    if (k == 1)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x12);
    if (k == 2)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x13);
    if (k == 0x1d)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
    if (k == 0x16)
        return gm_Query(*(int *)(b + 0x5d), 1, 0, 0x120);
    return 0;
}
