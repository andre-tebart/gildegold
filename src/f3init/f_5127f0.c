/* 0x5127f0, starts the looped sound name once (its volume scaled by the 0.7 of 0x61adfc) and keeps its handle in
   the table 0x76fd80 (16 entries) */


void init_PlayTownSound(char *name, int volume, int delay)
{
    int h;

    h = sb_PlaySample(0, name, 0, 0x3f, (int)(volume * init_SoundVolume), 1, delay);
    if (init_TownSoundCount < 0x10) {
        init_TownSounds[init_TownSoundCount] = h;
        init_TownSoundCount++;
    }
}
