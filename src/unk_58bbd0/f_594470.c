/* 0x594470, opens the pickpocket window (99 B): the 0x50-byte config local zeroed with +4 = 0x400, +8 =
   0x93810, +0x10 = 7; 0x525220 (param_1, 0); the record 0x5b2180 (1, the config, 0x1440974, the callback
   0x5941c0, 0); 0x525440 and 0x52ae40 */

int he_Unk553cc0();

void ai_Unk594470(int param_1)
{
    int cfg[0x14];

    memset(cfg, 0, 0x50);
    cfg[3] = 0;
    cfg[1] = 0x400;
    cfg[4] = 7;
    cfg[2] = 0x93810;
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk5b2180(1, cfg, U_I((unsigned)&full_data_1440974), (void *)ai_Unk5941c0, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
