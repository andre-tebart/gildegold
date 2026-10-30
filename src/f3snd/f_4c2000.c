/* 0x4c2000, the music step: a new season stops the outdoor track; a finished track starts a pause (its name kept
   in snd_LastTrack); after the pause, or when the player moved to another building / room (and the current track
   loses a random draw against its priority), the track of the place starts (0x4c18a0 / 0x4c1bd0)
   @deep B draft, 279 code hunks / 18 regs, no meaning error left: read the whole original 0x4c2000..0x4c23ef against the source. Fixed here: t = F3SND_TIME (the original reads the volatile d3_Time three times; the source had d3_Time * 0xd, one read), the pause length (int)((1.0f - snd_MusicVolume) * 240000.0f) (0x5eb780=1.0f, 0x5edc0c=240000.0f, call 0x5de518 = __ftol) instead of util_RandomF()*60000.0f, and the five debug texts to the exe's strings (seasonchange, reached its end!, pauseLength is, propability). Remaining: register allocation and basic-block layout only - ours keeps the same instruction multiset up to register choice but hoists loads (e.g. the t computation uses ebx where the original uses edi), inlines the cb4==0 early return where the original jumps to an out-of-line copy, and inverts the final branch polarity; the time/branch/loop/table walks, every constant and callee correspond. Readability debt: Q1 ai_Unk4c2000, Q2 ai_GameState, snd_MusicVolume, ai_Unk6f0cb4, ai_Unk6f0ca8, ai_Unk6f0cac, ai_Unk6f0cb8, ai_Unk6f0cb0, ai_Unk60b0d8, snd_SpecialTrack, ai_Unk6f0860, ai_PlayerBuilding/230c/2310, Q4 the current-track bytes [0x104]/[0x108]/[0x10c]. */

void ai_Unk4c2000(void)
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
    i = gm_ClockSeason(ai_Time);
    if (ai_Unk6f0cb4 != i) {
        if (ai_Unk6f0cb4 == 0) {
            ai_Unk6f0cb4 = gm_ClockSeason(ai_Time);
            return;
        }
        i = snd_FindTrackKind(0x2694);
        ai_Unk6f0cb4 = gm_ClockSeason(ai_Time);
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
