/* 0x4c67c0, check of message 0x53 ('cler' / 'buy ' on the object 300 of building data[8]) */

int ai_Unk4c67c0(G17MSG *m)
{
    char *b, *o;
    int i;

    if (*(int *)&m->data[0] == 0x636c6572) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0) {
            o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 300);
            if (o != 0)
                return *(int *)ai_Time - *(int *)(o + 0x28) <= 1;
        }
    } else if (*(int *)&m->data[0] == 0x62757920) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0) {
            o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 300);
            if (o != 0) {
                for (i = 0; i < 3; i++)
                    if (((int *)(o + 0x1c))[i] == *(int *)&m->data[0xc])
                        return 0;
            }
        }
    }
    return 1;
}
