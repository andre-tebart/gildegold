/* 0x4a4e30, a building's value for a stake of k (10 / 30 / 50) */

int ai_Unk4a4e30(AIBLD *b, int k)
{
    int v = (int)(gm_EquipmentValue(b, 100) * 0.005f);

    if (k == 50)
        return v * 4;
    if (k == 30)
        return v * 2;
    if (k != 10)
        v = 0;
    return v;
}
