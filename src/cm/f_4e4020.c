/* 0x4e4020, the reverse: the objects with the bit 4 get 0x404c20 (o, 0) */

int ai_Unk4e4020(void)
{
    char **pp;

    tb_SetFunctionOff(sv_UpdateClock, 0);
    for (pp = ai_Unk1468ae0; (int)pp < (int)(ai_Unk1468ae0 + 0x300); pp++) {
        if (*pp != 0 && (*(unsigned char *)(*pp + 0x8c) & 4))
            ch_PauseAnim(*pp, 0);
    }
}
