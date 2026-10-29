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
