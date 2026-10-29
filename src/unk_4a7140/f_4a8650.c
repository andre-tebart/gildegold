/* 0x4a8650, the ante: three stakes into the pot, the player's from p's money */

void ai_Unk4a8650(void)
{
    ai_Unkf83654->unka0 = ai_Unkf83654->stake2 * 3;
    cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), ai_OwnPlayer, ai_Unkf83654->stake2);
}
