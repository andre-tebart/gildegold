/* 0x4a85e0, deals the fifth card to each seat, open */

void ai_Unk4a85e0(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        ai_Unkf83654->seat[i].card[4] = ai_Unk4a7760(i);
        ai_Unkf83660[ai_Unkf83654->seat[i].card[4]].shown = 1;
    }
    ai_Unkf83654->unkb5++;
}
