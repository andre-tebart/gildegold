/* 0x5a5f60, a kidnapping request start (0x50 frame): 0x520fe0 (param_1); the request (0, 0x400, 0, 2) and
   0x525220 (param_1, 0) / 0x5b2180 (1, the request, 0x1440b60, the callback 0x5a5d70, 0), then 0x525440 /
   0x52ae40 */

void ai_Unk5a5f60(int param_1)
{
    int req[20];
    int i;
    int r;

    r = sv_CheckCharacterFlag(param_1);
    if (r != 0)
        return;
    for (i = 0; i < 0x14; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 2;
    req[2] = r;
    if (param_1 == 0)
        return;
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk5b2180(1, req, (char *)U_I((unsigned)&full_data_1440b60), (void *)(unsigned)&ai_Unk5a5d70, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
