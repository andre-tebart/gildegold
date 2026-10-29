/* 0x589cf0, the speed keys each frame (264 B): the game speed 0x6f3608 in steps of 40, v = (int)(speed / 40 +
   0.5f); key 0x1468410 0 = none and the pause 0x75e8c0 == 1 return at once; the key 0x1f takes a screenshot
   (0x589bc0), the key 0x10 runs 0x5d7430 while 0x6ed9dc is 0 and the mask 0x760638 has no 0x10000; then the mode
   byte 0x6f23f8: bit 2 (the network game) - with bit 0x10 the keys 0x4e / 0x4a only raise / lower 0x4c3f20 /
   0x4c3f50, without it they show the text U_I (0x143a3d8) via 0x52e330; without bit 2 they raise / lower and step
   the speed between 0 and 4 * 40 by 40, then 0x5d5e90 applies it (the keys are read once more after each call) */

void ai_Quicksave(void); void ai_ApplyOptions(); void ai_Unk4c3f20(void); void ai_Unk4c3f50(void);

void ai_Unk589cf0(void)
{
    unsigned char key;
    unsigned char fl;
    int v;

    v = (int)((float)(U_I((unsigned)&ai_OptionSpeed) / 40) + 0.5f);
    key = *(unsigned char *)(unsigned)&kb_Last;
    if (key == 0)
        return;
    if (U_I((unsigned)&sv_PanelFlag) == 1)
        return;
    if (key == 0x1f) {
        ai_Unk589bc0();
        key = *(unsigned char *)(unsigned)&kb_Last;
    }
    if (key == 0x10 && U_I((unsigned)&d2_FocusedInput) == 0 && (U_I((unsigned)&sv_FrameMask) & 0x10000) == 0) {
        ai_Quicksave();
        key = *(unsigned char *)(unsigned)&kb_Last;
    }
    fl = *(unsigned char *)(unsigned)&ai_GameFlags;
    if ((fl & 4) == 0) {
        if (key == 0x4e) {
            ai_Unk4c3f20();
            if (v < 4) {
                v++;
                U_I((unsigned)&ai_OptionSpeed) = v * 40;
                ai_ApplyOptions();
            }
            key = *(unsigned char *)(unsigned)&kb_Last;
        }
        if (key == 0x4a) {
            ai_Unk4c3f50();
            if (0 < v) {
                U_I((unsigned)&ai_OptionSpeed) = v * 40 - 40;
                ai_ApplyOptions();
                return;
            }
        }
    } else if ((fl & 0x10) != 0) {
        if (key == 0x4e) {
            ai_Unk4c3f20();
            return;
        }
        if (key == 0x4a) {
            ai_Unk4c3f50();
            return;
        }
    } else {
        if (key == 0x4e || key == 0x4a)
            sv_SetStatusText(U_I((unsigned)&full_data_143a3d8));
    }
}
