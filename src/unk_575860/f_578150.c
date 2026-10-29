/* 0x578150, clears the player S+0x1c and resets the window id S+0x20 = -1 (26 B, no return value) */

void ai_Unk578150(void)
{
    U_I(S + 0x1c) = 0;
    U_I(S + 0x20) = -1;
}
