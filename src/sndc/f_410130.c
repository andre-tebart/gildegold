/* 0x410130, script command StopSample: stops the sample at once (no fade); always returns 0 */

int sndc_StopSample(struct SBSAMPLE **s)
{
    sb_Stop(*s, 0);
    return 0;
}
