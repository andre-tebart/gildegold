/* 0x5a4130, the burglary start (158 B, ROUGH): the building gate 0x520fe0; the request record {0, 0x400,
   0xa07443b4, 1, 7, 0x727} and 0x525220 (param_1, 0), the text 0x19e2 and the request 0x5b2180 (1, the record,
   the text, the callback 0x5a3f10, 0), then 0x525440 and 0x52ae40 */

void ai_Unk5a4130(int param_1)
{
    int req[20];
    char buf[0x40];
    int i;

    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    for (i = 0; i < 20; i++)
        req[i] = 0;
    if (param_1 == 0)
        return;
    req[1] = 0x400;
    sv_OpenPlayerBar(param_1, 0);
    txt_Format(buf, 0x19e2);
    req[4] = 0xa07443b4;
    req[5] = 1;
    req[6] = 7;
    req[7] = 0x727;
    ai_Unk5b2180(1, req, buf, (void *)(unsigned)&ai_Unk5a3f10, 0);
    sv_ClosePlayerBar();
    sv_ClearSelection();
}
