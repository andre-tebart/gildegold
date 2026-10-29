/* 0x4a7a20, every seat still in (unk28 < 0) pays the stake into the pot (0xa0); the player's from p's money */

void ai_Unk4a7a20(void)
{
    int i;

    for (i = 0; i < 3; i++) {
        if (ai_Unkf83654->seat[i].unk28 < 0) {
            ai_Unkf83654->unka0 += ai_Unkf83654->stake2;
            if (i == 0) {
                gm_PlayerFactor(ai_Unkf83654->stake2, ai_OwnPlayer);
                cm_SendMoveGoodsUnchecked(-1, *(int *)((unsigned char *)ai_Unkf83654->seat[0].who + 4), ai_OwnPlayer,
                             ai_Unkf83654->stake2);
            }
            ai_Unkf83654->seat[i].unk1c += ai_Unkf83654->stake2;
        }
    }
}
