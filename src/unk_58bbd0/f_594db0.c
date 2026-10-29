/* 0x594db0, opens the raid window (158 B): as 0x594b10 but +8 = 0xa07443a0, +0xc = 1, the text 0x5176d0
   (0x1a83) and the callback 0x594bb0 */

void ai_Unk594db0(int param_1)
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
            cfg[2] = 0xa07443a0;
            cfg[3] = 1;
            txt_Format(text, 0x1a83);
            ai_Unk5b2180(1, cfg, text, (void *)ai_Unk594bb0, 0);
            sv_ClosePlayerBar();
            sv_ClearSelection();
        }
    }
}
