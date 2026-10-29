/* 0x577230, when `arg` is 0: posts the message 0x2bc0 for the player's character id (0x770b80 + id * 0x21c, +4)
   and a fight message (0x4c4880) on the record of 0x500ec0 (0x155): (id +2, -1, 0x155, 3, 0x6f1bc5, 0) */

void ai_Unk577230(int arg)
{
    char *r;
    int id;

    if (arg)
        return;
    r = (char *)gm_FindMarketObject(0x155);
    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
    cm_SendExSellObject(*(int *)(r + 2), -1, 0x155, 3, *(unsigned char *)(unsigned)&ai_OwnPlayer, 0);
}
