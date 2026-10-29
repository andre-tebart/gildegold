/* 0x4cef80 */

void ai_Unk4cef80()
{
    G18POP *p;

    for (p = ai_Unk9304c0; (int)p < (int)(ai_Unk9304c0 + 16); p++) {
        p->h = -1;
        p->v = 0;
        p->c = 0;
    }
}
