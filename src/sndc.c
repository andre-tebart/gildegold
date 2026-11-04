/* sndc.c -- original file name unknown: sound script commands (see include/sndc.h). Starts at
   0x40ffc0 and ends with the registration 0x410220. Names are ours.
   unit of rea 8cef7d5b (src/game/sndc.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "sndc.h"
#include "d3_snd.h"
#include "f3init.h"
#include "f3snd.h"
#include "sblib.h"

/* script constants registered by sndc_Register (0x5f3108..0x5f3124) */
int snd_W2 = -1, snd_W1 = -2, snd_M3 = -3, snd_M2 = -4, snd_M1 = -5, snd_HS = -6, snd_NONE = -7, snd_VAR = -8;  /* 0x5f3108, 0x5f310c, 0x5f3110, 0x5f3114, 0x5f3118, 0x5f311c, 0x5f3120, 0x5f3124 */

/* 0x40ffc0, loads the sample bank named by *name with its data preloaded (sb_LoadSampleBank's preload 1);
   returns the bank, NULL when the file cannot be read. */
struct SBBANK *sndc_LoadSampleBank(char **name)
{
    return sb_LoadSampleBank(*name, 1);
}
/* 0x40ffe0, unloads a sample bank (always returns 0) */

int sndc_KillSampleBank(struct SBBANK **bank)
{
    sb_UnloadSampleBank(*bank);
    return 0;
}
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
/* 0x410040, script command PlaySample: sets the loop count of the named sample of the default sample bank
   and plays it, only while the running script is in no group; returns the sample, 0 when it was not started
   (the lookup is repeated in the original's null path) */

struct SBSAMPLE *sndc_PlaySample(char **name, int *loops)
{
    struct SBSAMPLE *s;

    if (evt_Cur->group != evt_NoGroup)
        return 0;
    s = sb_GetSample(0, *name);
    if (s) {
        sb_SetLoops(s, *loops);
        sb_Play(s, 0, 0);
    } else
        sb_GetSample(0, *name);
    return s;
}
/* 0x4100b0, script command PlaySample3D: plays the named sample of the default bank at the 3D object
   (volume 60, the given range) when the loop count is 1, else non-positional with that loop count; only
   while the running script is in no group; returns the sample or 0 */

struct SBSAMPLE *sndc_PlaySample3D(struct D3OBJECT **obj, int *range, char **name, int *loops)
{
    struct SBSAMPLE *s;

    if (evt_Cur->group != evt_NoGroup)
        return 0;
    s = sb_GetSample(0, *name);
    if (s) {
        if (*loops == 1)
            d3snd_PlayAt(s, *obj, 60, (float)*range);
        else {
            sb_SetLoops(s, *loops);
            sb_Play(s, 0, 0);
        }
    }
    return s;
}
/* 0x410130, script command StopSample: stops the sample at once (no fade); always returns 0 */

int sndc_StopSample(struct SBSAMPLE **s)
{
    sb_Stop(*s, 0);
    return 0;
}
/* 0x410150, script command GetSampleBankHandle: returns the loaded sample bank of that name, 0 if none */

struct SBBANK *sndc_GetSampleBankHandle(char **name)
{
    return sb_FindBank(*name);
}
/* 0x410160, script command GetSampleHandle: returns the named sample of the bank, 0 if the bank has no such
   sample */

struct SBSAMPLE *sndc_GetSampleHandle(struct SBBANK **bank, char **name)
{
    return sb_GetSample(*bank, *name);
}
/* 0x410180, script command SpeechQueued: queues the spoken text (speaker id, bank, text, variation, delay)
   with the voice queue, only while the running script is in no group; returns the voice sample or 0 */

int sndc_SpeechQueued(int *id, int *bank, char **name, int *var, int *delay)
{
    if (evt_Cur->group != evt_NoGroup)
        return 0;
    return snd_QueueVoice(*id, *bank, *name, *var, *delay);
}
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
/* 0x410220, registers the sound script commands (LoadSampleBank ... WaitForSample) and the SND_* script
   globals with the event system (evt_ImportCommand / evt_AddGlobal) */

void sndc_Register(void)
{
    evt_ImportCommand("LoadSampleBank", (int (*)())sndc_LoadSampleBank, 1, 1, 6);
    evt_ImportCommand("KillSampleBank", (int (*)())sndc_KillSampleBank, 1, 1, 1);
    evt_ImportCommand("PlaySampleHandle", (int (*)())sndc_PlaySampleHandle, 1, 2, 1, 6);
    evt_ImportCommand("PlaySample", (int (*)())sndc_PlaySample, 1, 2, 6, 1);
    evt_ImportCommand("PlaySample3D", (int (*)())sndc_PlaySample3D, 1, 4, 1, 1, 6, 1);
    evt_ImportCommand("StopSample", (int (*)())sndc_StopSample, 1, 1, 1);
    evt_ImportCommand("GetSampleBankHandle", (int (*)())sndc_GetSampleBankHandle, 1, 1, 6);
    evt_ImportCommand("GetSampleHandle", (int (*)())sndc_GetSampleHandle, 1, 2, 1, 6);
    evt_ImportCommand("SpeechQueued", (int (*)())sndc_SpeechQueued, 1, 5, 1, 1, 6, 1, 1);
    evt_ImportCommand("WaitForSample", (int (*)())sndc_WaitForSample, 5, 1, 1);
    evt_AddGlobal("SND_W2", &snd_W2, 1);
    evt_AddGlobal("SND_W1", &snd_W1, 1);
    evt_AddGlobal("SND_M1", &snd_M1, 1);
    evt_AddGlobal("SND_M2", &snd_M2, 1);
    evt_AddGlobal("SND_M3", &snd_M3, 1);
    evt_AddGlobal("SND_HS", &snd_HS, 1);
    evt_AddGlobal("SND_NONE", &snd_NONE, 1);
    evt_AddGlobal("SND_VAR", &snd_VAR, 1);
}
