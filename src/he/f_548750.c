/* 0x548750, a Ghidra miss: the due clock = the start clock (+0x44), posted 3 units ahead */

void he_Unk548750(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 3, 0, 0);
}
