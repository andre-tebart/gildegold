/* 0x54cc20, a Ghidra miss: the init: the due clock = the start clock (+0x44), state 0, +0xb4 = 0 */

void he_Unk54cc20(int ev)
{
    *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xb4) = 0;
}
