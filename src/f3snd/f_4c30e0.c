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
            snd_AmbientTrack = d3snd_Attach(smp, o, 0x7f, 2200.0f);
            if (snd_AmbientTrack != 0) {
                d3snd_SetHold(snd_AmbientTrack, 1);
                d3snd_Play(snd_AmbientTrack, 0, 0, 0);
            }
        }
    }
}
