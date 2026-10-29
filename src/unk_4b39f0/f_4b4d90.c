/* 0x4b4d90, mission condition: 0x4f9d40 gives 0 */

float ord_WeightedRecordSum(int *out);                                                   /* 0x4f9d40, defined elsewhere */

int ai_Unk4b4d90(AICHAR *c, G13TYPE *t)
{
    char buf[0x30];

    if (ord_WeightedRecordSum(buf) == 0.0f)
        return 1;
    return 0;
}
