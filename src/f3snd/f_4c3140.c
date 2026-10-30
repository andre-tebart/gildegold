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
