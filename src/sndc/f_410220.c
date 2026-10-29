/* 0x410220, registers the sound script commands (LoadSampleBank ... WaitForSample) and the SND_* script
   globals with the event system (evt_ImportCommand / evt_AddGlobal) */

void sndc_Register(void)
{
    evt_ImportCommand("LoadSampleBank", (int (*)())sndc_LoadSampleBank, 1, 1, 6);
    evt_ImportCommand("KillSampleBank", (int (*)())sndc_KillSampleBank, 1, 1, 1);
    evt_ImportCommand("PlaySampleHandle", (int (*)())sndc_PlaySampleHandle, 1, 2, 1, 6);
    evt_ImportCommand("PlaySample", (int (*)())sndc_PlaySample, 1, 2, 6, 1);
    evt_ImportCommand("PlaySample3D", (int (*)())sndc_PlaySample3D, 1, 4, 1, 1, 6, 1);
    evt_ImportCommand("StopSample", (int (*)())sndc_StopSample, 1, 1, 1);
    evt_ImportCommand("GetSampleBankHandle", (int (*)())sndc_GetSampleBankHandle, 1, 1, 6);
    evt_ImportCommand("GetSampleHandle", (int (*)())sndc_GetSampleHandle, 1, 2, 1, 6);
    evt_ImportCommand("SpeechQueued", (int (*)())sndc_SpeechQueued, 1, 5, 1, 1, 6, 1, 1);
    evt_ImportCommand("WaitForSample", (int (*)())sndc_WaitForSample, 5, 1, 1);
    evt_AddGlobal("SND_W2", &snd_W2, 1);
    evt_AddGlobal("SND_W1", &snd_W1, 1);
    evt_AddGlobal("SND_M1", &snd_M1, 1);
    evt_AddGlobal("SND_M2", &snd_M2, 1);
    evt_AddGlobal("SND_M3", &snd_M3, 1);
    evt_AddGlobal("SND_HS", &snd_HS, 1);
    evt_AddGlobal("SND_NONE", &snd_NONE, 1);
    evt_AddGlobal("SND_VAR", &snd_VAR, 1);
}
