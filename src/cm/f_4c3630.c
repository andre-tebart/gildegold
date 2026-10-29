/* 0x4c3630, waits for the answer to 0x4c3600; 1: none */

int ai_Unk4c3630(void)
{
    G17MSG *m;

    m = ai_Unk4c4030(3, 0x165);
    if (m == 0)
        return 1;
    if (*(int *)&m->data[0] != 0xa03ef57c)
        return 1;
    ai_Unk4cd580(*(int *)&m->data[4], *(int *)&m->data[8]);
    ai_Unk4c4130(m);
    return 0;
}
