/* 0x550ea0, sets state 0 and the due clock of an event record to now with the minute word 7 (a clock field, ROUGH) */

void he_Unk550ea0(int ev)
{
    *(int *)(ev + 0x70) = 0;
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(unsigned short *)(ev + 0x56) = 7;
    *(int *)(ev + 0x58) = 0;
}
