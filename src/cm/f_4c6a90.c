/* 0x4c6a90, handler of type 8: allocates the load buffer */

int ai_Unk4c6a90(G17MSG *m, G17STAT *s)
{
    ai_Unka68d28 = *(int *)&m->data[0];
    ai_Unkf31140 = mem_Alloc(ai_Unka68d28, "cm_LoadBuf");
    ai_Unkf81220 = 0;
    if (s != 0)
        s->state = 1;
    return 0;
}
