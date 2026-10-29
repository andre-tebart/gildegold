/* 0x512840, the ambient sound of a graveyard building b of the type 0x1e on the map (the word +0x29 == 0xfd): "AesteKnacken_s" */


void init_PlayGraveyardSound(char *b)
{
    gm_ClockSeason(ai_Time);
    if (b != 0) {
        if (gm_IsHomeType(b) != 0 && *(short *)(b + 0x29) == 0xfd && *b == 0x1e)
            init_PlayTownSound("AesteKnacken_s", 0x50, 8000);
    }
}
