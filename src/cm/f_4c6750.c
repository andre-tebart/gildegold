/* 0x4c6750, check of message 0x38: 1 unless building id has flag 2 at 0x5a */

int ai_Unk4c6750(G17MSG *m)
{
    char *b;

    b = gm_BuildQuery(1, 1, *(int *)&m->data[0]);
    if (b != 0 && (b[0x5a] & 2))
        return 0;
    return 1;
}
