/* 0x4c18a0, the music when the player enters a building: the current track loses 30 (20 without a building of
   its kinds); the building's kind gets its track (resumed, or a random one of the matching ai_Unk602d70 entry:
   kind, room (0: any), season (-1: any), up to six names), started with 0x423a90
   @deep B draft, 337 code hunks, no meaning error found: read the whole original 0x4c18a0..0x4c1bc0 against the source. The building fallback (2308 then 230c), kind = ai_BTypes[*b * 0x24d] (the original's base + b + 588*b addressing), season = gm_ClockSeason(ai_Time), the current track i = snd_FindActiveTrack() losing 0x1e (outdoor kind 0x2694) or 0x14 + 0x1e by the gm_BuildQuery(2,4,player,5,kind) match, snd_FindTrackKind(kind), the empty-track search through ai_Unk602d70 (kind, room 0/== word [room], season/-1) with the e[1].kind end test, the 6-name scan, util_Random(j) (& 0xffff), the strlen checks, strcpy and the resume message, then active = 1, the same kinds loop adding 0x1e/0x14, msx_Play(track,0) and sprintf - all correspond. What is left is scheduling / register allocation: frame 0xc vs 0x10, kind/season share one slot in the original, the table walks keep different register webs. Readability debt: Q2 snd_FindActiveTrack / snd_FindTrackKind (the current / kind track lookup, declared here and in cm.h), gm_BuildQuery (the favour query), ai_Unk602d70 (the track table), snd_LocationTracks (the location tracks), ai_Unk6f0a60 (log text), snd_TrackHandles.current / snd_TrackHandles.copy (the music track), ai_PlayerBuilding / 230c (the building), sv_PanelShown (its room), ai_PlayerId (the player). */

void snd_EnterBuilding(void)
{
    unsigned char *b;
    unsigned int kind;
    int season;
    int i, k, j;
    G16TRACKS *e;
    G16LOC *l;
    char *msg;

    b = ai_PlayerBuildingB;
    if (b == 0 && (b = ai_FallbackBuildingB) == 0)
        return;
    k = 0;
    kind = (unsigned char)ai_BTypes[*b * 0x24d];
    season = gm_ClockSeason(ai_Time);
    i = snd_FindActiveTrack();
    if (i >= 0) {
        l = &snd_LocationTracks[i];
        l->active = 0;
        if (l->kinds[0] == 0x2694)
            l->prio -= 0x1e;
        else {
            for (; l->kinds[k] != 0; k++) {
                if (gm_BuildQuery(2, 4, ai_PlayerId, 5, l->kinds[k]) != 0) {
                    l->prio -= 0x1e;
                    break;
                }
            }
            if (snd_LocationTracks[i].kinds[k] == 0)
                snd_LocationTracks[i].prio -= 0x14;
        }
    }
    i = snd_FindTrackKind(kind);
    if (i < 0)
        return;
    l = &snd_LocationTracks[i];
    if (strlen(l->track) == 0) {
        if (ai_Unk602d70[0].kind == 0)
            return;
        for (e = ai_Unk602d70; e->kind != kind || e->room != 0 && e->room != (unsigned int)*sv_PanelShown
             || season != e->season && e->season != -1; e++) {
            if (e[1].kind == 0)
                return;
        }
        for (j = 0; j < 6 && e->names[j][0] != 0; j++)
            ;
        j = util_Random(j);
        if (strlen(e->names[j]) == 0)
            return;
        strcpy(l->track, e->names[j]);
        msg = "Starting location track %s";
    } else
        msg = "Resuming location track %s";
    l->active = 1;
    for (k = 0; snd_LocationTracks[i].kinds[k] != 0; k++) {
        if (gm_BuildQuery(2, 4, ai_PlayerId, 5, snd_LocationTracks[i].kinds[k]) != 0) {
            l->prio += 0x1e;
            break;
        }
    }
    if (snd_LocationTracks[i].kinds[k] == 0)
        l->prio += 0x14;
    snd_TrackHandles.current = msx_Play(l->track, 0);
    snd_TrackHandles.copy = snd_TrackHandles.current;
    sprintf(ai_Unk6f0a60, msg, l->track);
}
