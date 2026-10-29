/* 0x497630, execute callback of 0x497490's kind 10 */

int ai_Unk497630(G5CH *p, AIARG *s1)
{
    if (s1->kind != 10)
        return 0;
    return amt_ApplyForOffice(p, s1->unk04) ? 10 : 0;
}
