/* 0x5a6720, the city watch arrest request (127 B): 0x520fe0 (param_1); the request (1, -1, 0xa0744300, 1, 6,
   0x727 at +0x4c) and 0x525220 (param_1, 0) / 0x5b2180 (1, the request, 0x1440b88, the callback 0x5a64d0, 0),
   then 0x525440 / 0x52ae40 */

void ai_Unk5a6720(int param_1)
{
    int req[20];

    if (sv_CheckCharacterFlag(param_1) != 0 || param_1 == 0)
        return;
    req[0] = 1;
    req[1] = -1;
    req[4] = 6;
    req[2] = 0xa0744300;
    req[3] = 1;
    req[19] = 0x727;
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk5b2180(1, req, (char *)U_I((unsigned)&full_data_1440b88), (void *)(unsigned)&ai_Unk5a64d0, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
