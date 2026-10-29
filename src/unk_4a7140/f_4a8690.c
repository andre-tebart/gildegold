/* 0x4a8690, ends the round: the winner into unkb8, the pot to the player if he won, all cards back */

void ai_Unk4a8690(void)
{
    int i;

    ai_Unkf83654->unkb8 = (short)ai_Unk4a8190();
    if (ai_Unkf83654->unkb8 == 0)
        cm_SendMoveGoodsUnchecked(*(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), -1, ai_OwnPlayer,
                     ai_Unkf83654->unka0);
    for (i = 0; i < 24; i++) {
        ai_Unkf83660[i].owner = 3;
        ai_Unkf83660[i].shown = 0;
    }
}
