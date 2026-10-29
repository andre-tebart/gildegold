/* 0x576030, sets the end-of-game gate 0x6f2660 (the setter counterpart of 0x576020) */

void unk_SetEndGameGate(void)
{
    U_I((unsigned)&full_data_6f2660) = 1;
}
