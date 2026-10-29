/* 0x549110, a Ghidra miss: the init: state 0, +0xac = 0 */

void he_Unk549110(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(int *)(ev + 0xac) = 0;
}
