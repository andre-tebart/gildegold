/* 0x589400, clears one 12-byte slot of the table 0x6f5ec0 (14 B): its +4 / +8 = -1 */

void ai_Unk589400(int *p)
{
    p[1] = -1;
    p[2] = -1;
}
