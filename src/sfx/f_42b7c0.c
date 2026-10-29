/* 0x42b7c0 */

int sfx_OpenDigOutput(SFXDRIVER **out, int channels, unsigned int bits, int rate)
{
    int i;
    int n, pref;

    *out = 0;
    if (sfx_Initialized) {
        i = sfx_FreeDriverSlot();
        if (i >= 0) {
            sfx_Drivers[i] = mem_Alloc(sizeof(SFXDRIVER), "sfx_OpenDigOutput");
            sfx_Drivers[i]->wFormatTag = 1;
            sfx_Drivers[i]->nChannels = channels;
            sfx_Drivers[i]->nSamplesPerSec = rate;
            sfx_Drivers[i]->nAvgBytesPerSec = (bits >> 3) * channels * rate;
            sfx_Drivers[i]->nBlockAlign = (bits >> 3) * channels;
            sfx_Drivers[i]->wBitsPerSample = bits;
            if (AIL_waveOutOpen(&sfx_Drivers[i]->drv, 0, 0, &sfx_Drivers[i]->wFormatTag)) {
                mem_Free(sfx_Drivers[i]);
                sfx_Drivers[i] = 0;
            } else {
                /* First driver and no sample count yet: take the MSS default (DIG_MIXER_CHANNELS, 1). The two
                   locals and the second use of n are what the bytes need: n stays a register candidate in the
                   test block and VC6 folds `/ n` (n == 1 here); exact original spelling unknown. */
                n = sfx_NumDrivers();
                if (n == 1 && !sfx_NumSamples) {
                    pref = AIL_get_preference(1);
                    sfx_SetNumSamples(pref / n);
                }
                sfx_Samples[i] = mem_Alloc(sfx_NumSamples * sizeof(HSAMPLE), "sfx_OpenDigOutput");
                sfx_Streams[i] = mem_Alloc(sfx_NumSamples * sizeof(HSTREAM), "sfx_OpenDigOutput");
                SFX_ZERO(sfx_Samples[i], sfx_NumSamples * sizeof(HSAMPLE));
                SFX_ZERO(sfx_Streams[i], sfx_NumSamples * sizeof(HSTREAM));
                sfx_Drivers[i]->max = sfx_NumSamples;
                sfx_Drivers[i]->used = 0;
                *out = sfx_Drivers[i];
                return 0;
            }
        }
    }
    return -1;
}
