/* 0x4a8520, deals three cards to each seat (the first two hidden, the third open) */

void ai_Unk4a8520(void)
{
    int k, i;

    for (k = 0; k < 3; k++) {
        for (i = 0; i < 3; i++) {
            ai_Unkf83654->seat[i].card[k] = ai_Unk4a7760(i);
            if (k > 1)
                ai_Unkf83660[ai_Unkf83654->seat[i].card[k]].shown = 1;
            else
                ai_Unkf83660[ai_Unkf83654->seat[i].card[k]].shown = 0;
        }
    }
    ai_Unkf83654->unkb5 += 3;
}
