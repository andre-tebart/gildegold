/* 0x558de0, the allocation handler: due clock = the clock at +0xb4 */

void he_Unk558de0(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0xb4);
}
