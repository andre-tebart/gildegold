/* 0x4c6870, check of message 0x54 ('join' / 'leav' / 'cler' on the object 0x12d of building data[8]) */

int ai_Unk4c6870(G17MSG *m)
{
    char *b, *o, *c;
    int i;
    unsigned short k;

    if (*(int *)&m->data[0] == 0x6a6f696e) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0 && (o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == *(int *)&m->data[4])
                    return 1;
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == -1)
                    return i > 3;
        }
    } else if (*(int *)&m->data[0] == 0x6c656176) {
        b = gm_BuildQuery(1, 1, *(int *)&m->data[8]);
        if (b != 0 && (o = gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d)) != 0) {
            for (i = 0; i < 4; i++)
                if (((int *)(o + 0x1c))[i] == *(int *)&m->data[4])
                    break;
            if (i < 4)
                return 0;
        }
    } else if (*(int *)&m->data[0] == 0x636c6572 && (b = gm_BuildQuery(1, 1, *(int *)&m->data[8])) != 0) {
        k = *(unsigned short *)(b + 0x27);
        if (k != 0xffff) {
            c = ai_Chars[k];
            if (c != 0 && c != ai_PlayerCharC && c[8] != 0 && c[2] != 0xf)
                return 1;
        }
        return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x12d) == 0;
    }
    return 1;
}
