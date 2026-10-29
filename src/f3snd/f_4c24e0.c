/* 0x4c24e0, the voice variant of a speaker (voice number, woman): -2 / -1 for women (voice & 1), -5..-3 for
   men (voice % 3) - the negative ids 0x4c2510 turns into "_W1" / "_W2" / "_M1".."_M3" */

int snd_VoiceVariant(unsigned int voice, int woman)
{
    if (woman == 1)
        return (voice & 1) - 2;
    return voice % 3 - 5;
}
