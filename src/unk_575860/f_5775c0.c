/* 0x5775c0, when `arg` is 0: two messages on the player's character id (0x770b80 + id * 0x21c, +4):
   0x4c4a60 (id, 0x1c8, 4, 0x4000, 0) and 0x4c4830 (id, -1, 0, 0x3e80) */

void ai_Unk5775c0(int arg)
{
    char *rec;
    int id;

    id = *(int *)((unsigned)&ai_CharInts + *(unsigned short *)(unsigned)&ai_PlayerId * 0x21c);
    if (arg)
        return;
    cm_SendSetField(id, 0x1c8, 4, 0x4000, 0);
    cm_SendMoveGoodsUnchecked(id, -1, 0, 0x3e80);
}
