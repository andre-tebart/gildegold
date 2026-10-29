/* 0x4e3620 */

int ai_Unk4e3620(int a)
{
    if (ai_Unk6f0d8c == 0) {
        snd_ClearQueue();
        return ((int (*)(char *))snd_PlaySpecialTrack)(a);
    }
}
