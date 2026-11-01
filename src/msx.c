/* msx -- name leaked (the #line below); the start next to mov.c is plausible, not proven.
   unit of rea 8cef7d5b (src/game/msx.c); boundaries in decl/text.dcl */
#line 1 "D:\\develop\\units\\Msx\\msx.c"
/* msx.c -- D:\develop\units\Msx\msx.c (leaked assert path).
   Streamed music and ambient tracks: a worker thread updates the track fades every 50 ms, music
   tracks cross-fade (the next track is queued until the playing one has faded out), and a master
   fade scales the music driver volume. Extent in GildeGold.exe: 0x423770..0x424510.
   Functions are kept in their original order. Names are ours.
   @named_by leaked
   @boundary plausible */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <windows.h>
#include "msx.h"
#include "sfx.h"

extern volatile unsigned long d3_Time;      /* 0x6333cc, see d3.h */
/* d3_Time * 13, spelled so that the volatile is read three times */
#define MSX_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

extern LARGE_INTEGER msx_Last;

/* 0x423770 */
void msx_Thread(void)
{
    LARGE_INTEGER now;

    while (!msx_Quit) {
        if (!QueryPerformanceCounter(&now))
            msx_Exit();
        else if (now.QuadPart - msx_Last.QuadPart > msx_Freq.QuadPart / 20) {
            msx_Last = now;
            EnterCriticalSection(&msx_Lock);
            msx_Update();
            LeaveCriticalSection(&msx_Lock);
        }
        Sleep(23);
    }
    DeleteCriticalSection(&msx_Lock);
}
/* 0x423830, opens the music and ambient drivers, the timer and the mixer thread; 0 ok, -1 on failure
   (the music driver is closed again) */

int msx_Init(int a, int b, int c)
{
    DWORD id;

    memset(&msx_Dummy, 0, sizeof(msx_Dummy));
    InitializeCriticalSection(&msx_Lock);
    if (sfx_OpenDigOutput(&msx_Driver, a, b, c) == 0 && sfx_OpenDigOutput(&msx_AmbDriver, a, b, c) == 0) {
        if (!QueryPerformanceFrequency(&msx_Freq)) {
            sfx_CloseDigOutput(msx_Driver);
            msx_Driver = 0;
            return -1;
        }
        if (!CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)msx_Thread, NULL, 0, &id)) {
            sfx_CloseDigOutput(msx_Driver);
            msx_Driver = 0;
            return -1;
        }
        return 0;
    }
    return -1;
}
/* 0x4238e0, asks the mixer thread to stop, then closes the music and ambient drivers */

void msx_Exit(void)
{
    msx_Quit = 1;
    if (msx_Driver) {
        sfx_CloseAllStreams();
        sfx_CloseDigOutput(msx_Driver);
        msx_Driver = 0;
    }
    if (msx_AmbDriver) {
        sfx_CloseDigOutput(msx_AmbDriver);
        msx_AmbDriver = 0;
    }
}
/* 0x423930, sets the volume of track t; a playing track that is not fading takes it at once (its stream too) */

void msx_SetTrackVolume(MSXTRACK *t, int vol)
{
    if (msx_Driver && t) {
        EnterCriticalSection(&msx_Lock);
        t->volume = vol;
        if (t->fademode == 0 && t->playing) {
            t->curvol = vol;
            if (t->stream)
                sfx_SetStreamVolume(t->stream, vol);
        }
        LeaveCriticalSection(&msx_Lock);
    }
}
/* 0x4239a0, music master volume scaled by the current fade (clamped to 0..1); stores the raw volume */

void msx_SetVolume(int vol)
{
    if (msx_Driver) {
        if (msx_Fade < 0.0f)
            msx_Fade = 0.0f;
        else if (msx_Fade > 1.0f)
            msx_Fade = 1.0f;
        EnterCriticalSection(&msx_Lock);
        sfx_SetMasterVolume(msx_Driver, (int)(vol * msx_Fade));
        LeaveCriticalSection(&msx_Lock);
        msx_Volume = vol;
        sprintf(msx_VolMsg, "Fademodifier is %f, Mastervolume is %d", msx_Fade, vol);
    }
}
/* 0x423a50, ambient master volume */

void msx_SetAmbVolume(int vol)
{
    if (msx_AmbDriver) {
        EnterCriticalSection(&msx_Lock);
        sfx_SetMasterVolume(msx_AmbDriver, vol);
        msx_AmbVolume = vol;
        LeaveCriticalSection(&msx_Lock);
    }
}
/* 0x423a90 */

MSXTRACK *msx_Play(char *name, int once)
{
    MSXTRACK *t;

    if (!msx_Driver)
        return &msx_Dummy;
    EnterCriticalSection(&msx_Lock);
    t = msx_Find(name);
    LeaveCriticalSection(&msx_Lock);
    if (!t) {
        EnterCriticalSection(&msx_Lock);
        t = msx_Alloc();
        if (t) {
            memset(t, 0, sizeof(*t));
            strcpy(t->name, name);
        }
        LeaveCriticalSection(&msx_Lock);
    }
    t->bAmbient = 0;
    if (msx_Start(t, once) == -1)
        t = NULL;
    return t;
}
/* 0x423b30, opens and rewinds (or queues) the track's stream, looping unless once; -1 when the stream fails */

int msx_Start(MSXTRACK *t, int once)
{
    char path[256];
    MSXTRACK *q;

    if (msx_Driver && msx_AmbDriver) {
        EnterCriticalSection(&msx_Lock);
        if (msx_DelayPeak)
            sprintf(msx_Msg, "Reporting last delaypeak: %d", sfx_TimerHighestDelay());
        else
            msx_DelayPeak = sfx_TimerHighestDelay();
        if (!t->playing) {
            strcpy(path, msx_Path);
            strcat(path, t->name);
            if (t->stream == 0) {
                if (t->bAmbient ? sfx_OpenStream(msx_AmbDriver, path, &t->stream)
                               : sfx_OpenStream(msx_Driver, path, &t->stream) == -1) {
                    LeaveCriticalSection(&msx_Lock);
                    return -1;
                }
            }
            if (t->length == 0) {
                sfx_GetStreamLength(t->stream, &t->length);
                t->volume = 127;
            }
            if ((!t->bAmbient && t->length - t->pos < 10000) || t->pos == 0) {
                t->pos = 0;
                t->curvol = t->volume;
                t->fademode = 0;
                sfx_SetStreamVolume(t->stream, 0);
                sfx_StartStream(t->stream);
                sfx_PauseStream(t->stream, 1);
                sfx_SetStreamPos(t->stream, 0);
                sfx_SetStreamVolume(t->stream, t->volume);
                t->fadepos = 0;
            } else {
                sfx_PauseStream(t->stream, 1);
                sfx_SetStreamVolume(t->stream, 0);
                sfx_SetStreamPos(t->stream, t->pos);
                if (!t->bAmbient) {
                    t->curvol = 0;
                    t->fadevol = 0;
                    t->fademode = 1;
                    t->fadepos = t->pos;
                }
            }
            if (once)
                sfx_SetStreamLoopCount(t->stream, 0);
            else
                sfx_SetStreamLoopCount(t->stream, 1);
            q = msx_GetPlaying();
            if (!t->bAmbient && q) {
                sprintf(msx_Msg, "Queuing Track %s after Track %s !", t->name, q->name);
                q->next = t;
                t->queued = 1;
                msx_SetFade(q, 2);
            } else {
                sprintf(msx_Msg, "Starting Track %s !", t->name);
                t->queued = 0;
                sfx_PauseStream(t->stream, 0);
                t->playing = 1;
            }
        } else if (t->fademode == 2)
            msx_SetFade(t, 1);
        LeaveCriticalSection(&msx_Lock);
    }
    return 0;
}
/* 0x423df0, stops a playing track: a fade request starts a fade-out (fademode 2, the update thread
   closes the stream once it has faded), otherwise the stream is paused and closed at once. No return
   value; takes msx_Lock and clears the track's stream/playing */

void msx_Stop(MSXTRACK *t, int fade)
{
    if (msx_Driver) {
        EnterCriticalSection(&msx_Lock);
        if (t->playing) {
            if (fade)
                msx_SetFade(t, 2);
            else {
                sfx_PauseStream(t->stream, 1);
                sfx_CloseStream(t->stream);
                t->stream = 0;
                t->playing = 0;
            }
        }
        LeaveCriticalSection(&msx_Lock);
    }
}
/* 0x423e70, sets the per-track fade duration in ms (msx_FadeTime) used by the update thread's
   fade-in / fade-out volume ramp; no return value, writes one module variable */

void msx_SetFadeTime(int ms)
{
    msx_FadeTime = ms;
}
/* 0x423e80, body of the update thread (called with msx_Lock held about every 50 ms): for every playing
   track it follows the stream position, retires a finished stream and starts the queued track, ramps the
   fade-in / fade-out volume, then advances the master fade; no return value
   @deep B draft, 428/800 bytes: every code hunk is the loop IV base offset (ours &t->fadepos, original &t->volume, all field offsets +8), the same operations spelled differently. The attempts (582 operand-order variants, pointer/for/do-while loop forms, dead volume loads at 6 positions, volume read first in both fade blocks) all left the base unchanged; this is a VC6 base-selection choice, not a meaning error. Q6 added. */

void msx_Update(void)
{
    MSXTRACK *t;
    int i, dur;
    unsigned int elapsed;

    if (msx_Driver) {
        for (i = 0; i < MSX_TRACKS; i++) {
            t = &msx_Tracks[i];
            if (!t->playing)
                continue;
            if (t->stream == 0) {
                t->playing = 0;
                if (t->next) {
                    if (t->next->stream) {
                        sfx_PauseStream(t->next->stream, 0);
                        t->next->playing = 1;
                    }
                    t->next->queued = 0;
                    t->next = NULL;
                }
                continue;
            }
            sfx_GetStreamPos(t->stream, &t->pos);
            if (t->bAmbient)
                continue;
            if (sfx_StreamStatus(t->stream) != 4 && sfx_StreamStatus(t->stream) != 0x10) {
                t->playing = 0;
                t->fademode = 0;
                sfx_CloseStream(t->stream);
                t->stream = 0;
                if (t->next && t->next->stream) {
                    sfx_PauseStream(t->next->stream, 0);
                    t->next->playing = 1;
                    t->next->queued = 0;
                    t->next = NULL;
                }
            }
            if (t->fademode == 1) {
                if (t->pos < t->fadepos)
                    t->pos = t->fadepos;
                if (t->fadevol)
                    dur = t->fadevol * msx_FadeTime / t->volume;
                else
                    dur = msx_FadeTime;
                if (dur == 0 || dur <= t->pos - t->fadepos) {
                    t->fademode = 0;
                    t->curvol = t->volume;
                } else {
                    t->curvol = (t->pos - t->fadepos) * t->volume / dur;
                    if (abs(t->volume - t->curvol) < 2) {
                        t->fademode = 0;
                        t->curvol = t->volume;
                    }
                }
                sfx_SetStreamVolume(t->stream, t->curvol);
            } else if (t->fademode == 2) {
                if (t->pos < t->fadepos)
                    t->pos = t->fadepos;
                dur = t->fadevol * msx_FadeTime / t->volume;
                if (dur == 0 || dur <= t->pos - t->fadepos) {
                    t->fademode = 0;
                    t->curvol = 0;
                    sfx_SetStreamVolume(t->stream, 0);
                    sfx_CloseStream(t->stream);
                    t->stream = 0;
                    t->playing = 0;
                    if (t->next) {
                        sfx_PauseStream(t->next->stream, 0);
                        t->next->playing = 1;
                        t->next->queued = 0;
                        t->next = NULL;
                    }
                } else {
                    t->curvol = (t->fadepos - t->pos + dur) * t->volume / dur;
                    if (t->curvol > 2)
                        sfx_SetStreamVolume(t->stream, t->curvol);
                    else {
                        t->fademode = 0;
                        t->curvol = 0;
                        sfx_SetStreamVolume(t->stream, 0);
                        sfx_CloseStream(t->stream);
                        t->stream = 0;
                        t->playing = 0;
                        if (t->next) {
                            sfx_PauseStream(t->next->stream, 0);
                            t->next->playing = 1;
                            t->next->queued = 0;
                            t->next = NULL;
                        }
                    }
                }
            }
        }
        if (msx_FadeTo >= 0.0f) {
            elapsed = MSX_TIME;
            elapsed -= msx_FadeStart;
            if (elapsed < msx_FadeDuration)
                msx_Fade = (msx_FadeTo - msx_FadeFrom) / msx_FadeDuration * elapsed + msx_FadeFrom;
            else {
                msx_Fade = msx_FadeTo;
                msx_FadeTo = -1.0f;
            }
            msx_SetVolume(msx_Volume);
        }
    }
}
/* 0x4241a0, starts a master music fade to `vol` (0..1) over `ms` milliseconds; ms == 0 sets the
   factor at once. Does nothing without a driver or with vol outside 0..1; no return value, takes
   msx_Lock and leaves the fade to the update thread */

void msx_FadeMaster(float vol, int ms)
{
    if (msx_Driver && !(vol < 0.0f) && !(vol > 1.0f)) {
        EnterCriticalSection(&msx_Lock);
        if (ms == 0) {
            msx_Fade = vol;
            msx_SetVolume(msx_Volume);
            LeaveCriticalSection(&msx_Lock);
            return;
        }
        msx_FadeDuration = ms;
        msx_FadeTo = vol;
        msx_FadeFrom = msx_Fade;
        msx_FadeStart = MSX_TIME;
        LeaveCriticalSection(&msx_Lock);
    }
}
/* 0x424250, finds the track slot whose file name equals `name` (plain strcmp; callers hold msx_Lock);
   returns the slot or NULL */

MSXTRACK *msx_Find(char *name)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (strcmp(msx_Tracks[i].name, name) == 0)
            return &msx_Tracks[i];
    return NULL;
}
/* 0x4242c0, returns the first playing non-ambient (music) track, NULL when none; callers hold
   msx_Lock */

MSXTRACK *msx_GetPlaying(void)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].bAmbient && msx_Tracks[i].playing)
            return &msx_Tracks[i];
    return NULL;
}
/* 0x424300, picks a free track slot for a new track: first an idle one with no position yet, then any
   idle one; returns the slot or NULL when all MSX_TRACKS are busy. Callers hold msx_Lock */

MSXTRACK *msx_Alloc(void)
{
    int i;

    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].playing && !msx_Tracks[i].stream && !msx_Tracks[i].pos)
            return &msx_Tracks[i];
    for (i = 0; i < MSX_TRACKS; i++)
        if (!msx_Tracks[i].playing && !msx_Tracks[i].stream)
            return &msx_Tracks[i];
    return NULL;
}
/* 0x424370, starts a fade-in (mode 1) or fade-out (mode 2) on a playing track: snapshots the stream
   position (t->pos is read back from the stream) and the current volume as the fade start. No return
   value; no-op for a NULL or idle track or mode 0, callers hold msx_Lock */

void msx_SetFade(MSXTRACK *t, int mode)
{
    if (t && t->playing && mode) {
        sfx_GetStreamPos(t->stream, &t->pos);
        t->fadepos = t->pos;
        t->fadevol = t->curvol;
        t->fademode = mode;
    }
}
/* 0x4243d0, sets the directory prefix (msx_Path) that msx_Start puts before every track file name;
   unchecked strcpy into a 0x100-byte buffer, no return value */

void msx_SetPath(char *path)
{
    strcpy(msx_Path, path);
}
/* 0x4243f0, plays the named sound once on the ambient driver: reuses the slot of that name or takes a
   free one, closes a stream still open in it and starts the track at `vol`. Returns the track, NULL
   when no slot is free or the stream fails, &msx_Dummy when no ambient driver is open; takes msx_Lock */

MSXTRACK *msx_PlayAmbient(char *name, int vol)
{
    MSXTRACK *t;

    if (!msx_AmbDriver)
        return &msx_Dummy;
    EnterCriticalSection(&msx_Lock);
    t = msx_Find(name);
    if (!t) {
        t = msx_Alloc();
        if (!t)
            goto fail;
        memset(t, 0, sizeof(*t));
        strcpy(t->name, name);
    }
    t->bAmbient = 1;
    if (t->stream) {
        sfx_CloseStream(t->stream);
        t->stream = 0;
    }
    if (msx_Start(t, 1) == 0) {
        msx_SetTrackVolume(t, vol);
        LeaveCriticalSection(&msx_Lock);
        return t;
    }
fail:
    LeaveCriticalSection(&msx_Lock);
    return NULL;
}
/* 0x4244b0, stops an ambient track at once (no fade; msx_Stop with fade 0): asserts that the track is
   ambient, then pauses and closes its stream under msx_Lock. Does nothing without the ambient driver,
   no return value */

void msx_StopAmbient(MSXTRACK *hTrack)
{
    if (msx_AmbDriver) {
#line 741
        assert(hTrack->bAmbient);
        EnterCriticalSection(&msx_Lock);
        msx_Stop(hTrack, 0);
        LeaveCriticalSection(&msx_Lock);
    }
}

extern char msx_Path[0x100];
extern MSXTRACK msx_Dummy;
extern CRITICAL_SECTION msx_Lock;
float msx_Fade = 1.0f;  /* 0x5f74a8, master volume factor 0..1 */
float msx_FadeFrom = -1.0f;  /* 0x5f74ac */
float msx_FadeTo = -1.0f;  /* 0x5f74b0, < 0 = no master fade running */
unsigned int msx_FadeDuration = 5000;  /* 0x5f74b4 */
int msx_Volume = 127;  /* 0x5f74b8 */
int msx_AmbVolume = 127;  /* 0x5f74bc */
int msx_FadeTime = 10000;  /* 0x5f74c0, track fade time */
LARGE_INTEGER msx_Freq = {0};  /* 0x6323a8 */
char msx_Msg[0x100] = {0};  /* 0x6323b0, debug message (not printed) */
char msx_VolMsg[0x100] = {0};  /* 0x6324b0 */
unsigned int msx_FadeStart = 0;  /* 0x6325b0 */
LARGE_INTEGER msx_Last = {0};  /* 0x6325b4, counter at the last update */
int msx_DelayPeak = 0;  /* 0x6325bc */
int msx_Quit = 0;  /* 0x6325c0, ends the update thread */
int msx_Driver = 0;  /* 0x6325c4, music */
int msx_AmbDriver = 0;  /* 0x6325c8, ambient */
MSXTRACK msx_Tracks[MSX_TRACKS] = {0};  /* 0x6325d0 */
char msx_Path[0x100] = {0};  /* 0x14611a0 */
MSXTRACK msx_Dummy = {0};  /* 0x14612a0, returned when there is no driver */
CRITICAL_SECTION msx_Lock = {0};  /* 0x14613e0 */
