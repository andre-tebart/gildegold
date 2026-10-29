/* 0x594790, opens the bribe window (128 B): 0x520fe0 (param_1) == 0 and param_1 != 0; the 0x50-byte config
   zeroed with +0 = 1, +4 = -1, +0x10 = 6, +8 = 0xa07c43a0 (the float weight), +0xc = 1; 0x525220 (param_1, 0);
   0x5b2180 (1, the config, 0x1440be8, the callback 0x5944e0, 0); 0x525440 and 0x52ae40 */

void ai_Unk594790(int param_1)
{
    int cfg[0x14];

    if (sv_CheckCharacterFlag(param_1) == 0 && param_1 != 0) {
        memset(cfg, 0, 0x50);
        cfg[0] = 1;
        cfg[1] = -1;
        cfg[4] = 6;
        cfg[2] = 0xa07c43a0;
        cfg[3] = 1;
        sv_OpenPlayerBar(param_1, 0);
        ai_Unk5b2180(1, cfg, U_I((unsigned)&full_data_1440be8), (void *)ai_Unk5944e0, 0);
        sv_ClosePlayerBar();
        sv_ClearSelection();
    }
}
