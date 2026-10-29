/* 0x578120, sets the player S+0x1c = per and resets the window id S+0x20 = -1 (43 B): -4 without the gate
   0x6f265c, else 0 */

int ai_Unk578120(char *per)
{
    if (!U_I((unsigned)&full_data_6f265c))
        return -4;
    *(char **)(S + 0x1c) = per;
    U_I(S + 0x20) = -1;
    return 0;
}
