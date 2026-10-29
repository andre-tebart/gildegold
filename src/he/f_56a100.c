/* 0x56a100, the alloc of an event record: both clocks (+0x52 / +0x60) = the start clock (+0x44), due a random 0 .. 59 units of the kind 4 later */

void he_Unk56a100(int r)
{
    *(HETIME *)(r + 0x52) = *(HETIME *)(r + 0x44);
    *(HETIME *)(r + 0x60) = *(HETIME *)(r + 0x44);
    gm_AddTime(r + 0x52, 4, (unsigned short)util_Random(0x3c), 0);
}
