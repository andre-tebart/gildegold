/* 0x5ba1c0, a cleanup gated by 0x62c418 (208 B, ROUGH SKELETON): only when 0x62c418 == -1; the body is a
   comment */

int ai_Unk5ba1c0(void)
{
    if (U_I((unsigned)&full_data_62c418) != -1)
        return 0;
    /* ROUGH SKELETON: the cleanup path */
    return 0;
}
