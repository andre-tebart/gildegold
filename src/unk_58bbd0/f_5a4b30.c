/* 0x5a4b30, the pickpocket start (95 B): the request record {0, 0x400, 0x90010, 0, 7}, 0x525220 (param_1, 0),
   the text 0x1440974 and the request 0x5b2180 (1, the record, the text, the callback 0x5a48d0, 0), then
   0x525440 and 0x52ae40 */

void ai_Unk5a4b30(int param_1)
{
    int req[20];
    int i;

    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    req[4] = 7;
    req[2] = 0x90010;
    sv_OpenPlayerBar(param_1, 0);
    ai_Unk5b2180(1, req, (char *)U_I((unsigned)&full_data_1440974), (void *)(unsigned)&ai_Unk5a48d0, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
