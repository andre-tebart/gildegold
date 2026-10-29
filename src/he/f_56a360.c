/* 0x56a360, the alloc of the same event: both clocks (+0x52 / +0x60) = the start clock (+0x44), due 7 units of the kind 7 and a random 30 .. 59 later */

void he_Unk56a360(int r)
{
    *(HETIME *)(r + 0x52) = *(HETIME *)(r + 0x44);
    *(HETIME *)(r + 0x60) = *(HETIME *)(r + 0x44);
    gm_AddTime(r + 0x52, 7, (unsigned short)util_Random(0x1e) + 0x1e, 0);
}
