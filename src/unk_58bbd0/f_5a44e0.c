/* 0x5a44e0, the espionage start (216 B, ROUGH): the building gate 0x520fe0; the request record {0, 0x400,
   (kind 0x13 ? 0x90010 : 0xa7fd43b4), the flag, 7, ..., 0x727} and 0x525220 (param_1, 0), the text 0x19f6 and
   the request 0x5b2180 (1, the record, the text, the callback 0x5a41d0, 0), then 0x525440 and 0x52ae40 */

void ai_Unk5a44e0(unsigned char *param_1)
{
    int req[20];
    int k;
    int i;
    char buf[0x400];

    if (sv_CheckCharacterFlag(param_1) != 0)
        return;
    for (i = 0; i < 20; i++)
        req[i] = 0;
    req[1] = 0x400;
    if (param_1 == 0)
        return;
    sv_Unk525220(param_1, 0);
    k = *(char *)(U_I((unsigned)&ai_BTypes) + *param_1 * 0x24d) != 0x13;
    req[2] = k ? 0xa7fd43b4 : 0x90010;
    req[3] = k;
    req[4] = 7;
    req[19] = 0x727;
    txt_Format(buf, 0x19f6);
    ai_Unk5b2180(1, req, buf, (void *)(unsigned)&ai_Unk5a41d0, 0);
    sv_ClosePlayerBar();
    sv_Unk52ae40();
}
