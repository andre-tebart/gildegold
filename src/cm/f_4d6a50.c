/* 0x4d6a50, builds the fight UI: the view "cutscenes\combat" (0x6f0d64), the button 0x60cb68 (text 0x143e8b4) and the
   bar 0x60cb64 at the bottom, then the portrait boxes (0x4cfe70 / 0x4cfc10) */

void ai_Unk4d6a50()
{
    int v, t;

    v = d2_OpenForm(0, 0, "cutscenes\\combat");
    ai_Unk6f0d64 = v;
    ((void (*)(int, int))d2_PlaceMainWindow)(v, 3);
    d2_SetForm(v, 0);
    t = d2_WinCreateSlider(0x10, 0xc, 0xbf, ai_Unk143e8b4, d2_LastWindow);
    ai_Unk60cb68 = t;
    G19_R(t, 0x5c, int) = 1;
    d2_CenterObj(ai_Unk60cb68);
    t = d2_WinOpenSlider((*(short *)(d2_LastWindowPtr + 8) - 0x190) / 2, 0x37, 0x190, 0, ai_Unk60d364, 0x83, 0x42, d2_LastWindow);
    ai_Unk60cb64 = t;
    G19_R(t, 0x74, short) = 0x54;
    G19_R(*(int *)(ai_Unkf8f39c - 0x10 + d2_LastWindow * 0x3c4), 0x4c, int) = 1;
    d2_SetForm(ai_Unk60d368, 0);
    io_PrintText("$C");
    ai_Unk4cfe70();
    ai_Unk4cfc10();
}
