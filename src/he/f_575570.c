/* 0x575570, the low outcome of the event 0xa4: per +4 pays (0x4c4830) the amount 0x5089c0 (0x2b7, 0x6f1bc5) times
   (2 * random (0x42ec20) + 0x5eb780) */

void he_Unk575570(char *per)
{
    float f;

    f = ((float (*)(int, unsigned char))gm_GoodsPrice)(0x2b7, *(unsigned char *)(unsigned)&ai_OwnPlayer);
    ((int (*)(int, int, unsigned char, int))cm_SendMoveGoodsUnchecked)(HM_I(per, 4), -1, *(unsigned char *)(unsigned)&ai_OwnPlayer, (int)((util_RandomF() * 2.0f + *(float *)(unsigned)&ai_Unk5eb780) * f));
}
