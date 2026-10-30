/* 0x4a7760, deals a card from the deck to owner (the first one when 10 or fewer are left, else a random one) */

int ai_Unk4a7760(int owner)
{
    unsigned int i;

    if (ai_Unkf83654->left == 0)
        return -1;
    if (ai_Unkf83654->left < 11) {
        for (i = 0; i < 24; i++)
            if (ai_Unkf83660[i].owner == 3)
                break;
    } else {
        do
            i = util_RandomU(24);
        while (ai_Unkf83660[i].owner != 3);
    }
    ai_Unkf83660[i].owner = owner;
    ai_Unkf83654->left--;
    return i;
}
