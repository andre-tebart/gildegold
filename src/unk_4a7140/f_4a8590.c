/* 0x4a8590, deals the fourth card to each seat, open */

void ai_Unk4a8590(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_Unkf83654->seat[i].card[3] = ai_Unk4a7760(i);
        ai_Unkf83660[ai_Unkf83654->seat[i].card[3]].shown = 1;
    }
    ai_Unkf83654->unkb5++;
}
