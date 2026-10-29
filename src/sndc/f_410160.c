/* 0x410160, script command GetSampleHandle: returns the named sample of the bank, 0 if the bank has no such
   sample */

struct SBSAMPLE *sndc_GetSampleHandle(struct SBBANK **bank, char **name)
{
    return sb_GetSample(*bank, *name);
}
