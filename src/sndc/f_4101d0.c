/* 0x4101d0, script command WaitForSample: suspends the script until the sample has stopped playing - the
   first call arms the script's wait function when the script is in mode 1, each resumed call while
   sb_IsPlaying stays true re-arms it */

void sndc_WaitForSample(struct SBSAMPLE **s)
{
    struct SBSAMPLE *sample = *s;

    if (evt_CurCall && evt_CurCall->fn == (int (*)())sndc_WaitForSample) {
        if (sb_IsPlaying(sample))
            evt_Cur->waitfn = sndc_WaitForSample;
    } else if (evt_Cur->mode == 1)
        evt_Cur->waitfn = sndc_WaitForSample;
}
