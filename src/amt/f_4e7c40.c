/* 0x4e7c40, the level of the office kind k (0 when the kind has no record); the callers use it as the rank of a character's
   office: the nearest partner 0x4a0240, ai_FillNearRing 0x4a3af0, the election score hmt_Score, f3init.c and unk_4a7140.c */

int amt_GetKindLevel(int k)
{
    AMTKIND a;

    amt_GetKind(k, &a);
    return a.level;
}
