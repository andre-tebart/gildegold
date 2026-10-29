/* 0x5662d0, the allocation handler of the night watchman (type 0x73; a Ghidra miss): the clocks +0x52 / +0x44 are now, the building / player +0xac = random(5) - 5 (as the last argument of the run: a signed pick), the first run at the morning hour 0x617830 [season] (+0x56 = ftol, +0x58 = 0) */

void he_Unk5662d0(int ev)
{
    *(HETIME *)(ev + 0x52) = ai_Time;
    *(HETIME *)(ev + 0x44) = ai_Time;
    *(int *)(ev + 0xac) = (unsigned short)util_Random(5) - 5;
    *(short *)(ev + 0x56) = (short)(int)((float *)(unsigned)&ai_Unk617830)[gm_ClockSeason((unsigned)&ai_Time)];
    *(int *)(ev + 0x58) = 0;
}
