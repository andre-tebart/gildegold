/* 0x596400, the sabotage / blackmail contact listener (482 B): 0x52e190, the pump 0x5309a0 (this, 0x67fff);
   SABOTAGE (0x143fdb8, 0x17): the config (1, -1, 0xe07c43b4, 3, 6, +0x4c = 0x727) and the text 0x5176d0
   (0x16ff) go to 0x5b2180 (1, the config, the text, the callback 0x5b3630, 0); VERPRUEGELN (0x143fdd8, 0xd):
   the config (0x400, 0xa7fdfbb6, 1, 6, +0x4c = 0x727) and the text 0x1707 go to 0x5c1130 (the config, the text,
   0, the callback 0x5b3f30) and 0x52d7a0; BEI_NACHT_UND_NEBEL (0x1442160, 0xc) -> 0x5b3130 (param_1, param_2,
   2); BESTECHUNG (0x143feec, 0x10) -> 0x5b4bf0 (param_1) */

void ai_Unk596400(int param_1, int param_2)
{
    int r;
    int sabotage;
    int pruegel;
    int nebel;
    int bestechung;
    int cfg[0x14];
    char text[0x80];

    sv_ReleaseNamePlates();
    r = sv_RunFrame(ai_Unk596400, 0x67fff);
    while (r != 0) {
        sabotage = sv_RegisterNamePlate("contact_SABOTAGE", U_I((unsigned)&full_data_143fdb8), 0x17);
        pruegel = sv_RegisterNamePlate("contact_VERPRUEGELN", U_I((unsigned)&full_data_143fdd8), 0xd);
        nebel = sv_RegisterNamePlate("contact_BEI_NACHT_UND_NEBEL", U_I((unsigned)&full_data_1442160), 0xc);
        bestechung = sv_RegisterNamePlate("contact_BESTECHUNG", U_I((unsigned)&full_data_143feec), 0x10);
        if (U_I((unsigned)&ai_ClickedObject) != 0) {
            if (U_I((unsigned)&ai_ClickedObject) == sabotage) {
                memset(cfg, 0, 0x50);
                cfg[0] = 1;
                cfg[1] = -1;
                cfg[4] = 6;
                cfg[2] = 0xe07c43b4;
                cfg[3] = 3;
                cfg[0x13] = 0x727;
                txt_Format(text, 0x16ff);
                ai_Unk5b2180(1, cfg, text, (void *)(unsigned)&ai_Unk5b3630, 0);
            } else if (U_I((unsigned)&ai_ClickedObject) == pruegel) {
                memset(cfg, 0, 0x50);
                cfg[1] = 0x400;
                cfg[4] = 6;
                cfg[2] = 0xa7fdfbb6;
                cfg[3] = 1;
                cfg[0x13] = 0x727;
                txt_Format(text, 0x1707);
                ai_AmountMenu(cfg, text, 0, (void *)(unsigned)&ai_Unk5b3f30);
                sv_CameraFollow();
            } else if (U_I((unsigned)&ai_ClickedObject) == nebel) {
                ai_Unk5b3130(param_1, param_2, 2);
            } else if (U_I((unsigned)&ai_ClickedObject) == bestechung) {
                ai_Unk5b4bf0(param_1);
            }
        }
        r = sv_RunFrame(ai_Unk596400, 0x67fff);
    }
}
