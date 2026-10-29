/* 0x52ae40, clears the group selection: the focus 0x7662c0, 0x6f2304, the selected flag (+0x21c stride: +0x188 of the character records at 0x770d08) and 0x6f2334 */

void sv_Unk52ae40(void)
{
    char *q;

    q = (char *)(unsigned)&ai_Selected;
    SV_I((unsigned)&ai_Unk7662c0) = 0;
    SV_I((unsigned)&ai_Unk6f2304) = 0;
    do {
        *q = 0;
        q += 0x21c;
    } while ((int)q < (int)((unsigned)&ai_Selected + 0x65400));
    SV_I((unsigned)&ai_Unk6f2334) = 0;
}
