/* 0x547170, sets the start (+0x44) and the due clock (+0x52) of the event record to now and posts it 2 units later (4fe140) */

void he_Unk547170(int ev)
{
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(HETIME *)(ev + 0x52) = ai_Time;
    gm_AddTime(ev + 0x52, 2, 0, 0);
}
