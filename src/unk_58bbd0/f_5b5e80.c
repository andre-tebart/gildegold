/* 0x5b5e80, (2753 B, ROUGH SKELETON): the game clock 0x770ae0 is copied FIELD BY FIELD into the frame (+0x1c ..
   +0x34, the `mov ds:0x770ae0` form, not a HETIME assignment), then a 0x3e4-byte frame; the body is a comment */

int ai_Unk5b5e80(void)
{
    int clock[4];

    clock[0] = U_I((unsigned)&ai_Time);
    clock[1] = U_I((unsigned)&ai_TimeHour);
    clock[2] = U_I(((unsigned)&ai_Time + 0x8));
    U_W((int)clock + 0xc) = U_W(((unsigned)&ai_Time + 0xc));
    /* ROUGH SKELETON: the 0x3e4-byte frame */
    return 0;
}
