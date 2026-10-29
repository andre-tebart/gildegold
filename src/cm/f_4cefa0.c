/* 0x4cefa0, removes all floating numbers */

void ai_Unk4cefa0()
{
    G18POP *p;

    for (p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); p++) {
        if (p->h != -1)
            d2_RemoveObj(p->h);
        p->h = -1;
        p->v = 0;
        p->c = 0;
    }
}
