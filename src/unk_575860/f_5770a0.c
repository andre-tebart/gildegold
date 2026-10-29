/* 0x5770a0, when `arg` is 0: posts the message 0x2bc0 for the player's character id (0x770b80 + id * 0x21c, +4) */

void ai_Unk5770a0(int arg)
{
    int id;

    if (arg)
        return;
    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x2bc0);
}
