/* 0x4c87a0, handler of type 0x1d, "cm_ExSetHE": sets the three times and the text block of job data[0] */

int ai_Unk4c87a0(G17MSG *m, G17STAT *s)
{
    char buf[0x100];
    char *p;

    if (s != 0) {
        s->state = 2;
        s->unk01 = 4;
        s->value = 0;
    }
    p = he_FindEvent(1, 1, *(int *)&m->data[0]);
    if (p == 0) {
        sprintf(buf, "cm_ExSetHE(): He not found. id:%i name:%s", *(int *)&m->data[0], &m->data[0x2f]);
        return 1;
    }
    *(G17T14 *)(p + 0x44) = *(G17T14 *)&m->data[4];
    *(G17T14 *)(p + 0x52) = *(G17T14 *)&m->data[0x12];
    *(G17T14 *)(p + 0x60) = *(G17T14 *)&m->data[0x20];
    *(int *)(p + 0x70) = (char)m->data[0x2e];
    memcpy(p + 0xac, ai_Unka68d40, 0xa0);
    if (s != 0) {
        s->state = 1;
        s->unk01 = 4;
        s->value = (int)p;
    }
    return 0;
}
