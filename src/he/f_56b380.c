/* 0x56b380, the alloc of an event record: both clocks (+0x52 / +0x60) = the start clock (+0x44), due now + 1, +0xbc / +0xc0 / +0xc4 = 0 */

void he_Unk56b380(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    gm_AddTime(ev + 0x52, 0, 1, 0);
    *(int *)(ev + 0xbc) = 0;
    *(int *)(ev + 0xc0) = 0;
    *(int *)(ev + 0xc4) = 0;
}
