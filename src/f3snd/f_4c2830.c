/* 0x4c2830, loads the voice sample bank name: "sprache_<language>\<name>" ("german"), or the plain
   "sprache\<name>" when that fails; returns the bank handle or 0 */

int snd_LoadVoiceBank(char *name, int a)
{
    char buf[0x104];
    int b;

    sprintf(buf, "sprache_%s\\%s", "german", name);
    b = sb_LoadSampleBank(buf, a);
    if (b == 0) {
        sprintf(buf, "sprache\\%s", name);
        b = sb_LoadSampleBank(buf, a);
    }
    return b;
}
