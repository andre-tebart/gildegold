/* 0x40ffe0, unloads a sample bank (always returns 0) */

int sndc_KillSampleBank(struct SBBANK **bank)
{
    sb_UnloadSampleBank(*bank);
    return 0;
}
