/* 0x4948c0, the record of the table at 0x5ecfd8 with this key */

AIREC10 *ai_Unk4948c0(int key)
{
    int i;

    for (i = 0; i < 7; i++)
        if (ai_Unk5ecfd8[i].key == key)
            return &ai_Unk5ecfd8[i];
    return 0;
}
