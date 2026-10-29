/* 0x48feb0, a random character of the building (kind 0x14, not busy with a grave action) */

AIPX *ai_Unk48feb0(AIBLD *b)
{
    unsigned short list[8];
    int n, i;

    n = 0;
    for (i = 0; i < 768; i++) {
        if (n >= 8)
            break;
        if (AIC[i].id != 0xffff && AIC[i].kind == 0x14 && AIC[i].bld == b && AIC[i].unk008 > 0 &&
            AIC[i].unk180 != -0x78 && AIC[i].unk180 != -0x70 && AIC[i].unk180 != -0x77 &&
            AIC[i].unk180 != -0x76)
            list[n++] = i;
    }
    if (n < 2)
        return 0;
    return &AIC[list[util_Random(n)]];
}
