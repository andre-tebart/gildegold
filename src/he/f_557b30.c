/* 0x557b30, the allocation handler of the 0x557b70 event: due clock = start clock, posted 2 units later */

void he_Unk557b30(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 2, 0, 0);
}
