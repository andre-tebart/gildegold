/* 0x4c69c0, check of message 0x59: 1 if one of the 16 characters is (6 / 7) bound to another id */

int ai_Unk4c69c0(G17MSG *m)
{
    char *c;
    int i;

    if (*(int *)&m->data[0] != -1) {
        for (i = 0; i < 0x10; i++) {
            c = gm_FindChar(((int *)&m->data[4])[i]);
            if (c != 0 && (c[2] == 6 || c[2] == 7) && *(int *)(c + 0x20c) != -1 && *(int *)(c + 0x20c) != *(int *)&m->data[0])
                return 1;
        }
        return 0;
    }
    return 1;
}
