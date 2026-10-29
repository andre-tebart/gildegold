/* 0x554820, the allocation handler of the 0x554860 event: the due clock is the start clock (+0x44) and the event posted 0xf later */

void he_Unk554820(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 0xf, 0);
}
