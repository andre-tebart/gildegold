/* 0x4f88c0, shifts the 15 records of 0x14 bytes of the table 0x91ab00 down by one (a history: the oldest drops out) and fills the last one (0x91ac2c) from the 0x91b4c0 table (+4 .. +0xc and +0x24) and the float 0x6f1b9c truncated */

void unk_Unk4f88c0(void)
{
    memmove(ord_StatisticsHistory, ord_StatisticsHistory + 0x14, 0x12c);
    *(int *)(ord_StatisticsHistory + 0x12c) = *(int *)(ord_StatisticsRecord + 4);
    *(int *)(ord_StatisticsHistory + 0x130) = *(int *)(ord_StatisticsRecord + 8);
    *(int *)(ord_StatisticsHistory + 0x134) = *(int *)(ord_StatisticsRecord + 0xc);
    *(int *)(ord_StatisticsHistory + 0x138) = *(int *)(ord_StatisticsRecord + 0x24);
    *(int *)(ord_StatisticsHistory + 0x13c) = (int)ai_RatingBaseF;
}
