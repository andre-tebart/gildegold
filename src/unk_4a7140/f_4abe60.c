/* 0x4abe60, execute callback of action 0x47 "Ehrerbietung": p is flagged (0x1cc bit 5), pays m + 0x10 and wins
   five times that in standing with the character of m */

int ai_Unk4abe60(AIPER *p, unsigned char *m)
{
    AIPER *o;

    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    cm_SendSetField(p->netId, 0x1cc, 4, 0x20, 0);
    gm_SendMoneyDelta(p->netId, -*(int *)(m + 0x10));
    cm_SendRelations(p->id, o->id, *(int *)(m + 0x10) * 5, 1);
    return 0x47;
}
