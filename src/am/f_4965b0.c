/* 0x4965b0, execute of 0x4960b0 */

int ai_Unk4965b0(AIPER *p, AIARG *s1, AIARG *s2)
{
    AIXMSG m;

    cm_SendMoneyDelta(p->netId, -s1->unk10);
    m.flags = 1;
    m.type = 0x60;
    m.to = p->netId;
    m.fromId = -1;
    m.unk5a = (unsigned char)s1->unk04;
    cm_SendLongMessage((AIMSG *)&m);
    return 4;
}
