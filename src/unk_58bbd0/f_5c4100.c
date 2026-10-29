/* 0x5c4100, the law selection entry (80 B): param_1 the building record; 0x50b090 (*param_1, &param_1)
   redirects it, else 0x5c3bb0 (param_1, param_2, param_3, param_4); the redirect returns 0x52a040 */

int ai_Unk5c4100(unsigned char *param_1, int param_2, int param_3, int param_4)
{
    if (param_1 == 0)
        return 0;
    if (gm_ResearchClass(*param_1, &param_1) != 0)
        return sv_ReturnZero();
    return ai_Unk5c3bb0(param_1, (int *)param_2, param_3, (void *)param_4);
}
