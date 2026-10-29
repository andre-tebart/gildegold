/* 0x410000, script command PlaySampleHandle: starts the named sample of a bank, only while the running
   script is in no group; 0 when it is */

struct SBSAMPLE *sndc_PlaySampleHandle(struct SBBANK **bank, char **name)
{
    struct SBSAMPLE *s;

    if (evt_Cur->group != evt_NoGroup)
        return 0;
    s = sb_GetSample(*bank, *name);
    if (!s)
        return s;
    return (struct SBSAMPLE *)sb_Play(s, 0, 0);
}
