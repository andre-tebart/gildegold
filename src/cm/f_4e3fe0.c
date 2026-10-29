/* 0x4e3fe0, switches the scene mode on: 0x42ea10 (0x5315c0, 1), then 0x404c20 (o, 1) for every object whose +0x8c bit 4 is not set */

int ai_Unk4e3fe0(void)
{
    char **pp;

    tb_SetFunctionOff(sv_UpdateClock, 1);
    for (pp = ai_Unk1468ae0; (int)pp < (int)(ai_Unk1468ae0 + 0x300); pp++) {
        if (*pp != 0 && !(*(unsigned char *)(*pp + 0x8c) & 4))
            ch_PauseAnim(*pp, 1);
    }
}
