/* 0x4f9ce0, a rating from the table records 0 and 1 of 0x4f2720 (their int +0x18): (4 - rec0[6]) * 0.25 * 0.75 + (1 - rec1[6]) * 0.25 */

float ord_StatisticsRating(void)
{
    int ra[9];
    int rb[9];

    gv_GetTableRecord(0, ra);
    gv_GetTableRecord(1, rb);
    return (float)(4 - ra[6]) * 0.25f * 0.75 + (1 - rb[6]) * 0.25;
}
