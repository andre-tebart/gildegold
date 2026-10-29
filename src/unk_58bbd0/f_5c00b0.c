/* 0x5c00b0, a tab fill of the special-amount window (259 B, ROUGH SKELETON): as 0x5bffb0, the page switch
   0x5bfed0 (param_1, param_2, param_3) and the rows */

int ai_Unk5c00b0(int param_1, int param_2, int param_3, int param_4)
{
    if (param_4 == 0)
        return 0;
    ai_Unk5bfed0(param_1, param_2, param_3);
    /* ROUGH SKELETON: the rows of the page */
    return 0;
}
