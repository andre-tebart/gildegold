/* 0x4e3060, fades the screen to black ("BLACK", 0x5a ticks) and waits until the fade has reached its end flag (+0 & 4) */

int ai_Unk4e3060(void)
{
    if (ai_Unk6f0d8c == 0) {
        ai_Unk6f0da0 = (char *)d2_StartFade(0, 0, d2_ScreenW, d2_ScreenH, "BLACK", 0x5a, 1);
        if (!(*ai_Unk6f0da0 & 4)) {
            do {
                sv_RunFrame(0, ai_Unk60d360 | 0x80);
            } while (!(*ai_Unk6f0da0 & 4));
        }
    }
}
