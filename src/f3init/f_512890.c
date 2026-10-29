/* 0x512890, stops all sounds of the town table (0x427290) */


void unk_Unk512890(void)
{
    int i, n;

    i = 0;
    if (0 < init_TownSoundCount) {
        do {
            sb_Stop(init_TownSounds[i], 1);
            n = init_TownSoundCount;
            init_TownSounds[i] = 0;
            i++;
        } while (i < n);
    }
    init_TownSoundCount = 0;
}
