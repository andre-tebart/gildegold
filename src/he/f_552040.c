/* 0x552040, re-posts an event 2 later and resets its state to 0 */

void he_Unk552040(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 2, 0, 0);
    *(int *)(ev + 0x70) = 0;
}
