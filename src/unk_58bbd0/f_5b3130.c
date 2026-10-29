/* 0x5b3130, a window with five text slots (1272 B, ROUGH SKELETON): the carrier 0x53e400 (2, 2, 0, 0x60, the
   player 0x6f25b8) first, the five caption ids 0x25e / 0x264 / 0x26a / 0x270 / 0x276 in a 0x358-byte frame;
   the window body is left as a comment */

int ai_Unk5b3130(void)
{
    int rec;
    int texts[5];

    texts[0] = 0x25e;
    texts[1] = 0x264;
    texts[2] = 0x26a;
    texts[3] = 0x270;
    texts[4] = 0x276;
    rec = he_FindEvent(2, 2, 0, 0x60, U_W((unsigned)&ai_PlayerId));
    /* ROUGH SKELETON: the window, the five controls and the pump loop of the 0x358 frame */
    return 0;
}
