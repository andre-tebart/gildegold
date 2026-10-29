/* 0x4a3a90, the next entry of the ring */

G6RING *ai_Unk4a3a90(void)
{
    G6RING *r = ai_Unk60034c;

    if (ai_Unk6f070c == 0)
        return ai_Unkf83c20;
    ai_Unk60034c = &ai_Unkf83c20[(unsigned int)(ai_Unk60034c - ai_Unkf83c20 + ai_Unk600350) % ai_Unk6f070c];
    return r;
}
