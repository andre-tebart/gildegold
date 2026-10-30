/* 0x4c1bd0, the music when the player leaves a building: the outdoor track (kind 0x2694), resumed or a new random
   one by season, not the one played last (snd_LastTrack) */

void snd_PlayOutdoorTrack(void)
{
    int season;
    int i;
    int r;
    char *t;

    season = gm_ClockSeason(ai_Time);
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
