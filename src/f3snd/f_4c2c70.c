/* 0x4c2c70, a click comment of the character c (half the time a click noise; never for the building kind
   0x14): plays one of the worker banks loaded by snd_LoadBuildingVoices */

void snd_PlayWorkerClick(G16CHARCLICK *c)
{
    int r;

    r = util_Random(2);
    if (c->kind == 0x14)
        r = 0;
    if (snd_VoiceBankClick != 0 && r == 0) {
        snd_PlayVoice(c->id, snd_VoiceBankClick, "_ARBEITER_KOMMENTARE_KLICK", -1);
        return;
    }
    if (snd_VoiceBankNoise != 0 && r == 1)
        snd_PlayVoice(c->id, snd_VoiceBankNoise, "_ARBEITER_KOMMENTARE_KLICK", -1);
}
