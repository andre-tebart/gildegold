/* sfx.c -- original file name unknown (mem_Alloc tag "sfx_OpenDigOutput").
   Digital sound output on top of the Miles Sound System: up to 16 waveOut drivers, each with a table of
   sample and stream handles. Extent in GildeGold.exe (inferred): starts at 0x42b720 after the event script
   interpreter and ends with sfx_TimerHighestDelay at 0x42c3a0 (a tail call of the import
   AIL_get_timer_highest_delay, compiled in this unit). Functions are kept in their original order. Names are ours.
   unit of rea 8cef7d5b (src/game/sfx.c); boundaries in decl/text.dcl */
#include <windows.h>
#include "sfx.h"

/* 0x42b720 */
int sfx_Startup(void)
{
    int ret = 0;

    if (!sfx_Initialized) {
        MSS_auto_cleanup();
        AIL_startup();
        AIL_last_error();
        sfx_Initialized = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42b750 */

int sfx_Shutdown(void)
{
    int ret = 0;

    if (sfx_Initialized) {
        sfx_CloseAllStreams();
        sfx_ReleaseAllSamples();
        sfx_CloseAll();
        AIL_shutdown();
        sfx_Initialized = ret;
    } else
        ret = -1;
    return ret;
}
/* 0x42b780 */

int sfx_SetMasterVolume(SFXDRIVER *d, int vol)
{
    int ret = 0;

    if (sfx_FindDriver(d) >= 0 && vol >= 0 && vol <= 127) {
        AIL_set_digital_master_volume(d->drv, vol);
    } else
        ret = -1;
    return ret;
}
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
/* 0x42b970 */

int sfx_CloseDigOutput(SFXDRIVER *d)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            AIL_waveOutClose(d->drv);
            if (sfx_Samples[i]) {
                mem_Free(sfx_Samples[i]);
                sfx_Samples[i] = 0;
            }
            if (sfx_Streams[i]) {
                mem_Free(sfx_Streams[i]);
                sfx_Streams[i] = 0;
            }
            mem_Free(d);
            sfx_Drivers[i] = 0;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42b9f0 */

int sfx_CloseAll(void)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                if (sfx_CloseDigOutput(sfx_Drivers[i]) == -1)
                    ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42ba30 */

int sfx_Release(SFXDRIVER *d)
{
    int ret = 0;

    if (!sfx_Initialized || sfx_FindDriver(d) < 0 || !AIL_digital_handle_release(d->drv))
        ret = -1;
    return ret;
}
/* 0x42ba70 */

int sfx_ReleaseAll(void)
{
    int ret = 0;
    int i;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                if (sfx_Release(sfx_Drivers[i]))
                    ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bab0 */

void sfx_Reacquire(SFXDRIVER *d, void *hwnd, unsigned int msg)
{
    if (sfx_Initialized && msg >= WM_USER && sfx_FindDriver(d) >= 0)
        if (!AIL_digital_handle_reacquire(d->drv))
            PostMessageA(hwnd, msg, (WPARAM)d, 0);
}
/* 0x42bb00 */

void sfx_ReacquireAll(void *hwnd, unsigned int msg)
{
    int i;

    if (sfx_Initialized)
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                sfx_Reacquire(sfx_Drivers[i], hwnd, msg);
}
/* 0x42bb40 */

int sfx_SetNumSamples(int n)
{
    int ret = 0;

    if (sfx_Initialized && sfx_NumDrivers() <= 1) {
        sfx_NumSamples = n;
        AIL_set_preference(1, n);
    } else
        ret = -1;
    return ret;
}
/* 0x42bb80 */

int sfx_AllocSample(SFXDRIVER *d, HSAMPLE *out)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            k = sfx_FreeSampleSlot(d);
            if (k >= 0) {
                *out = AIL_allocate_sample_handle(d->drv);
                if (*out) {
                    d->used++;
                    sfx_Samples[i][k] = *out;
                } else
                    ret = -1;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bbf0 */

int sfx_SetSampleFile(HSAMPLE s, char *type, void *image, int size)
{
    int ret = 0;

    if (!sfx_Initialized || sfx_FindSample(s) < 0 || !AIL_set_named_sample_file(s, type, image, size, ret))
        ret = -1;
    return ret;
}
/* 0x42bc40 */

int sfx_ReleaseSample(HSAMPLE s)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_SampleDriver(s);
        if (i >= 0) {
            k = sfx_FindSample(s);
            if (k >= 0) {
                AIL_release_sample_handle(s);
                sfx_Samples[i][k] = 0;
                sfx_Drivers[i]->used--;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bca0 */

int sfx_ReleaseAllSamples(void)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                for (k = 0; k < sfx_Drivers[i]->max; k++)
                    if (sfx_Samples[i][k])
                        if (sfx_ReleaseSample(sfx_Samples[i][k]) == -1)
                            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bd00 */

int sfx_InitSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_init_sample(s);
    } else
        ret = -1;
    return ret;
}
/* 0x42bd30 */

int sfx_StartSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_start_sample(s);
    } else
        ret = -1;
    return ret;
}
/* 0x42bd60 */

int sfx_ResumeSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_resume_sample(s);
    } else
        ret = -1;
    return ret;
}
/* 0x42bd90 */

int sfx_EndSample(HSAMPLE s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_end_sample(s);
    } else
        ret = -1;
    return ret;
}
/* 0x42bdc0 */

int sfx_SetSampleVolume(HSAMPLE s, int vol)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_volume(s, vol);
    } else
        ret = -1;
    return ret;
}
/* 0x42be00 */

int sfx_SetSamplePan(HSAMPLE s, int pan)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_pan(s, pan);
    } else
        ret = -1;
    return ret;
}
/* 0x42be40 */

int sfx_SetSampleLoopCount(HSAMPLE s, int count)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindSample(s) >= 0) {
        AIL_set_sample_loop_count(s, count);
    } else
        ret = -1;
    return ret;
}
/* 0x42be80 */

int sfx_SampleStatus(HSAMPLE s)
{
    int ret = -1;

    if (sfx_Initialized && sfx_FindSample(s) >= 0)
        ret = AIL_sample_status(s);
    return ret;
}
/* 0x42beb0 */

int sfx_OpenStream(SFXDRIVER *d, char *file, HSTREAM *out)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_FindDriver(d);
        if (i >= 0) {
            k = sfx_FreeStreamSlot(d);
            if (k >= 0) {
                *out = AIL_open_stream(d->drv, file, ret);
                if (*out) {
                    sfx_Streams[i][k] = *out;
                    d->used++;
                } else {
                    AIL_last_error();
                    ret = -1;
                }
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bf20 */

int sfx_CloseStream(HSTREAM s)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        i = sfx_StreamDriver(s);
        if (i >= 0) {
            k = sfx_FindStream(s);
            if (k >= 0) {
                AIL_close_stream(s);
                sfx_Drivers[i]->used--;
                sfx_Streams[i][k] = 0;
            } else
                ret = -1;
        } else
            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bf80 */

int sfx_CloseAllStreams(void)
{
    int ret = 0;
    int i;
    int k;

    if (sfx_Initialized) {
        for (i = 0; i < SFX_DRIVERS; i++)
            if (sfx_Drivers[i])
                for (k = 0; k < sfx_Drivers[i]->max; k++)
                    if (sfx_Streams[i][k])
                        if (sfx_CloseStream(sfx_Streams[i][k]))
                            ret = -1;
    } else
        ret = -1;
    return ret;
}
/* 0x42bfe0 */

int sfx_StartStream(HSTREAM s)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_start_stream(s);
    } else
        ret = -1;
    return ret;
}
/* 0x42c010 */

int sfx_PauseStream(HSTREAM s, int pause)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_pause_stream(s, pause);
    } else
        ret = -1;
    return ret;
}
/* 0x42c050 */

int sfx_SetStreamVolume(HSTREAM s, int vol)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_volume(s, vol);
    } else
        ret = -1;
    return ret;
}
/* 0x42c090 */

int sfx_SetStreamLoopCount(HSTREAM s, int count)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_loop_count(s, count);
    } else
        ret = -1;
    return ret;
}
/* 0x42c0d0 */

int sfx_SetStreamPos(HSTREAM s, int ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_set_stream_ms_position(s, ms);
    } else
        ret = -1;
    return ret;
}
/* 0x42c110 */

int sfx_StreamStatus(HSTREAM s)
{
    if (sfx_Initialized && sfx_FindStream(s) >= 0)
        return AIL_stream_status(s);
    return -1;
}
/* 0x42c140 */

int sfx_GetStreamPos(HSTREAM s, int *ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_stream_ms_position(s, 0, ms);
    } else
        ret = -1;
    return ret;
}
/* 0x42c180 */

int sfx_GetStreamLength(HSTREAM s, int *ms)
{
    int ret = 0;

    if (sfx_Initialized && sfx_FindStream(s) >= 0) {
        AIL_stream_ms_position(s, ms, 0);
    } else
        ret = -1;
    return ret;
}
/* 0x42c1c0 */

int sfx_FreeDriverSlot(void)
{
    int i;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (!sfx_Drivers[i])
            return i;
    return -1;
}
/* 0x42c1e0 */

int sfx_FindDriver(SFXDRIVER *d)
{
    int i;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i] == d)
            return i;
    return -1;
}
/* 0x42c200 */

int sfx_FreeSampleSlot(SFXDRIVER *d)
{
    int i;
    int k;

    i = sfx_FindDriver(d);
    if (i >= 0 && d->used < d->max)
        for (k = 0; k < d->max; k++)
            if (!sfx_Samples[i][k])
                return k;
    return -1;
}
/* 0x42c240 */

int sfx_FreeStreamSlot(SFXDRIVER *d)
{
    int i;
    int k;

    i = sfx_FindDriver(d);
    if (i >= 0 && d->used < d->max)
        for (k = 0; k < d->max; k++)
            if (!sfx_Streams[i][k])
                return k;
    return -1;
}
/* 0x42c280 */

int sfx_FindSample(HSAMPLE s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Samples[i][k] == s)
                    return k;
    return -1;
}
/* 0x42c2c0 */

int sfx_FindStream(HSTREAM s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Streams[i][k] == s)
                    return k;
    return -1;
}
/* 0x42c300 */

int sfx_SampleDriver(HSAMPLE s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Samples[i][k] == s)
                    return i;
    return -1;
}
/* 0x42c340 */

int sfx_StreamDriver(HSTREAM s)
{
    int i;
    int k;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            for (k = 0; k < sfx_Drivers[i]->max; k++)
                if (sfx_Streams[i][k] == s)
                    return i;
    return -1;
}
/* 0x42c380 */

int sfx_NumDrivers(void)
{
    int i;
    int n = 0;

    for (i = 0; i < SFX_DRIVERS; i++)
        if (sfx_Drivers[i])
            n++;
    return n;
}
/* 0x42c3a0, the sound timer's highest delay (AIL_get_timer_highest_delay) for msx: a tail call through the
   import slot. It is sfx's own code (its nop fill before it is the compiler's alignment inside one object, not
   LINK's int3 padding between objects), so it is no import thunk */

int sfx_TimerHighestDelay(void)
{
    return AIL_get_timer_highest_delay();
}

SFXDRIVER * sfx_Drivers[SFX_DRIVERS] = {0};  /* 0x6332f0 */
HSAMPLE * sfx_Samples[SFX_DRIVERS] = {0};  /* 0x633330 */
HSTREAM * sfx_Streams[SFX_DRIVERS] = {0};  /* 0x633370 */
int sfx_Initialized = 0;  /* 0x6333b0 */
int sfx_NumSamples = 0;  /* 0x6333b4, samples per driver (DIG_MIXER_CHANNELS) */
