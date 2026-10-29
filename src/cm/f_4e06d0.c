/* 0x4e06d0, ends a wedding scene s (s[0] the scene, s[1] its view, s[2] a sample bank, s[3] a playing sample): the exit scene
   "cutscenes\\hochzeit\\hochzeit-exit.esc", the globals reset. Returns 1 */

int ai_Unk4e06d0(int *s)
{
    int v;

    ai_Unk4e3060();
    if (s[3] != 0)
        sb_Stop(s[3], 0);
    v = s[2];
    s[3] = 0;
    sb_UnloadSampleBank(v);
    if (s[1] != -1)
        d2_CloseView(s[1]);
    if (s[0] != -1)
        ai_Unk4e2d90(s[0]);
    s[0] = ai_Unk4e2c30("cutscenes\\hochzeit\\hochzeit-exit.esc", &ch_EmptyName);
    ai_Unk4e2c70(s[0]);
    s[0] = -1;
    s[1] = -1;
    ai_Unk6f0da4 = 0;
    ai_Unk4e30c0();
    ai_Unk4e3640();
    d2_CutsceneActive = 0;
    return 1;
}
