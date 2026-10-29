/* 0x5029f0, callback of 0x438d40 for 0x502a60: collects the objects whose prototype (low 24 bits of +0x48) is of the kind 0x21 and has
   its top byte 1 into list (0x40 objects, then their keys, then the count); true while there is room */

char gm_Unk5029f0(char *o, int *list)
{
    int key;

    key = *(int *)(o + 0x48) & ((unsigned)&txt_Keys + 0x181bf);
    if (o[0x4b] == 1 && ai_Goods[key * 0x40 + key] == 0x21) {
        list[list[0x80] + 0x40] = key;
        list[list[0x80]] = (int)o;
        list[0x80]++;
    }
    return list[0x80] < 0x40;
}
