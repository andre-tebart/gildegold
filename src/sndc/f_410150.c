/* 0x410150, script command GetSampleBankHandle: returns the loaded sample bank of that name, 0 if none */

struct SBBANK *sndc_GetSampleBankHandle(char **name)
{
    return sb_FindBank(*name);
}
