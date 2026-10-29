/* 0x59df10, opens the fortune overview (0x18ec) through the window helper 0x5c1130 (a 0x50-byte request
   record: +4 = 0x400, +8 = 0xa7fdfbb6, +0xc = 1, +0x10 = 6, the text 0x18ec, the callback 0x59dc00) and
   0x52d7a0 */

void ai_Unk59df10(int param_1)
{
    int req[20];
    char text[0x40];
    int i;

    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 6;
    req[2] = 0xa7fdfbb6;
    req[3] = 1;
    txt_Format(text, 0x18ec);
    ai_AmountMenu(req, text, 0, (void *)(unsigned)&ai_Unk59dc00);
    sv_CameraFollow();
}
