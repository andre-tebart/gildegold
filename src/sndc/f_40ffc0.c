/* 0x40ffc0 */
struct SBBANK *sndc_LoadSampleBank(char **name)
{
    return sb_LoadSampleBank(*name, 1);
}
