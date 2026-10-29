/* 0x53cb40, the outro: fades the music out, waits for the black fade, plays outro.mpg, restores the window and the music volume (byte 0x6f3602 / 127) (ROUGH) */

void main_Unk53cb40(void)
{
    char sv;
    unsigned char *f;
    char buf[0x104];

    if (U_I((unsigned)&full_data_6f2574) == 0)
        mov_Init();
    snd_FadeMusic(0.0f, 1000);
    f = (unsigned char *)d2_StartFade(0, 0, (int)*(short *)(unsigned)&d2_ScreenW, (int)*(short *)(unsigned)&d2_ScreenH, "BLACK", 0x5a, 1);
    while (sv = *(char *)(unsigned)&snd_AmbientOn, (*f & 4) == 0 || 0.0 <= *(float *)(unsigned)&msx_FadeTo)
        sv_RunFrame(0, U_I((unsigned)&sv_FrameMask));
    *(char *)(unsigned)&snd_AmbientOn = 0;
    sprintf(buf, "%soutro.mpg", (char *)(unsigned)&full_data_624798);
    U_I((unsigned)&di_Disabled) = 1;
    di_PollKeyboard();
    U_I((unsigned)&di_Disabled) = 0;
    di_AcquireMouse(0);
    di_AcquireKeyboard(0);
    sv_PumpMessages();
    Sleep(500);
    U_I((unsigned)&full_data_6f241c) = mov_Open(buf, 0, U_I((unsigned)&main_Instance), 0, 0, 0, 0, 0x8a);
    Sleep(200);
    mov_Play(U_I((unsigned)&full_data_6f241c));
    d2_ReleaseFade(f);
    sv_PumpMessages();
    SetFocus((HWND)U_I((unsigned)&main_Window));
    di_AcquireMouse(1);
    di_AcquireKeyboard(1);
    U_I((unsigned)&di_Disabled) = 1;
    di_PollKeyboard();
    U_I((unsigned)&di_Disabled) = 0;
    U_I((unsigned)&kb_Last) = 0;
    snd_FadeMusic((float)*(unsigned char *)((unsigned)&ai_SoundOptions + 0x2) * 0.007874016, 1000);
    *(char *)(unsigned)&snd_AmbientOn = sv;
    if (U_I((unsigned)&full_data_6f2574) == 0)
        mov_Exit();
}
