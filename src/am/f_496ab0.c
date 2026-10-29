/* 0x496ab0, execute of 0x496610 */

int ai_Unk496ab0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->unk10);
    m.to = p->netId;
    m.fromId = p->netId;
    m.flags = 1;
    m.type = 0x60;
    m.unk5a = (unsigned char)s1->unk04;
    cm_SendLongMessage((AIMSG *)&m);
    return 5;
}
