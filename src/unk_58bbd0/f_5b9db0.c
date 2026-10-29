/* 0x5b9db0, a window gated by 0x6f22e8 (368 B, ROUGH SKELETON): with 0x6f22e8 == 0 it returns early, a
   0xf8-byte frame; the body is a comment */

int ai_Unk5b9db0(void)
{
    if (U_I((unsigned)&ai_Unk6f22e8) == 0)
        return 0;
    /* ROUGH SKELETON: the 0xf8-byte frame */
    return 0;
}
