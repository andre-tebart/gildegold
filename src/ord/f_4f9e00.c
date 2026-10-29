/* 0x4f9e00, copies the record 0x91b4c0 (10 ints) to o, sets o[6] to the float 0x6f1b9c and returns o[9] as a float; 0 for no o */

float unk_Unk4f9e00(int *o)
{
    if (o == 0)
        return 0;
    memcpy(o, ord_StatisticsRecord, 0x28);
    o[6] = *(int *)&ai_RatingBase;
    return *(float *)(o + 9);
}
