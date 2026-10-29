/* 0x53d5d0, frees the records, then asks 0x52a040 three times */

int he_Unk53d5d0(void)
{
    he_Unk53d590();
    if (sv_ReturnZero() != 0)
        return 1;
    if (sv_ReturnZero() != 0)
        return 1;
    return sv_ReturnZero() != 0;
}
