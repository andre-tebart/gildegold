/* f3snd.c -- tag "f3snd: snd_AddToQueue" (mem_Alloc, 0x4c0230); the sound queue and ambient sounds: the playing list snd_Queue, the gusts ("Boehe_Leicht_s", "Boehe_Mittel_s",
   "Boehe_Stark_s", .data 0x602b10 right after rt.c's "rt_load"). Start: the first function after rt.c's
   save / load; no module tag. Names are ours.
   unit of rea 8cef7d5b (src/game/f3snd.c); boundaries in decl/text.dcl */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <d3d.h>
#include "d3.h"
#include "f3snd.h"

/* d3_Time * 13, spelled so that the volatile is read three times */
#define F3SND_TIME ((d3_Time << 3) + (d3_Time << 2) + d3_Time)

/* The engine's inline-asm fill (as D3FL_FILL in d3_fl.h): size and value through temps (declared in
   this order), the destination named directly in the asm. */
#define F3SND_CAT(a, b) a##b
#define F3SND_LABEL(n) F3SND_CAT(f3snd_fill_tail, n)
#define F3SND_FILL(dvar, val, size) {                               \
    int _fn = (size); int _fv = (val);                              \
    __asm mov edi, dvar                                             \
    __asm mov eax, _fv                                              \
    __asm mov ebx, _fn                                              \
    __asm mov ah, al                                                \
    __asm shl eax, 16                                               \
    __asm mov ecx, eax                                              \
    __asm shr ecx, 16                                               \
    __asm add eax, ecx                                              \
    __asm lea ecx, [edi+edi*2]                                      \
    __asm and ecx, 3                                                \
    __asm sub ebx, ecx                                              \
    __asm jle F3SND_LABEL(__LINE__)                                 \
    __asm rep stosb                                                 \
    __asm mov ecx, ebx                                              \
    __asm and ebx, 3                                                \
    __asm shr ecx, 2                                                \
    __asm rep stosd                                                 \
    __asm F3SND_LABEL(__LINE__): add ecx, ebx                       \
    __asm rep stosb                                                 \
}
/* stdlib min / max (the body ai.h defines too) */
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#include "data_views.h"
#include "ahm.h"
#include "ai.h"
#include "anm.h"
#include "d3_snd.h"
#include "d3_tools.h"
#include "f3gm.h"
#include "f3init.h"
#include "mem.h"
#include "msx.h"
#include "sblib.h"
#include "sv.h"
#include "tr.h"
#include "d3_pose.h"
#include "ch.h"
#include "txt.h"                      /* txt_Language */
#define util_RandomU ((unsigned short (*)(int))util_Random)  /* 0x42ec00 (tr.c: int util_Random(unsigned short)): @original this unit's callers read the 16-bit result (byte-matched ch_RandomNear 0x405160, ai_CalcMeisterBank 0x4836a0, cm_FightLoop 0x4d88c0 move it as a word / movzx) */
#define sb_PlaySampleI ((struct SBSAMPLE *(*)(int, char *, int, int, int, int, int))sb_PlaySample)  /* 0x426fa0 (sblib.c: SBBANK *bank): this unit passes the bank as an int */
#define sb_SetVolumeI ((void (*)(int, int))sb_SetVolume)  /* 0x427330 (sblib.c: SBSAMPLE *): @original f3snd.c passes the int handle view of the sample */
#define sb_IsVariationI ((int (*)(int, char *))sb_IsVariation)  /* 0x426f60 (sblib.c: SBBANK *): this unit's int bank view */
#define sb_GetVariationI ((int (*)(int, char *, int))sb_GetVariation)  /* 0x426190 (sblib.c: SBSAMPLE *(SBBANK *, char *, int)): this unit's int bank / handle view */
#define sb_SetRestartC ((void (*)(char *, int))sb_SetRestart)  /* 0x426170 (sblib.c: SBSAMPLE *): this unit's char * view of the sample */
#define d3snd_DetachI ((void (*)(int))d3snd_Detach)  /* 0x4177b0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_StopI ((void (*)(int, int))d3snd_Stop)  /* 0x417db0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_SetHoldI ((void (*)(int, int))d3snd_SetHold)  /* 0x417dd0 (d3_snd.c: D3SOUND *): this unit's int handle */
#define d3snd_AttachI ((int (*)(int, char *, int, float))d3snd_Attach)  /* 0x417710 (d3_snd.c: D3SOUND *(SNDHANDLE *, D3OBJECT *, int, float)): this unit's int handle / char * object view */
#define d3snd_SetVolumeI ((void (*)(int, int))d3snd_SetVolume)  /* 0x417e00 (d3_snd.c: D3SOUND *): this unit keeps the slot as an int handle */
#define ai_MarketBuildingV (*(G16BLDVIEW **)&ai_MarketBuilding)  /* 0x6f1bc8, ai_MarketBuilding (ai.c: AIBLD *) as this unit's G16BLDVIEW (its +0x4c / object fields) */
#define ai_FallbackBuildingB (*(unsigned char **)&ai_FallbackBuilding)  /* 0x6f230c, ai_FallbackBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_PlayerBuildingB (*(unsigned char **)&ai_PlayerBuilding)  /* 0x6f2308, ai_PlayerBuilding (sv.c: char *) read bytewise, as this unit does */
#define ai_CharsC (*(char (*)[768 * 0x21c])&ai_Chars)  /* 0x770b80, ai_Chars (ahm.c: char[768][0x21c]) read flat, as this unit does */
/* 0x4c01b0, stops the queue: restores the music after a fanfare (fade back to 1.0), frees every queued sound */
void snd_ClearQueue(void)
{
    G16SND *n;

    if (snd_FanfareFade != 0) {
        snd_FanfareFade = 0;
        snd_FadeMusic(1.0f, 2000);
    }
    if (snd_Queue != 0) {
        do {
            n = snd_Queue->next;
            sb_SetRestartC(snd_Queue->smp, 0);
            mem_Free(snd_Queue);
            snd_Queue = n;
        } while (n != 0);
    }
}
/* 0x4c0210, the tail of the queue (the sound queued last), 0 when the queue is empty */

G16SND *snd_QueueTail(void)
{
    G16SND *p;

    if (snd_Queue != 0) {
        p = snd_Queue;
        while (p->next != 0)
            p = p->next;
        return p;
    }
    return 0;
}
/* 0x4c0230, appends smp to the queue, played delay after the previous one */

int snd_AddToQueue(char *smp, unsigned int delay)
{
    G16SND *s;
    G16SND *l;

    if (smp != 0) {
        s = mem_Alloc(0x10, "f3snd: snd_AddToQueue");
        if (s != 0) {
            F3SND_FILL(s, 0, 0x10);
            sb_SetRestartC(smp, 1);
            s->smp = smp;
            s->delay = delay;
            l = snd_QueueTail();
            if (l != 0)
                l->next = s;
            else
                snd_Queue = s;
            return 1;
        }
    }
    return 0;
}
/* 0x4c02e0, the queue step: starts the first sound once its delay is over (a "Fanfare" fades the ambience out),
   drops it when it has ended ("MausclickLinks" after a fanfare fades the ambience back in)
   @deep B draft, 75 code hunks, no meaning error: read the whole body against the original 0x4c02e0..0x4c0462. Every statement, constant, callee and branch corresponds (paused gate, delay < t - snd_QueueTime with t = F3SND_TIME's three volatile reads now equal, the Fanfare fade-out, sb_Play, playing=1/snd_QueueTime=t, sb_IsPlaying, the free and the MausclickLinks fade-back). What is left is scheduling and register allocation: ours loads delay before computing t - snd_QueueTime, hoists mov esi,1 above the sample/entry test, re-reads snd_Queue into another register after sb_Play, and jumps straight to the epilogue where the original merges through a second test ecx,ecx. Same operations spelled/ordered differently; readable (names, the G16SMP/G16SMPENTRY union view). */

void snd_UpdateQueue(void)
{
    G16SND *n;
    unsigned int t;

    if (snd_QueuePaused != 0)
        return;
    t = F3SND_TIME;
    if (snd_Queue == 0)
        return;
    if (snd_Queue->playing == 0) {
        if (snd_Queue->delay < t - snd_QueueTime) {
            if (snd_Queue->sample != 0 && snd_Queue->sample->info != 0) {
                if (strstr(snd_Queue->sample->info->name, "Fanfare") && snd_FanfareFade == 0) {
                    snd_FadeMusic(0.0f, 2000);
                    snd_FanfareFade = 1;
                }
                sb_Play(snd_Queue->smp, 0, 0);
            }
            snd_Queue->playing = 1;
            snd_QueueTime = t;
        }
    } else if (!sb_IsPlaying(snd_Queue->smp)) {
        n = snd_Queue->next;
        sb_SetRestartC(snd_Queue->smp, 0);
        mem_Free(snd_Queue);
        snd_Queue = n;
        snd_QueueTime = t;
        if (n != 0 && n->sample != 0 && n->sample->info != 0 && snd_FanfareFade != 0
            && strstr(n->sample->info->name, "MausclickLinks")) {
            n = snd_Queue->next;
            sb_SetRestartC(snd_Queue->smp, 0);
            mem_Free(snd_Queue);
            snd_Queue = n;
            snd_FadeMusic(1.0f, 2000);
            snd_FanfareFade = 0;
        }
    }
}

/* stops ambient sound s (a macro: the original repeats this block for each one) */
#define SND_STOP(s) if ((s) != 0) { sb_Stop((s), 0); (s) = 0; }

/* plays ambient sound s (name n, volume v; a, b the last two arguments) or sets its volume */
#define SND_SET(s, n, v, a, b) if ((s) == 0) (s) = sb_PlaySampleI(snd_Bank, (n), 0, 0x3f, (v), (a), (b)); \
    else sb_SetVolumeI((s), (v))

/* the volume of a rain loop: x scaled to 0..88.9 (0.7 of the full 127) by the divisor d; the float cast keeps
   the two factors apart, as the original multiplies by 88.9 and by 1 / d separately (a reasoned guess at the
   spelling: without it VC6 folds both constants into one) */
#define SND_RAINVOL(x, d) ((int)((float)((x) * 88.9f) / (d)))

/* 0x4c0470, the rain sounds by the rain level *d3t_Rain (near / far loops louder the lower / higher the camera
   is above the ground, drops "Tropfen_s"), then now and then a gust ("Boehe_*", chance and interval by season from
   the table 0x602ba8); everything stops without a scene, with sound off or when the season changed @deep Read all of it against the original: the 499 / 0x95 branches keep a float fraction t = n - base (then t = 500 / 350 - t, was 499 - k / 350 - k) and depth = 2600 - f, the 0x95 drops start at depth's full volume, volumes multiply by 88.9 and 1/d separately (SND_RAINVOL), the gust compares written as the original's (>= interval, w > 0x28a); 748 = 748 instructions. Left: f and t swap slots ([esp+0x10]/[esp+0x14]) and the four near loops: the original loads the handle into edi/esi before the volume's ftol and keeps it for sb_SetVolume (mov edi, [snd_RainHeavyNear] ... cmp edi, ebx), ours reloads it into eax after (mov esi, eax; mov eax, [X]; cmp eax, ebx), 1 byte each (2538 vs 2542). */
void snd_UpdateRain(void)
{
    float f;
    float t;
    float depth;                        /* 2600 - the camera height above the ground */
    int n;
    int w;
    int v;
    int k;

    if (d3_Terrain == 0 || snd_AmbientOn == 0 || snd_Season != gm_ClockSeason(&ai_Time)) {
        snd_Season = gm_ClockSeason(&ai_Time);
        SND_STOP(snd_RainHeavyFar);
        SND_STOP(snd_RainHeavyNear);
        SND_STOP(snd_RainMediumFar);
        SND_STOP(snd_RainMediumNear);
        SND_STOP(snd_RainLightFar);
        SND_STOP(snd_RainLightNear);
        SND_STOP(snd_RainDrops);
        return;
    }
    w = 0;
    f = d3_Camera->pos.y - d3t_GetMeanHeight(&d3_Camera->pos);
    memcpy(snd_TimeCopy, &ai_Time, 14);
    if (d3t_Rain == 0 || (n = *d3t_Rain) == 0) {
        SND_STOP(snd_RainHeavyFar);
        SND_STOP(snd_RainHeavyNear);
        SND_STOP(snd_RainMediumFar);
        SND_STOP(snd_RainMediumNear);
        SND_STOP(snd_RainLightFar);
        SND_STOP(snd_RainLightNear);
        SND_STOP(snd_RainDrops);
    } else {
        w = n * 8;
        if (n >= 999) {
            /* heavy only */
            v = SND_RAINVOL(2600.0f - f, 2600.0f);
            SND_SET(snd_RainHeavyNear, "StarkNah_l_01", v, 0, 0);
            SND_SET(snd_RainHeavyFar, "StarkFern_l_01", SND_RAINVOL(f, 2600.0f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 1000);
            else
                sb_SetVolumeI(snd_RainDrops, v);
            SND_STOP(snd_RainMediumNear);
            SND_STOP(snd_RainLightNear);
            SND_STOP(snd_RainMediumFar);
            SND_STOP(snd_RainLightFar);
        } else if (n >= 499) {
            /* heavy fading in over medium: t of 500 */
            t = (float)(n - 499);
            depth = 2600.0f - f;
            v = SND_RAINVOL(depth * t, 1300000.0f);
            SND_SET(snd_RainHeavyNear, "StarkNah_l_01", v, 0, 0);
            SND_SET(snd_RainHeavyFar, "StarkFern_l_01", SND_RAINVOL(t * f, 1300000.0f), 0, 0);
            t = 500.0f - t;
            SND_SET(snd_RainMediumNear, "MittelNah_l_01", SND_RAINVOL(depth * t, 1300000.0f), 0, 0);
            SND_SET(snd_RainMediumFar, "MittelFern_l_01", SND_RAINVOL(t * f, 1300000.0f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 0xa28);
            else {
                ((G16SMP *)snd_RainDrops)->delay = 2000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainLightFar);
            SND_STOP(snd_RainLightNear);
        } else if (n >= 0x95) {
            /* medium fading in over light: t of 350 */
            t = (float)(n - 0x95);
            depth = 2600.0f - f;
            v = SND_RAINVOL(depth * t, 910000.0f);
            SND_SET(snd_RainMediumNear, "MittelNah_l_01", v, 0, 0);
            SND_SET(snd_RainMediumFar, "MittelFern_l_01", SND_RAINVOL(t * f, 910000.0f), 0, 0);
            t = 350.0f - t;
            SND_SET(snd_RainLightNear, "LeichtNah_l_01", SND_RAINVOL(depth * t, 910000.0f), 0, 0);
            SND_SET(snd_RainLightFar, "LeichtFern_l_01", SND_RAINVOL(t * f, 910000.0f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 1, 0x3f, SND_RAINVOL(depth, 2600.0f), 0, 3000);
            else {
                ((G16SMP *)snd_RainDrops)->delay = 3000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainHeavyFar);
            SND_STOP(snd_RainHeavyNear);
        } else {
            /* light only: t of 149 */
            t = (float)n;
            v = SND_RAINVOL((2600.0f - f) * t, 387400.0f);
            SND_SET(snd_RainLightNear, "LeichtNah_l_01", v, 0, 0);
            SND_SET(snd_RainLightFar, "LeichtFern_l_01", SND_RAINVOL(t * f, 387400.0f), 0, 0);
            if (snd_RainDrops == 0)
                snd_RainDrops = sb_PlaySampleI(snd_Bank, "Tropfen_s", 0, 0x3f, v, 0, 4000);
            else {
                ((G16SMP *)snd_RainDrops)->delay = 4000;
                sb_SetVolumeI(snd_RainDrops, v);
            }
            SND_STOP(snd_RainHeavyFar);
            SND_STOP(snd_RainHeavyNear);
            SND_STOP(snd_RainMediumFar);
            SND_STOP(snd_RainMediumNear);
        }
    }
    if (d3_Terrain != 0 && snd_AmbientOn != 0
        && (unsigned int)(w + (F3SND_TIME - snd_LastGust)) >= snd_Gusts[snd_Season].interval) {
        snd_LastGust = F3SND_TIME;
        if (rand() * 3.0518509e-05f + snd_Gusts[snd_Season].chance > 1.0f) {
            k = 0;
            if (w > 0x28a)
                k = rand() % 3;
            else if (w > 0xfa)
                k = rand() % 2;
            sb_PlaySampleI(0, snd_GustNames[k], 1, 0x3f, (int)(snd_Gusts[snd_Season].vol * 127.0f), 0, 0);
        }
    }
}

/* stops locust sound s with log text t */
#define SND_STOPLOG(s, t) if ((s) != 0) { sprintf(ai_Unk6f0960, (t), (s)); sb_Stop((s), 0); \
    *(int *)((s) + 0x18) = 0; (s) = 0; }

/* 0x4c0e60, the animal ambience by season (ai_Unk602bd8[season], records birds "Voegel_s", crickets "Grillen_l"
   (hpLocusts / hpLocustsPerm, with a debug log in ai_Unk6f0960), chickens "Gackern_s", horses "Wiehern_s" /
   "Schnauben_s", dogs "Bellt_s"), quieter with the camera height and silent in rain; the bird song of 0x6f0b80 by
   the distance to the player's house; then the atmosphere mp3 of the building the player is in ("Athmos\..")
   @deep B draft, 337 code hunks, two meaning errors fixed: (1) the atmosphere track volume is not ch_Count*100 - the original computes n = ch_Count(d3_CurrentScene, 0) * 9 for the Witshaus (0x16) and * 6 for Handwerk (0x12/0x14/8/0x15/0xe) and Schreibstube (7), caps it with the float 64.0f min (0x5eb808) and keeps it on the FPU stack to the ftol at the play / set-volume call; our source recomputed ch_Count*100.0f at each call; (2) the two d3_Time * 0xd sites are F3SND_TIME (three volatile reads). Read against the original 0x4c0e60..0x4c18a0: the season change, h = (2000 - camera.y - meanHeight) * 0.0005f, the five animal blocks (birds / crickets+hpLocusts / dogs / horses / chickens), the bird song by house distance and the atmosphere branches all correspond in statements, constants and calls. What is left is scheduling / register allocation (337 hunks): stack slots differ ([esp+0x8] vs [esp+0xc]), the min comes out as fcom + jnp where the original has fld 64.0f / fcomp st(1) / jp (the arguments spelled the other way, likely max(64.0f, n)), and the whole body's webs differ. Readability debt: Q1, Q2 (ai_Unk602bd8 the animal table, ai_Unk6f0960 log text, snd_AmbientTrack bird song, ai_Unk6f0b94/98 hpLocusts*, ai_Unk6f0b9c season, ai_Unk6f0ba0 atmosphere track, ai_MarketBuilding player house, ai_PlayerBuilding/2310 building/room - several also declared in cm.h), Q4 9 raw accesses incl. the SND_STOPLOG macro. */
void snd_UpdateAnimals(void)
{
    unsigned int t;
    int n;
    float h;
    float vol;
    float v[4];
    char *name;
    int k;
    char c;
    G16AMB *a;

    if (d3_Terrain == 0 || snd_AmbientOn == 0) {
        t = F3SND_TIME;
        if (ai_Unk6f0b9c != gm_ClockSeason(&ai_Time))
            ai_Unk6f0b9c = gm_ClockSeason(&ai_Time);
        SND_STOPLOG(ai_Unk6f0b94, "hpLocustsPerm stopped, reason: !(boden && snd_AmbientActive) ! Handlevalue is %x");
        SND_STOPLOG(ai_Unk6f0b98, "hpLocusts stopped, reason: !(boden && snd_AmbientActive) ! Handlevalue is %x");
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        a[0].last = t;
        a[1].last = t;
        a[4].last = t;
        a[3].last = t;
        a[2].last = t;
    } else {
        n = 0;
        t = F3SND_TIME;
        if (ai_Unk6f0b9c != gm_ClockSeason(&ai_Time)) {
            ai_Unk6f0b9c = gm_ClockSeason(&ai_Time);
            if (ai_Unk6f0b98 != 0) {
                sb_Stop(ai_Unk6f0b98, 0);
                *(int *)(ai_Unk6f0b98 + 0x18) = 0;
                ai_Unk6f0b98 = 0;
            }
            if (ai_Unk6f0b94 != 0) {
                sb_Stop(ai_Unk6f0b94, 0);
                *(int *)(ai_Unk6f0b94 + 0x18) = 0;
                ai_Unk6f0b94 = 0;
            }
        }
        h = (2000.0f - d3_Camera->pos.y - d3t_GetMeanHeight(&d3_Camera->pos)) * 0.0005f;
        if (d3t_Rain != 0)
            n = *d3t_Rain;
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[0].interval) < t - a[0].last && n < 0x15e) {
            a[0].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][0].chance > 1.0f)
                sb_PlaySampleI(0, "Voegel_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][0].count + 1, 0x3f,
                    (int)(ai_Unk602bd8[ai_Unk6f0b9c][0].vol * 127.0f), 0, 0);
            if (rand() % 10 > 6)
                sb_PlaySampleI(0, "Voegel_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][0].count + 1, 0x3f,
                    (int)(ai_Unk602bd8[ai_Unk6f0b9c][0].vol * 127.0f), 0, 0);
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[1].interval) < t - a[1].last && n < 100) {
            a[1].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][1].chance > 1.0f) {
                if (ai_Unk6f0b98 == 0)
                    k = 0;
                else
                    k = *(int *)(ai_Unk6f0b98 + 0xc);
                if (ai_Unk6f0b98 == 0 || k <= 2) {
                    sprintf(ai_Unk6f0960, "About to trigger hpLocusts ! Handlevalue before triggering is %x, loopCount is %d.", ai_Unk6f0b98, k);
                    ai_Unk6f0b98 = sb_PlaySampleI(0, "Grillen_l", rand() % ai_Unk602bd8[ai_Unk6f0b9c][1].count + 2,
                        0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f), 0, 0);
                    sprintf(ai_Unk6f0960, "hpLocusts triggered ! Handlevalue is %x", ai_Unk6f0b98);
                }
            }
            if (ai_Unk6f0b9c == 1) {
                if (ai_Unk6f0b94 == 0) {
                    ai_Unk6f0b94 = sb_PlaySampleI(0, "Grillen_l", 0, 0x3f, (int)(0.7f * h * 127.0f) >> 1, 0, 0);
                    sprintf(ai_Unk6f0960, "hpLocustsPerm triggered ! Handlevalue is %x", ai_Unk6f0b94);
                } else {
                    sb_SetVolumeI(ai_Unk6f0b94, (int)(0.7f * h * 127.0f) >> 1);
                    sprintf(ai_Unk6f0960, "hpLocustsPerm volume adjusted ! Handlevalue is %x", ai_Unk6f0b94);
                }
            } else if (ai_Unk6f0b94 != 0) {
                sprintf(ai_Unk6f0960, "hpLocustsPerm stopped, reason: s_season != GM_SUMMER ! Handlevalue is %x", ai_Unk6f0b94);
                sb_Stop(ai_Unk6f0b94, 0);
                ai_Unk6f0b94 = 0;
                *(int *)(ai_Unk6f0b94 + 0x18) = 0;
            }
        } else if (n < 0x65 && ai_Unk6f0b9c != 3) {
            sprintf(ai_Unk6f0960, "Adjusting Volume for hpLocusts (%x) and hpLocustsPerm (%x) !", ai_Unk6f0b98, ai_Unk6f0b94);
            if (ai_Unk6f0b94 != 0)
                sb_SetVolumeI(ai_Unk6f0b94, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f) >> 1);
            if (ai_Unk6f0b98 != 0)
                sb_SetVolumeI(ai_Unk6f0b98, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][1].vol * 127.0f) >> 1);
        } else {
            SND_STOPLOG(ai_Unk6f0b94, "hpLocustsPerm stopped, reason: numDropsModifier > 100 || s_season == GM_WINTER ! Handlevalue is %x");
            SND_STOPLOG(ai_Unk6f0b98, "hpLocusts stopped, reason: numDropsModifier > 100 || s_season == GM_WINTER ! Handlevalue is %x");
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[4].interval) < t - a[4].last) {
            a[4].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][0].chance > 1.0f)
                sb_PlaySampleI(0, "Bellt_s", 1, 0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][4].vol * 127.0f), 0, 0);
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[3].interval) < t - a[3].last) {
            a[3].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][3].chance > 1.0f) {
                name = "Wiehern_s";
                if (rand() % 10 > 4)
                    name = "Schnauben_s";
                sb_PlaySampleI(0, name, 1, 0x3f, (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][3].vol * 127.0f), 0, 0);
            }
        }
        a = ai_Unk602bd8[ai_Unk6f0b9c];
        if ((unsigned int)(rand() % 2000 + a[2].interval) < t - a[2].last) {
            a[2].last = t;
            if (rand() * 3.0518509e-05f + ai_Unk602bd8[ai_Unk6f0b9c][2].chance > 1.0f)
                sb_PlaySampleI(0, "Gackern_s", rand() % ai_Unk602bd8[ai_Unk6f0b9c][2].count + 1, 0x3f,
                    (int)(h * ai_Unk602bd8[ai_Unk6f0b9c][2].vol * 127.0f), 0, 0);
        }
        if (snd_AmbientTrack != 0 && ai_MarketBuildingV != 0) {
            d3_PointToWorld(ai_MarketBuildingV->object, ai_MarketBuildingV->object + 0x4c, v);
            k = ch_CountAt(0, 0, v, 900.0f) * 0x14;
            if (k > 0x7e)
                k = 0x7f;
            d3snd_SetVolumeI(snd_AmbientTrack, k);
        }
    }
    if (ai_PlayerBuildingB != 0 && snd_AmbientOn != 0) {
        c = ai_BTypes[*ai_PlayerBuildingB * 0x24d];
        if (c == 0x16 && sv_PanelShown != 0 && *sv_PanelShown == 0x120) {
            n = ch_Count(d3_CurrentScene, 0) * 9;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 != 0) {
                msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
                return;
            }
            ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Witshaus.mp3", 5);
            return;
        }
        if (c == 0x12 || c == 0x14 || c == 8 || c == 0x15 || c == 0xe) {
            n = ch_Count(d3_CurrentScene, 0) * 6;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 == 0) {
                if (*(int *)(ai_PlayerBuildingB + 0x95) == -1)
                    return;
                ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Handwerk.mp3", (int)vol);
                return;
            }
            if (*(int *)(ai_PlayerBuildingB + 0x95) == -1) {
                msx_StopAmbient(ai_Unk6f0ba0);
                ai_Unk6f0ba0 = 0;
                return;
            }
            msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
            return;
        }
        if (c == 7 && sv_PanelShown != 0 && *sv_PanelShown == 0xf7) {
            n = ch_Count(d3_CurrentScene, 0) * 6;
            vol = min(n, 64.0f);
            if (ai_Unk6f0ba0 == 0) {
                ai_Unk6f0ba0 = msx_PlayAmbient("Athmos\\Athmo_Schreibstube.mp3", (int)vol);
                return;
            }
            msx_SetTrackVolume(ai_Unk6f0ba0, (int)vol);
            return;
        }
    }
    if (ai_Unk6f0ba0 == 0)
        return;
    msx_StopAmbient(ai_Unk6f0ba0);
    ai_Unk6f0ba0 = 0;
}
/* 0x4c18a0, the music when the player enters a building: the current track loses 30 (20 without a building of
   its kinds); the building's kind gets its track (resumed, or a random one of the matching snd_BuildingTracks
   entry: kind, room (0: any), season (-1: any), up to six names) and gains 30 (20), started with msx_Play @deep Read all of it against the original: the two location-track tails are separate (Resuming / Starting, with the hyphenated strings 'location-track'), every access indexes snd_LocationTracks[i] (no pointer), 0x2694 test inverted as the original's je, the snd_BuildingTracks search is a for loop with goto found. 257 vs 259 instructions. Left: the search loop: the original walks a second pointer (ecx) and copies it to e (mov ecx, edi / mov edi, ecx; cmp dword ptr [edi], ebp on the first pass), ours coalesces both into edi and reuses the table[0].kind load (cmp eax, ebp) - tried a separate loop pointer (e = p, coalesced the same) and e = p with p = e + 1 (worse, 72 ins); plus edx/ebx for the room / season temps and eax/ecx for the gm_BuildQuery kind (4 bytes, 804 vs 808). */

void snd_EnterBuilding(void)
{
    unsigned char *b;
    unsigned int kind;
    int season;
    int i, k, j;
    G16TRACKS *e;

    b = ai_PlayerBuildingB;
    if (b == 0 && (b = ai_FallbackBuildingB) == 0)
        return;
    k = 0;
    kind = (unsigned char)ai_BTypes[*b * 0x24d];
    season = gm_ClockSeason(&ai_Time);
    i = snd_FindActiveTrack();
    if (i >= 0) {
        snd_LocationTracks[i].active = 0;
        if (snd_LocationTracks[i].kinds[0] != 0x2694) {
            for (; snd_LocationTracks[i].kinds[k] != 0; k++) {
                if (gm_BuildQuery(2, 4, ai_PlayerId, 5, snd_LocationTracks[i].kinds[k]) != 0) {
                    snd_LocationTracks[i].prio -= 0x1e;
                    break;
                }
            }
            if (snd_LocationTracks[i].kinds[k] == 0)
                snd_LocationTracks[i].prio -= 0x14;
        } else
            snd_LocationTracks[i].prio -= 0x1e;
    }
    i = snd_FindTrackKind(kind);
    if (i < 0)
        return;
    if (strlen(snd_LocationTracks[i].track) != 0) {
        snd_LocationTracks[i].active = 1;
        for (k = 0; snd_LocationTracks[i].kinds[k] != 0; k++) {
            if (gm_BuildQuery(2, 4, ai_PlayerId, 5, snd_LocationTracks[i].kinds[k]) != 0) {
                snd_LocationTracks[i].prio += 0x1e;
                break;
            }
        }
        if (snd_LocationTracks[i].kinds[k] == 0)
            snd_LocationTracks[i].prio += 0x14;
        snd_TrackHandles.current = msx_Play(snd_LocationTracks[i].track, 0);
        snd_TrackHandles.copy = snd_TrackHandles.current;
        sprintf(snd_DebugText, "Resuming location-track %s", snd_LocationTracks[i].track);
        return;
    }
    for (e = snd_BuildingTracks; e->kind != 0; e++) {
        if (e->kind == kind && (e->room == 0 || e->room == (unsigned int)*sv_PanelShown)
            && (season == e->season || e->season == -1))
            goto found;
    }
    return;
found:
    for (j = 0; j < 6 && e->names[j][0] != 0; j++)
        ;
    j = util_RandomU(j);
    if (strlen(e->names[j]) == 0)
        return;
    strcpy(snd_LocationTracks[i].track, e->names[j]);
    snd_LocationTracks[i].active = 1;
    for (k = 0; snd_LocationTracks[i].kinds[k] != 0; k++) {
        if (gm_BuildQuery(2, 4, ai_PlayerId, 5, snd_LocationTracks[i].kinds[k]) != 0) {
            snd_LocationTracks[i].prio += 0x1e;
            break;
        }
    }
    if (snd_LocationTracks[i].kinds[k] == 0)
        snd_LocationTracks[i].prio += 0x14;
    snd_TrackHandles.current = msx_Play(snd_LocationTracks[i].track, 0);
    snd_TrackHandles.copy = snd_TrackHandles.current;
    sprintf(snd_DebugText, "Starting location-track %s", snd_LocationTracks[i].track);
}
/* 0x4c1bd0, the music when the player leaves a building: the outdoor track (kind 0x2694), resumed or a new random
   one by season, not the one played last (snd_LastTrack) */

void snd_PlayOutdoorTrack(void)
{
    int season;
    int i;
    int r;
    char *t;

    season = gm_ClockSeason(&ai_Time);
    i = snd_FindActiveTrack();
    if (i >= 0) {
        snd_LocationTracks[i].prio -= 0x1e;
        snd_LocationTracks[i].active = 0;
    }
    i = snd_FindTrackKind(0x2694);
    if (i < 0)
        return;
    t = snd_LocationTracks[i].track;
    if (strlen(t) != 0) {
        sprintf(snd_TrackText, "Resuming Outdoor-Track %s !", t);
        snd_LocationTracks[i].prio += 0x1e;
        snd_LocationTracks[i].active = 1;
        snd_TrackHandles.copy = msx_Play(t, 0);
        snd_TrackHandles.current = snd_TrackHandles.copy;
        return;
    }
    do {
        r = util_RandomU(0x75) % 4;
        switch (season) {
        case 0:
            if (util_RandomU(0xb) < 4)
                strcpy(t, "CD2\\KraeuterUndPhiolen.mp3");
            else if (r == 0)
                strcpy(t, "cd1\\Burgfraeulein.mp3");
            else if (r == 1)
                strcpy(t, "cd1\\ImFruehling.mp3");
            else
                strcpy(t, "CD3\\Crescendo.mp3");
            sprintf(snd_TrackText, "Outdoor-Track for GM_SPRING selected: %s.", t);
            break;
        case 1:
            if (util_RandomU(0xb) < 3)
                strcpy(t, "CD3\\Heldenglanz.mp3");
            else if (r == 0)
                strcpy(t, "cd1\\Burgfraeulein.mp3");
            else if (r == 1)
                strcpy(t, "cd1\\ZurSommerzeit.mp3");
            else
                strcpy(t, "CD3\\MitFlinkemStich.mp3");
            sprintf(snd_TrackText, "Outdoor-Track for GM_SUMMER selected: %s.", t);
            break;
        case 2:
            if (util_RandomU(0xb) < 3)
                strcpy(t, "CD3\\DerStolzenTraum.mp3");
            else if (r == 0)
                strcpy(t, "cd1\\AufDenStrassen.mp3");
            else if (r == 1)
                strcpy(t, "cd1\\ImHerbst.mp3");
            else
                strcpy(t, "CD3\\BurgUndFeste.mp3");
            sprintf(snd_TrackText, "Outdoor-Track for GM_AUTUMN selected: %s.", t);
            break;
        case 3:
            if (util_RandomU(0xb) < 4)
                strcpy(t, "CD3\\SchmerzlichErinnerung.mp3");
            else if (r == 0)
                strcpy(t, "cd1\\AufDenStrassen.mp3");
            else if (r == 1)
                strcpy(t, "cd1\\InkalterNovembernacht.mp3");
            else
                strcpy(t, "cd1\\ImmerKalt.mp3");
            sprintf(snd_TrackText, "Outdoor-Track for GM_WINTER selected: %s.", t);
            break;
        default:
            goto skip;
        }
skip:
        if (strlen(snd_LastTrack) == 0)
            break;
    } while (strcmp(snd_LastTrack, t) == 0);
    snd_LocationTracks[i].prio += 0x1e;
    snd_LastTrack[0] = 0;
    snd_LocationTracks[i].active = 1;
    snd_TrackHandles.copy = msx_Play(t, 0);
    snd_TrackHandles.current = snd_TrackHandles.copy;
}
/* 0x4c1f90, the location track playing, -1 if none */

int snd_FindActiveTrack(void)
{
    int i;
    G16LOC *l;

    i = 0;
    l = snd_LocationTracks;
    do {
        if (l->active != 0)
            return i;
        i++;
    } while ((l++)->kinds[0] != 0);
    return -1;
}
/* 0x4c1fc0, the location track of building kind kind, -1 if none @deep B draft, 24 code-hunk instructions, no meaning error: read the original 0x4c1fc0..0x4c1fff against the source - the table walk (kinds[], kind compare, i++), the end test (l->kinds[0]) and the -1 return all correspond; only register allocation / scheduling differs. Ours keeps l->kinds[0] in esi from the loop entry (0x4c1fce) and tests esi after advancing edi; the original reloads kinds[0] into ecx (mov ecx,[esi] at 0x4c1fe6), advances both esi (l) and edi (a pointer it increments but never reads) and tests ecx. */

int snd_FindTrackKind(int kind)
{
    int i;
    int *k;
    G16LOC *l;

    i = 0;
    l = snd_LocationTracks;
    do {
        for (k = l->kinds; *k != 0; k++) {
            if (*k == kind)
                return i;
        }
        i++;
    } while ((l++)->kinds[0] != 0);
    return -1;
}
/* 0x4c2000, the music step: a new season stops the outdoor track; a finished track starts a pause (its name kept
   in snd_LastTrack); after the pause, or when the player moved to another building / room (and the current track
   loses a random draw against its priority), the track of the place starts (0x4c18a0 / 0x4c1bd0)
   @deep B draft, 279 code hunks / 18 regs, no meaning error left: read the whole original 0x4c2000..0x4c23ef against the source. Fixed here: t = F3SND_TIME (the original reads the volatile d3_Time three times; the source had d3_Time * 0xd, one read), the pause length (int)((1.0f - snd_MusicVolume) * 240000.0f) (0x5eb780=1.0f, 0x5edc0c=240000.0f, call 0x5de518 = __ftol) instead of util_RandomF()*60000.0f, and the five debug texts to the exe's strings (seasonchange, reached its end!, pauseLength is, propability). Remaining: register allocation and basic-block layout only - ours keeps the same instruction multiset up to register choice but hoists loads (e.g. the t computation uses ebx where the original uses edi), inlines the cb4==0 early return where the original jumps to an out-of-line copy, and inverts the final branch polarity; the time/branch/loop/table walks, every constant and callee correspond. Readability debt: Q1 snd_UpdateMusic, Q2 ai_GameState, snd_MusicVolume, ai_Unk6f0cb4, ai_Unk6f0ca8, ai_Unk6f0cac, ai_Unk6f0cb8, ai_Unk6f0cb0, ai_Unk60b0d8, snd_SpecialTrack, ai_Unk6f0860, ai_PlayerBuilding/230c/2310, Q4 the current-track bytes [0x104]/[0x108]/[0x10c]. */

void snd_UpdateMusic(void)
{
    unsigned int t;
    int i;
    int room;
    unsigned char *b;
    unsigned char *b2;
    char *trk;
    short prio;
    unsigned int kind;
    int k;

    t = F3SND_TIME;
    if (ai_GameState == 0 || !(snd_MusicVolume > 0.0f))
        return;
    i = gm_ClockSeason(&ai_Time);
    if (ai_Unk6f0cb4 != i) {
        if (ai_Unk6f0cb4 == 0) {
            ai_Unk6f0cb4 = gm_ClockSeason(&ai_Time);
            return;
        }
        i = snd_FindTrackKind(0x2694);
        ai_Unk6f0cb4 = gm_ClockSeason(&ai_Time);
        if (i < 0)
            return;
        trk = snd_TrackHandles.current;
        if (snd_LocationTracks[i].active != 0) {
            sprintf(ai_Unk6f0860, "Outdoortrack %s stopped because of seasonchange !", snd_LocationTracks[i].track);
            snd_LocationTracks[i].prio -= 0x1e;
            snd_LocationTracks[i].active = 0;
            trk = 0;
            if (snd_TrackHandles.current != 0) {
                msx_Stop(snd_TrackHandles.current, 1);
                strcpy(snd_LastTrack, snd_TrackHandles.current);
                ai_Unk60b0d8 = 1;
                snd_TrackHandles.current = 0;
                trk = 0;
            }
        }
        snd_LocationTracks[i].track[0] = 0;
        if (trk != 0)
            sprintf(ai_Unk6f0860, "Track still running after seasonchange is %s", trk);
        return;
    }
    room = (int)sv_PanelShown;
    b2 = ai_FallbackBuildingB;
    b = ai_PlayerBuildingB;
    trk = snd_TrackHandles.current;
    if (snd_TrackHandles.current == 0) {
        if (ai_Unk60b0d8 != 0 && ai_Unk6f0cb0 + ai_Unk60b0d8 >= t)
            return;
        ai_Unk60b0d8 = 0;
        ai_Unk6f0ca8 = ai_PlayerBuildingB;
        if (ai_PlayerBuildingB == 0)
            ai_Unk6f0ca8 = ai_FallbackBuildingB;
    } else {
        if (snd_TrackHandles.current[0x104] == 0 && snd_TrackHandles.current[0x105] == 0) {
            i = snd_FindActiveTrack();
            if (i >= 0) {
                snd_LocationTracks[i].active = 0;
                if (*(int *)(trk + 0x108) - *(int *)(trk + 0x10c) < 20000)
                    snd_LocationTracks[i].track[0] = 0;
                sprintf(ai_Unk6f0860, "track %s has reached its end!", snd_LocationTracks[i].track);
                trk = snd_TrackHandles.current;
            }
            strcpy(snd_LastTrack, trk);
            snd_TrackHandles.current = 0;
            ai_Unk60b0d8 = t;
            ai_Unk6f0cb0 = (int)((1.0f - snd_MusicVolume) * 240000.0f);
            sprintf(ai_Unk6f0860, "Pause starting at %d, pauseLength is %d ", t, ai_Unk6f0cb0);
            return;
        }
        if (!(t - ai_Unk6f0cb8 > 40000 && snd_TrackHandles.current != snd_SpecialTrack
              && (ai_PlayerBuildingB != 0 && ai_Unk6f0ca8 != ai_PlayerBuildingB || ai_FallbackBuildingB != 0 && ai_Unk6f0ca8 != ai_FallbackBuildingB
                  || ai_PlayerBuildingB == 0 && ai_FallbackBuildingB == 0 && ai_Unk6f0ca8 != 0))
            && !(ai_Unk6f0cac != room && ai_Unk6f0ca8 != 0 && ai_BTypes[*ai_Unk6f0ca8 * 0x24d] == 2))
            return;
        i = snd_FindActiveTrack();
        ai_Unk6f0ca8 = b;
        if (b == 0)
            ai_Unk6f0ca8 = b2;
        ai_Unk6f0cac = room;
        prio = snd_LocationTracks[i].prio;
        if (prio <= util_RandomU(0x80)) {
            ai_Unk6f0cb8 = t - 15000;
            return;
        }
        if (ai_Unk6f0ca8 == 0)
            kind = 0x2694;
        else
            kind = (unsigned char)ai_BTypes[*ai_Unk6f0ca8 * 0x24d];
        for (k = 0; snd_LocationTracks[i].kinds[k] != 0 && snd_LocationTracks[i].kinds[k] != kind; k++)
            ;
        if (snd_LocationTracks[i].kinds[k] != 0)
            return;
        sprintf(ai_Unk6f0860, "Random decision: interrupt running track %s, propability was %d.", snd_LocationTracks[i].track, prio);
    }
    ai_Unk6f0cb8 = t;
    if (ai_Unk6f0ca8 == 0) {
        snd_PlayOutdoorTrack();
        return;
    }
    snd_EnterBuilding();
}
/* 0x4c23f0, starts the special track of an event (fight, cutscene, ...): the first call saves the ambient flag
   and turns the ambient sounds off, then plays name (with name 0 only the music gets quieter, 0.4 over 2000 ms);
   snd_EndSpecialTrack ends it */

void snd_PlaySpecialTrack(char *name)
{
    unsigned int on;

    if (snd_AmbientSaved == 0) {
        on = snd_AmbientOn;
        snd_AmbientOn = 0;
        snd_AmbientSaved = 1;
        snd_AmbientOnSaved = on;
    }
    if (snd_MusicEnabled == 0 || snd_MusicVolume == 0.0f)
        return;
    if (name != 0) {
        if (snd_TrackHandles.current != 0)
            msx_Stop(snd_TrackHandles.current, 1);
        snd_SpecialTrack = msx_Play(name, 0);
        snd_TrackHandles.current = snd_SpecialTrack;
        return;
    }
    snd_FadeMusic(0.4f, 2000);
}
/* 0x4c2480, ends the special track (snd_PlaySpecialTrack): the ambient sounds back on, and the music back to
   full volume (unless the special track still plays) */

void snd_EndSpecialTrack(void)
{
    snd_AmbientSaved = 0;
    snd_AmbientOn = (unsigned char)snd_AmbientOnSaved;
    if (snd_MusicEnabled != 0 && snd_TrackHandles.current != snd_SpecialTrack)
        snd_FadeMusic(1.0f, 3000);
}
/* 0x4c24c0, fades the music to v in ms */

void snd_FadeMusic(float v, int ms)
{
    if (snd_TrackHandles.current != 0)
        msx_FadeMaster(v, ms);
}
/* 0x4c24e0, the voice variant of a speaker (voice number, woman): -2 / -1 for women (voice & 1), -5..-3 for
   men (voice % 3) - the negative ids 0x4c2510 turns into "_W1" / "_W2" / "_M1".."_M3" */

int snd_VoiceVariant(unsigned int voice, int woman)
{
    if (woman == 1)
        return (voice & 1) - 2;
    return voice % 3 - 5;
}
/* 0x4c2510, the voice sample text of speaker id (a character: "_M%i" / "_W%i" by sex, -1.. -6: "_W2", "_W1", "_M1",
   "_M2", "_M3", "_HS"; -7: text itself, -8: none) in bank, variation var if >= 0 ("%02i" appended)
   @deep B draft, 268 code hunks / 2 slots. Read the whole original 0x4c2510..0x4c275d against the source: no missing statement, constant, callee or condition - the id < 0x300 table sex/voice read, the six strcpy suffixes, id -7 sprintf(buf, text), -8 the empty first byte, both %s%s + sb_IsVariation (0x426f60), the inlined strcat, the second %s%s, sb_GetVariation (0x426190) and the two not-found messages, sb_GetSample (0x426030) all correspond; our call multiset is the same. Left is codegen: our frame is 0x12c vs the original's 0x128 (a 4-byte unused local slot shifts every local), the character table is read through the G16CHAR view at 0x770b84 (sex +5, voice +0) as lea chain + shl eax,2 + [eax+disp] where the original reads the record base 0x770b80 (sex +9, voice +4) with the folded [eax*4+disp], and the rest is register allocation / basic-block layout (the early return inlined and moved after the body). Lead: the real AICHAR view at 0x770b80 (ai.h) for the table access, and the extra 4-byte slot. Readability: Q1 done (snd_FindVoiceSample); Q2 ch_EmptyName, ai_CharInts, unk00. */

int snd_FindVoiceSample(int id, int bank, char *text, int var)
{
    char sfx[0x14];
    char num[0x10];
    char buf[0x104];
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    if (id == -7)
        sprintf(buf, text);
    else {
        if (id < 0x300 && id >= 0) {
            if (((G16CHAR *)ai_CharInts)[id].sex == 1)
                sprintf(sfx, "_W%i", (((G16CHAR *)ai_CharInts)[id].unk00 & 1) + 1);
            else if (((G16CHAR *)ai_CharInts)[id].sex == 0)
                sprintf(sfx, "_M%i", ((G16CHAR *)ai_CharInts)[id].unk00 % 3 + 1);
        } else if (id == -2)
            strcpy(sfx, "_W1");
        else if (id == -1)
            strcpy(sfx, "_W2");
        else if (id == -5)
            strcpy(sfx, "_M1");
        else if (id == -4)
            strcpy(sfx, "_M2");
        else if (id == -3)
            strcpy(sfx, "_M3");
        else if (id == -6)
            strcpy(sfx, "_HS");
        else if (id == -8)
            sfx[0] = ch_EmptyName[0];
        sprintf(buf, "%s%s", text, sfx);
        s = sb_IsVariationI(bank, buf);
        if (var >= 0 && s == 0) {
            sprintf(num, "%02i", var);
            strcat(sfx, num);
        }
        sprintf(buf, "%s%s", text, sfx);
        if (s != 0 && var >= 0) {
            s = sb_GetVariationI(bank, buf, var);
            if (s != 0)
                return s;
            sprintf(buf, "Voicesample not found in variation:\n%s %i", text, var);
            return 0;
        }
    }
    s = sb_GetSample(bank, buf);
    if (s == 0)
        sprintf(buf, "Voicesample not found:\n%s", text);
    return s;
}
/* 0x4c2760, plays the voice sample of speaker id in bank now (snd_FindVoiceSample) at the voice volume,
   unless a queued sound is playing; returns the sample handle or 0 */

int snd_PlayVoice(int id, int bank, char *text, int var)
{
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    if (snd_Queue != 0 && snd_Queue->playing != 0)
        return 0;
    s = snd_FindVoiceSample(id, bank, text, var);
    if (s != 0) {
        sb_Play((char *)s, 0, 0);
        sb_SetVolumeI(s, (int)(snd_VoiceVolume * 127.0f));
    }
    return s;
}
/* 0x4c27d0, queues a voice sample (snd_FindVoiceSample) with delay */

int snd_QueueVoice(int id, int bank, char *text, int var, unsigned int delay)
{
    int s;

    if (snd_SfxEnabled == 0)
        return 0;
    s = snd_FindVoiceSample(id, bank, text, var);
    if (s != 0) {
        sb_SetVolumeI(s, (int)(snd_VoiceVolume * 127.0f));
        snd_AddToQueue((char *)s, delay);
    }
    return s;
}
/* 0x4c2830, loads the voice sample bank name: "sprache_<language>\<name>" ("german"), or the plain
   "sprache\<name>" when that fails; returns the bank handle or 0 */

int snd_LoadVoiceBank(char *name, int a)
{
    char buf[0x104];
    int b;

    sprintf(buf, "sprache_%s\\%s", txt_Language, name);
    b = sb_LoadSampleBank(buf, a);
    if (b == 0) {
        sprintf(buf, "sprache\\%s", name);
        b = sb_LoadSampleBank(buf, a);
    }
    return b;
}
/* 0x4c28a0, loads the voice banks for building b: the workers' orders / click comments by kind (thieves 4,
   robbers 0x10, mercenaries 0x13, addon kinds 0x1d / 0x20 / 0x1f / 0x1e, else craftsmen) and the greeting of
   its kind */

void snd_LoadBuildingVoices(unsigned char *b)
{
    char greet[0x100];
    char order[0x100];
    char click[0x100];
    char *t;

    greet[0] = ch_EmptyName[0];
    memset(greet + 1, 0, sizeof(greet) - 1);
    t = ai_BTypes + *b * 0x24d;
    if (*t == 4) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_DIEBE.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_DIEBE.sbf");
    } else if (*t == 0x10) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_RAEUBER.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_RAEUBER.sbf");
    } else if (*t == 0x13) {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_SOELDNER.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_SOELDNER.sbf");
    } else if (*t == 0x1d) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_GEHILFEN.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_GEHILFEN.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_FRIEDHOF.sbf");
    } else if (*t == 0x20) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_SCHNEIDER.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_SCHNEIDER.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_SCHNEIDER.sbf");
    } else if (*t == 0x1f) {
        strcpy(order, "ADDON\\ARBEITERKOMMENTARE_BEFEHL_ZIGEUNER.sbf");
        strcpy(click, "ADDON\\ARBEITERKOMMENTARE_KLICK_ZIGEUNER.sbf");
        strcpy(greet, "ADDON\\BEGRUESSUNG_ZIGEUNER.sbf");
    } else if (*t == 0x1e) {
        click[0] = 0;
        strcpy(order, "ADDON\\ARBEITER_KOMMENTARE_BEFEHL_ALCHIMIST.sbf");
        greet[0] = 0;
    } else {
        strcpy(order, "ARBEITER_KOMMENTARE_BEFEHL_HANDWERK.sbf");
        strcpy(click, "ARBEITER_KOMMENTARE_KLICK_HANDWERK.sbf");
        if (*t == 7)
            strcpy(greet, "BEGRUESSUNG_KIRCHE.sbf");
        else if (*t == 5)
            strcpy(greet, "BEGRUESSUNG_GELDLEIHE.sbf");
        else if (*t == 8 || *t == 0xe)
            strcpy(greet, "BEGRUESSUNG_KRAEUTERLADEN_PARFUEMERIE.sbf");
        else if (*t == 9)
            strcpy(greet, "BEGRUESSUNG_LAGERHAUS.sbf");
        else if (*t == 0x15 || *t == 0x14 || *t == 0x12)
            strcpy(greet, "BEGRUESSUNG_SCHMIEDE_STEIMETZ_TISCHLER.sbf");
        else if (*t == 0xb || *t == 0xc || *t == 0xd)
            strcpy(greet, "BEGRUESSUNG_WALDSTUECK_STEINBRUCH_MINE.sbf");
        else if (*t == 0x16)
            strcpy(greet, "BEGRUESSUNG_WIRTSHAUS.sbf");
        else if (*t == 0x1e)
            strcpy(greet, "ADDON\\BEGRUESSUNG_DOMIZIL.sbf");
    }
    if (order[0] != 0)
        snd_VoiceBankOrders = snd_LoadVoiceBank(order, 0);
    if (click[0] != 0)
        snd_VoiceBankClick = snd_LoadVoiceBank(click, 0);
    if (*t == 0x1d || *t == 0x1e)
        snd_VoiceBankNoise = 0;
    else
        snd_VoiceBankNoise = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_KLICK_GERAEUSCHE.sbf", 0);
    if (greet[0] != 0)
        snd_VoiceBankGreeting = snd_LoadVoiceBank(greet, 0);
}
/* 0x4c2c00, unloads the voice banks of snd_LoadBuildingVoices */

void snd_UnloadBuildingVoices(void)
{
    if (snd_VoiceBankOrders != 0) {
        sb_UnloadSampleBank(snd_VoiceBankOrders);
        snd_VoiceBankOrders = 0;
    }
    if (snd_VoiceBankClick != 0) {
        sb_UnloadSampleBank(snd_VoiceBankClick);
        snd_VoiceBankClick = 0;
    }
    if (snd_VoiceBankNoise != 0) {
        sb_UnloadSampleBank(snd_VoiceBankNoise);
        snd_VoiceBankNoise = 0;
    }
    if (snd_VoiceBankGreeting != 0) {
        sb_UnloadSampleBank(snd_VoiceBankGreeting);
        snd_VoiceBankGreeting = 0;
    }
}
/* 0x4c2c70, a click comment of the character c (half the time a click noise; never for the building kind
   0x14): plays one of the worker banks loaded by snd_LoadBuildingVoices */

void snd_PlayWorkerClick(G16CHARCLICK *c)
{
    int r;

    r = util_RandomU(2);
    if (c->kind == 0x14)
        r = 0;
    if (snd_VoiceBankClick != 0 && r == 0) {
        snd_PlayVoice(c->id, snd_VoiceBankClick, "_ARBEITER_KOMMENTARE_KLICK", -1);
        return;
    }
    if (snd_VoiceBankNoise != 0 && r == 1)
        snd_PlayVoice(c->id, snd_VoiceBankNoise, "_ARBEITER_KOMMENTARE_KLICK", -1);
}
/* 0x4c2ce0, plays the click comment of the first selected character (walks the selected flag +0x188 of the
   ai_Chars records, the same click snd_PlayWorkerClick plays on a click of that character) */

void snd_PlaySelectedClick(void)
{
    int i;
    char *p;

    for (i = 0, p = ai_Selected; (int)p < (int)(ai_Selected + 0x300 * 0x21c); p += 0x21c, i++) {
        if (*p != 0) {
            snd_PlayWorkerClick((G16CHARCLICK *)(ai_CharsC + i * 0x21c));
            return;
        }
    }
}
/* 0x4c2d20, now and then (40% of the calls) a comment of the selected workers (ai_SceneChars[0x20]) about
   their favour with the player: the average gm_Liking favour over the selected workers selects the text
   ("SCHLECHTE_GUNST" < 0x32, "MITTLERE_GUNST" < 0x4b, "GUTE_GUNST" above), spoken by the more numerous
   sex (w / m, the last selected woman / man), -5 (a neutral voice) when the preferred one is missing
   @deep 65 code hunks, same operation / scheduling only: the whole original 0x4c2d20..0x4c2f2f read against the source, every constant and callee corresponds (the 0.4 double gate, snd_LoadVoiceBank of ARBEITER_KOMMENTARE_BEFEHL_HANDWERK.sbf, the 0x20 selected-worker scan at 0x7603a0 with the +0x188 selected and +9 sex bytes, men/women counts, the float favour sum truncated per step (fiadd + __ftol), f /= men+women, the 0x32/0x4b thresholds, the w/m choice and the -5 fallback, SCHLECHTE/MITTLERE/GUTE_GUNST). Left: ours CSEs snd_VoiceBankOrders into edx across the tail where the original compares memory (cmp ds:0x6f0cbc,esi) and reloads in every play branch; the original's out-of-line early-return block at 0x4c2e2a; the init store order (mov edi,ebx) and eax/edx choice for the men+women add. Readability debt after this session: Q2 ai_SceneChars (now unsigned short *[0x20]), gm_Liking. */

void snd_PlayFavourComment(void)
{
    AICHAR *c;
    int w;
    int m;
    int f;
    int men;
    int women;
    int i;

    women = 0;
    men = 0;
    f = 0;
    w = 0xffff;
    m = 0xffff;
    if (util_RandomF() > 0.4)
        return;
    if (snd_VoiceBankOrders == 0)
        snd_VoiceBankOrders = snd_LoadVoiceBank("ARBEITER_KOMMENTARE_BEFEHL_HANDWERK.sbf", 0);
    for (i = 0; i < 0x20; i++) {
        c = (AICHAR *)ai_SceneChars[i];
        if (c != 0 && c->selected != 0) {
            if (c->sex == 0) {
                men++;
                m = c->id;
                f += gm_Liking(ai_PlayerId, m, 1);
            } else {
                women++;
                w = c->id;
                f += gm_Liking(ai_PlayerId, w, 1);
            }
        }
    }
    if (men + women != 0)
        f /= men + women;
    if (f < 0x32)
        f = 0;
    else if (f < 0x4b)
        f = 1;
    else if (f > 0x4b)
        f = 2;
    if (men < women) {
        if (w != 0xffff) {
            if (f == 0)
                snd_PlayVoice(w, snd_VoiceBankOrders, "SCHLECHTE_GUNST", -1);
            else if (f == 1)
                snd_PlayVoice(w, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
            else if (f >= 2)
                snd_PlayVoice(w, snd_VoiceBankOrders, "GUTE_GUNST", -1);
            return;
        }
        if (m != 0xffff)
            return;
    } else if (m != 0xffff) {
        if (f == 0)
            snd_PlayVoice(m, snd_VoiceBankOrders, "SCHLECHTE_GUNST", -1);
        else if (f == 1)
            snd_PlayVoice(m, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
        else if (f >= 2)
            snd_PlayVoice(m, snd_VoiceBankOrders, "GUTE_GUNST", -1);
        return;
    }
    snd_PlayVoice(-5, snd_VoiceBankOrders, "MITTLERE_GUNST", -1);
}
/* 0x4c2f30, a comment about the player's favour from the selected character of the party record a (a random
   variation r, 3 when the first check is clear): "_SCHLECHTE_GUNST" / "_MITTLERE_GUNST" / "_GUTE_GUNST" by
   favour / 33. x is volatile because the original reloads it from its slot before every use (matching trick,
   do not clean).
   @deep 14 code hunks, one scheduling cluster only: the whole original 0x4c2f30..0x4c30df read against the source; ours differs only in the position of the pop esi (ours right after the two out calls, the original after the level test at 0x4c2fea), which shifts the esp offsets of out[1] and x by 4 in that one block; the next 102 instructions are equal. Meaning fixes done in this session: out is a 0xb8-byte unsigned array (the 0xbc frame: x at R-0xbc, out at R-0xb8), r is a 32-bit unsigned (the original's mov bp,ax + and ebp,0xffff), x is volatile (the original stores it to its slot and reloads it at every use - a matching trick, not cleaned), and the special-kind / generic calls are one ternary so that the compiler emits the original's single shared test tail. Readability debt: Q1 name, Q2 gm_BuildingGoodsList / gm_BuildingGoodsList17 / gm_Liking, Q4 the four raw casts (the party record +0x34 / +0x200 and the character +0x16c). */

void ai_FavourComment(char *a)
{
    volatile int x;
    unsigned out[0x2e];
    unsigned int r;
    short *c;
    char *g;
    int level;

    x = 0;
    r = util_RandomU(3);
    c = *(short **)(*(char **)(a + 0x34) + 0x200);
    if (c == 0)
        return;
    g = *(char **)((char *)c + 0x16c);
    level = (int)gm_Liking((unsigned short)ai_PlayerId, *(unsigned short *)(g + 0x27), 1) / 33;
    /* the three building kinds with their own work check, all others the generic one; only the first out value
       decides (a running order?) */
    if ((*g == 0x20 || *g == 0x1f || *g == 0x1e) ? gm_BuildingGoodsList17(g, out, 0) : gm_BuildingGoodsList(g, out, 0)) {
        if (out[1] == 0)
            x = 1;
    }
    if (level == 0) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_SCHLECHTE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_SCHLECHTE_GUNST", r);
    } else if (level == 1) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_MITTLERE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_MITTLERE_GUNST", r);
    } else if (level >= 2) {
        if (x)
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_GUTE_GUNST", 3);
        else
            snd_PlayVoice((unsigned short)*c, snd_VoiceBankGreeting, "_GUTE_GUNST", r);
    }
}
/* 0x4c30e0, starts the market ambience ("Athmo_Marktplatz_Mono_4Bit") at the market building's object:
   attaches it as the ambient track (volume 0x7f, range 2200.0), holds and plays it; does nothing when a
   track is already playing or the building has no object */

void snd_StartMarketAmbience(void)
{
    char *o;
    int smp;

    if (snd_AmbientTrack == 0) {
        o = ai_MarketBuildingV->object;
        if (o != 0) {
            smp = sb_GetSample(0, "Athmo_Marktplatz_Mono_4Bit");
            snd_AmbientTrack = d3snd_AttachI(smp, o, 0x7f, 2200.0f);
            if (snd_AmbientTrack != 0) {
                d3snd_SetHoldI(snd_AmbientTrack, 1);
                d3snd_Play(snd_AmbientTrack, 0, 0, 0);
            }
        }
    }
}
/* 0x4c3140, stops the market ambience: releases the hold, stops and detaches the ambient track, clears the
   handle */

void snd_StopMarketAmbience(void)
{
    if (snd_AmbientTrack != 0) {
        d3snd_SetHoldI(snd_AmbientTrack, 0);
        d3snd_StopI(snd_AmbientTrack, 1);
        d3snd_DetachI(snd_AmbientTrack);
        snd_AmbientTrack = 0;
    }
}

extern int * d3t_Rain;
extern char snd_TimeCopy[14];
extern int snd_RainDrops;
extern int snd_RainLightNear;
extern int snd_RainMediumNear;
extern int snd_RainHeavyNear;
extern int snd_RainLightFar;
extern int snd_RainMediumFar;
extern int snd_RainHeavyFar;
float snd_VoiceVolume = 1.0f;  /* 0x602b08, voice volume (1.0, scaled by 127) */
char snd_GustNames[3][0x32] = { "Boehe_Leicht_s", "Boehe_Mittel_s", "Boehe_Stark_s" };  /* 0x602b10, "Boehe_Leicht_s", "Boehe_Mittel_s", "Boehe_Stark_s" */
G16SEASON snd_Gusts[] = { { 0.699999988f, 0.5f, 16000 }, { 0.300000012f, 0.200000003f, 27000 }, { 0.899999976f, 0.600000024f, 12000 }, { 0.5f, 0.400000006f, 20000 } };  /* 0x602ba8, by season */
G16AMB ai_Unk602bd8[][5] = { { { 0.899999976f, 0.699999988f, 2, 4000, 0 }, { 0.449999988f, 0.300000012f, 2, 7000, 0 }, { 0.400000006f, 0.5f, 2, 30000, 0 }, { 0.5f, 0.600000024f, 0, 27000, 0 }, { 0.600000024f, 0.600000024f, 0, 20000, 0 } }, { { 0.699999988f, 0.699999988f, 2, 5000, 0 }, { 0.899999976f, 0.699999988f, 4, 5000, 0 }, { 0.699999988f, 0.5f, 2, 20000, 0 }, { 0.600000024f, 0.600000024f, 0, 25000, 0 }, { 0.899999976f, 0.600000024f, 0, 17000, 0 } }, { { 0.300000012f, 0.25f, 1, 17000, 0 }, { 0.200000003f, 0.200000003f, 1, 18000, 0 }, { 0.800000012f, 0.5f, 2, 12000, 0 }, { 0.300000012f, 0.600000024f, 0, 20000, 0 }, { 0.600000024f, 0.600000024f, 0, 20000, 0 } }, { { 0.150000006f, 0.200000003f, 1, 30000, 0 }, { 0.0f, 0.0f, 1, 500000, 0 }, { 0.200000003f, 0.400000006f, 2, 23000, 0 }, { 0.200000003f, 0.5f, 0, 22000, 0 }, { 0.400000006f, 0.5f, 0, 20000, 0 } } };  /* 0x602bd8, per season: birds, crickets, chickens, horses, dogs */
float snd_MusicVolume = 0.5f;  /* 0x602d68, music volume 0..1: 0 = no music ( */
G16TRACKS snd_BuildingTracks[] = { { 7, 0, -1, { "cd1\\LichtUndGnade.mp3", "cd1\\Moenchsgesang.mp3", "cd1\\Sakral.mp3", "cd2\\AmKuehlenGrabe.mp3", "", "" } }, { 2, 23, -1, { "cd1\\AufDemBall.mp3", "", "", "", "", "" } }, { 2, 20, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\RuestetEuch.mp3", "cd1\\FinstererPriester.mp3", "", "", "" } }, { 2, 19, -1, { "cd1\\Rittersleut.mp3", "cd1\\Burgfraeulein.mp3", "", "", "", "" } }, { 2, 21, -1, { "cd2\\DerSeuchenzug.mp3", "cd1\\ImDunklenTann.mp3", "", "", "", "" } }, { 2, 24, -1, { "cd1\\AufDemBall.mp3", "", "", "", "", "" } }, { 2, 22, -1, { "cd1\\finsterePlaene.mp3", "", "", "", "", "" } }, { 22, 0, -1, { "cd1\\AufspielZumTanz.mp3", "cd1\\ZurSommerzeit.mp3", "cd2\\KraeuterUndPhiolen.mp3", "", "", "" } }, { 19, 0, -1, { "cd1\\RuestetEuch.mp3", "cd1\\ImDunklenTann.mp3", "cd1\\Flucht.mp3", "cd1\\Rittersleut.mp3", "", "" } }, { 4, 0, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\Flucht.mp3", "cd2\\ImKerker.mp3", "cd1\\Hungerlohn.mp3", "", "" } }, { 16, 0, -1, { "cd1\\FinsterePlaene.mp3", "cd1\\flucht.mp3", "cd1\\Hungerlohn.mp3", "cd1\\ImDunklenTann.mp3", "", "" } }, { 11, 0, -1, { "cd1\\BeiDerQuelle.mp3", "cd1\\ImDunklenTann.mp3", "", "", "", "" } }, { 13, 0, 2, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 13, 0, 3, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 13, 0, -1, { "cd1\\Hungerlohn.mp3", "", "", "", "", "" } }, { 12, 0, 2, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 12, 0, 3, { "cd1\\Immerkalt.mp3", "", "", "", "", "" } }, { 12, 0, -1, { "cd1\\Hungerlohn.mp3", "", "", "", "", "" } }, { 28, 0, -1, { "", "", "", "", "", "" } }, { 14, 0, -1, { "cd2\\DasParfuem.mp3", "cd2\\DerFernhaendler.mp3", "cd2\\KraeuterUndPhiolen.mp3", "", "", "" } }, { 8, 0, -1, { "cd2\\KraeuterUndPhiolen.mp3", "cd2\\DasParfuem.mp3", "cd2\\DerFernhaendler.mp3", "", "", "" } }, { 6, 0, -1, { "cd2\\ArmerDelinquent.mp3", "cd2\\ImKerker.mp3", "", "", "", "" } }, { 18, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 18, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 18, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 20, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 20, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 20, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 21, 0, -1, { "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "", "" } }, { 21, 0, 2, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 21, 0, 3, { "cd1\\ImmerKalt.mp3", "cd1\\DieTaeglichMueh.mp3", "cd1\\Hungerlohn.mp3", "cd2\\AufDemMarkt.mp3", "", "" } }, { 5, 0, -1, { "cd2\\DerFernhaendler.mp3", "cd2\\ZumGutenEnd.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 9, 0, -1, { "cd2\\DerFernhaendler.mp3", "cd2\\ZumGutenEnd.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 15, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "cd1\\mauerundtor.mp3", "", "", "" } }, { 23, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 24, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 25, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 26, 0, -1, { "cd1\\AufDemBall.mp3", "cd2\\Hochzeit.mp3", "", "", "", "" } }, { 29, 0, -1, { "CD3\\DuestresGemaeuer.mp3", "CD3\\DunkleMondesnacht.mp3", "CD2\\AmKuehlenGrabe.mp3", "CD1\\FinstrerPriester.mp3", "CD3\\Grabesklang.mp3", "CD2\\DerSeuchenzug.mp3" } }, { 31, 0, -1, { "CD3\\DerStolzenTraum.mp3", "CD3\\Vagabundentanz.mp3", "CD3\\SchmerzlichErinnerung.mp3", "CD1\\ImDunklenTann.mp3", "CD1\\BeiDerQuelle.mp3", "CD1\\AufspielZumTanz.mp3" } }, { 32, 0, -1, { "CD3\\MitFlinkemStich.mp3", "CD3\\Medley.mp3", "CD2\\DasParfum.mp3", "CD2\\DerFernhaendler.mp3", "CD2\\Hochzeit.mp3", "CD1\\AufDemBall.mp3" } }, { 30, 587, -1, { "CD2\\ZumGutenEnd.mp3", "cd2\\DasParfuem.mp3", "cd2\\KraeuterUndPhiolen.mp3", "CD2\\DerFernhaendler.mp3", "", "" } }, { 30, 598, -1, { "CD3\\BurgUndFeste.mp3", "CD3\\Heldenglanz.mp3", "CD2\\Krieg.mp3", "CD2\\Satisfaktion.mp3", "CD1\\AufDenStrassen.mp3", "CD1\\FinsterePlaene.mp3" } }, { 30, 614, -1, { "CD2\\ArmerDelinquent.mp3", "CD2\\DerSeuchenzug.mp3", "CD1\\FinstererPriester.mp3", "CD1\\FinsterePlaene.mp3", "CD1\\AufDemBall.mp3", "CD3\\Heldenglanz.mp3" } }, { 0, 0, -1, { "", "", "", "", "", "" } } };  /* 0x602d70 */
G16LOC snd_LocationTracks[] = { { { 7, 0, 0, 0 }, "", -20, 0, 0 }, { { 2, 0, 0, 0 }, "", -20, 0, 0 }, { { 22, 0, 0, 0 }, "", -20, 0, 0 }, { { 19, 0, 0, 0 }, "", -20, 0, 0 }, { { 4, 0, 0, 0 }, "", -20, 0, 0 }, { { 16, 0, 0, 0 }, "", -20, 0, 0 }, { { 11, 0, 0, 0 }, "", -20, 0, 0 }, { { 13, 0, 0, 0 }, "", -20, 0, 0 }, { { 12, 0, 0, 0 }, "", -20, 0, 0 }, { { 28, 0, 0, 0 }, "", -20, 0, 0 }, { { 14, 8, 0, 0 }, "", -20, 0, 0 }, { { 6, 0, 0, 0 }, "", -20, 0, 0 }, { { 18, 20, 21, 0 }, "", -20, 0, 0 }, { { 5, 0, 0, 0 }, "", -20, 0, 0 }, { { 9, 0, 0, 0 }, "", -20, 0, 0 }, { { 15, 0, 0, 0 }, "", -20, 0, 0 }, { { 23, 0, 0, 0 }, "", -20, 0, 0 }, { { 24, 0, 0, 0 }, "", -20, 0, 0 }, { { 25, 0, 0, 0 }, "", -20, 0, 0 }, { { 26, 0, 0, 0 }, "", -20, 0, 0 }, { { 29, 0, 0, 0 }, "", -20, 0, 0 }, { { 31, 0, 0, 0 }, "", -20, 0, 0 }, { { 32, 0, 0, 0 }, "", -20, 0, 0 }, { { 30, 0, 0, 0 }, "", -20, 0, 0 }, { { 9876, 0, 0, 0 }, "", -20, 0, 0 }, { { 0, 0, 0, 0 }, "", 0, 0, 0 } };  /* 0x6094d0 */
unsigned int ai_Unk60b0d8 = 1;  /* 0x60b0d8, start of the music pause */
char snd_TrackText[256] = {0};  /* 0x6f0760, a debug text */
char ai_Unk6f0860[256] = {0};  /* 0x6f0860, a debug text */
char ai_Unk6f0960[256] = {0};  /* 0x6f0960, a debug text */
char snd_DebugText[256] = {0};  /* 0x6f0a60, a debug text */
unsigned char snd_AmbientOn = 0;  /* 0x6f0b60, 1: ambient sounds on */
unsigned int snd_AmbientOnSaved = 0;  /* 0x6f0b64, the saved snd_AmbientOn */
int snd_QueuePaused = 0;  /* 0x6f0b68, nonzero: the queue does not advance */
G16TRACKHANDLES snd_TrackHandles = {0};  /* 0x6f0b6c, the music track and its copy */
char * snd_SpecialTrack = 0;  /* 0x6f0b74, the special track of */
int snd_AmbientSaved = 0;  /* 0x6f0b78, 1 while the ambient flag is off and saved ( */
int snd_FanfareFade = 0;  /* 0x6f0b7c, 1 while a "Fanfare" has faded the music out */
int snd_AmbientTrack = 0;  /* 0x6f0b80, the d3snd handle of the ambient track (bird song, market ambience) */
G16SND * snd_Queue = 0;  /* 0x6f0b84, the queue (the first queued sound) */
int snd_QueueTime = 0;  /* 0x6f0b88, time of the last queue change */
int snd_Season = 0;  /* 0x6f0b8c, the season */
unsigned int snd_LastGust = 0;  /* 0x6f0b90, time of the last gust */
int ai_Unk6f0b94 = 0;  /* 0x6f0b94, hpLocustsPerm */
int ai_Unk6f0b98 = 0;  /* 0x6f0b98, hpLocusts */
int ai_Unk6f0b9c = 0;  /* 0x6f0b9c, the season */
void * ai_Unk6f0ba0 = 0;  /* 0x6f0ba0, the atmosphere mp3 (MSXTRACK *) */
char snd_LastTrack[256] = {0};  /* 0x6f0ba8, the outdoor track played last */
unsigned char * ai_Unk6f0ca8 = 0;  /* 0x6f0ca8, the building of the track */
int ai_Unk6f0cac = 0;  /* 0x6f0cac, the room of the track */
unsigned int ai_Unk6f0cb0 = 0;  /* 0x6f0cb0, the pause length */
int ai_Unk6f0cb4 = 0;  /* 0x6f0cb4, the season of the music */
unsigned int ai_Unk6f0cb8 = 0;  /* 0x6f0cb8, time of the last track decision */
int snd_VoiceBankOrders = 0;  /* 0x6f0cbc, voice bank: orders (snd_LoadBuildingVoices) */
int snd_VoiceBankClick = 0;  /* 0x6f0cc0, voice bank: click comments */
int snd_VoiceBankNoise = 0;  /* 0x6f0cc4, voice bank: click noises */
int snd_VoiceBankGreeting = 0;  /* 0x6f0ccc, voice bank: greeting */
int snd_Bank = 0;  /* 0x6f2404, the sample bank */
int snd_MusicEnabled = 0;  /* 0x6f2578, the "msx" setting of game.INI [Sound]: music enabled */
int snd_SfxEnabled = 0;  /* 0x6f257c, the "sfx" setting of game.INI [Sound]: effects and voices on */
int * d3t_Rain = 0;  /* 0x760618, the rain level */
/* ai_Selected: view of ai_Chars (tools/data_views.py) */
char snd_TimeCopy[14] = {0};  /* 0xf81240, a copy of ai_Time */
int snd_RainDrops = 0;  /* 0xf81254, the "Tropfen_s" loop */
int snd_RainLightNear = 0;  /* 0xf8125c, "LeichtNah_l_01" */
int snd_RainMediumNear = 0;  /* 0xf81264, "MittelNah_l_01" */
int snd_RainHeavyNear = 0;  /* 0xf8126c, "StarkNah_l_01" */
int snd_RainLightFar = 0;  /* 0xf81274, "LeichtFern_l_01" */
int snd_RainMediumFar = 0;  /* 0xf8127c, "MittelFern_l_01" */
int snd_RainHeavyFar = 0;  /* 0xf81284, "StarkFern_l_01" */
