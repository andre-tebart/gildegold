/* 0x52fad0, shows the scroll and sets the animation frame (the clock 0x6ed980 +0x248 / 6 % 12) of both end pieces */

void sv_Unk52fad0(void)
{
    int f;

    if (SV_I((unsigned)&ai_Unk623ea0) != -1) {
        d2_SetForm(SV_I((unsigned)&ai_Unk623ea0), 1);
        f = (*(int *)(SV_I((unsigned)&d2_LastWindowPtr) + 0x248) / 6) % 0xc;
        if (f < 0)
            f = 0;
        SV_WI(SV_I((unsigned)&ai_Unk623ea4), 0x78) = f;
        SV_WI(SV_I((unsigned)&ai_Unk623ea8), 0x78) = f;
    }
}
