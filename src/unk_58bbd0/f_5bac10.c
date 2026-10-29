/* 0x5bac10, a window gated by 0x62c418 == -1 (512 B, ROUGH SKELETON): the 0x1000 path after the check; the body
   is a comment */

int ai_Unk5bac10(void)
{
    if (U_I((unsigned)&full_data_62c418) != -1)
        return 0;
    /* ROUGH SKELETON: the 0x1000 allocation path */
    return 0;
}
