/* 0x4cdb20 */

void ai_Unk4cdb20(char *c)
{
    char *q;
    int id;

    for (q = gm_Query(*(int *)(c + 0x178), 1, 5); q != 0; q = gm_QueryNext()) {
        id = *(unsigned short *)q;
        if (ai_Unk4cd9d0(id) != 0)
            cm_SendExSellObject(-1, *(int *)(c + 4), id, *(int *)(q + 0xe), ai_OwnPlayer, 0);
    }
}
