/* 0x56b750, scene callback: appends the object o to the list (0x40 entries, the count at +0x104); true while there is room */

char he_Unk56b750(int o, int *list)
{
    list[list[0x41] + 1] = o;
    list[0x41]++;
    return list[0x41] < 0x40;
}
