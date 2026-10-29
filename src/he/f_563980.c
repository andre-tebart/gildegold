/* 0x563980, the allocation handler: due clock now + 1 */

void he_Unk563980(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 0, 0, 1);
}
