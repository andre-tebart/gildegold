/* 0x56cdd0, the alloc of an event of the type 0x4c: both clocks (+0x52 / +0x60) = the start clock (+0x44), due 10 units later (0x4fe140), state 0, +0xac = -1 */

void he_Unk56cdd0(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(HETIME *)(ev + 0x60) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xac) = -1;
    gm_AddTime(ev + 0x52, 0, 0xa, 0);
}
