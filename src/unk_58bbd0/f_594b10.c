/* 0x594b10, opens the spying window (150 B): 0x520fe0; the 0x50-byte config zeroed with +4 = 0x400, +0x4c =
   0x727, +0x10 = 6, +8 = 0; the text 0x5176d0 (0x19f6) into the 0x400 buffer; 0x525220 (param_1, 0), the
   record 0x5b2180 (1, the config, the text, the callback 0x594810, 0); 0x525440 and 0x52ae40 */

void ai_Unk594b10(int param_1)
{
    int cfg[0x14];
    char text[0x400];

    if (sv_CheckCharacterFlag(param_1) == 0) {
        memset(cfg, 0, 0x50);
        cfg[1] = 0x400;
        if (param_1 != 0) {
            sv_OpenPlayerBar(param_1, 0);
            cfg[0x13] = 0x727;
            cfg[4] = 6;
            cfg[2] = 0;
            txt_Format(text, 0x19f6);
            ai_Unk5b2180(1, cfg, text, (void *)ai_Unk594810, 0);
            sv_ClosePlayerBar();
            sv_ClearSelection();
        }
    }
}
