/* 0x541a50, a Ghidra miss (disassembled): sets the due clock (+0x52) of the event record to now */

void he_Unk541a50(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
}
