/* 0x5a6480, a kidnapping request start: 0x520fe0 (param_1); 0x525220 (param_1, 0) / 0x5b2180 (4, 0,
   0x1440b7c, the callback 0x5a6270, 0), then 0x525440 / 0x52ae40 */

void ai_Unk5a6480(int param_1)
{
    if (sv_CheckCharacterFlag(param_1) != 0 || param_1 == 0)
        return;
    sv_Unk525220(param_1, 0);
    ai_Unk5b2180(4, 0, (char *)U_I((unsigned)&full_data_1440b7c), (void *)(unsigned)&ai_Unk5a6270, 0);
    sv_ClosePlayerBar();
    sv_Unk52ae40();
}
