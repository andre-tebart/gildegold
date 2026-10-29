/* 0x597380, the master-exam demand (107 B): the config (0x400, 0xa7fdfbb6, 1, 6) and the text 0x5176d0
   (0x16f9) go to 0x5c1130 (the config, the text, 0, the callback 0x5b3ad0), then 0x52d7a0 */

void ai_Unk597380(void)
{
    int cfg[0x14];
    char text[0x80];

    memset(cfg, 0, 0x50);
    cfg[1] = 0x400;
    cfg[4] = 6;
    cfg[2] = 0xa7fdfbb6;
    cfg[3] = 1;
    txt_Format(text, 0x16f9);
    ai_AmountMenu(cfg, text, 0, (void *)(unsigned)&ai_Unk5b3ad0);
    sv_CameraFollow();
}
