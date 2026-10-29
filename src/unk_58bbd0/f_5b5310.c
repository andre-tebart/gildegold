/* 0x5b5310, a Ghidra-missed start (753 B, ROUGH SKELETON): the seam is the ret at 0x5b530b and the four nops to
   0x5b5310 (the functions.csv has only 0x5b5010 before 0x5b5610, so Ghidra merged this into the 0x5b5010
   record); param_1 == 0 returns 0, then a 0x518-byte frame. The body is a comment */

int ai_Unk5b5310(int param_1)
{
    if (param_1 == 0)
        return 0;
    /* ROUGH SKELETON: the 0x518-byte frame */
    return 0;
}
