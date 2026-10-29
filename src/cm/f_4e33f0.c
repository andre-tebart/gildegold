/* 0x4e33f0, frees the thing 0x4e33d0 remembered */

int ai_Unk4e33f0(void)
{
    if (ai_Unk6f0d8c == 0) {
        d3t_DeleteRain(ai_Unk6f0dbc);
        ai_Unk6f0dbc = 0;
    }
}
