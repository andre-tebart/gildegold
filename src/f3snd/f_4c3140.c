/* 0x4c3140, stops the market ambience: releases the hold, stops and detaches the ambient track, clears the
   handle */

void snd_StopMarketAmbience(void)
{
    if (snd_AmbientTrack != 0) {
        d3snd_SetHold(snd_AmbientTrack, 0);
        d3snd_Stop(snd_AmbientTrack, 1);
        d3snd_Detach(snd_AmbientTrack);
        snd_AmbientTrack = 0;
    }
}
