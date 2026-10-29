/* 0x50c4e0 */

void gm_Unk50c4e0(int a)
{
    unsigned char *b;
    char *o;

    b = gm_FirstBuilding(1);
    o = gm_Query(*(void **)(b + 0x5d), 2, 6, 0, 0x115);
    cm_SendMoveGoodsUnchecked(-1, *(int *)(o + 2), ai_OwnPlayer, a);
}
