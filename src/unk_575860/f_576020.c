/* 0x576020, clears the end-of-game gate 0x6f2660 (checked by the tick 0x575860 at 0x575915) */

void unk_ClearEndGameGate(void)
{
    U_I((unsigned)&full_data_6f2660) = 0;
}
