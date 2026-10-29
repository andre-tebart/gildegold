/* 0x55e3a0, the allocation handler: the due clock is the start clock (+0x44) */

void he_Unk55e3a0(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
}
