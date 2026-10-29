/* 0x4ab6b0, execute callback of action 0x44 "Blutrache": p pays the cost, is flagged (0x1cc bit 3), the victim
   loses standing with every helper by their relation (0x504e20), and 0x5a9470 starts the feud */

int ai_Unk4ab6b0(AIPER *p, unsigned char *m, unsigned char *m2)
{
    AIPER *who[4];
    AIPER *o;
    int cost, n, i;

    cost = *(int *)(m + 0x10);
    if (m2 == 0 || p == 0)
        return 0;
    o = AI_505DF0(*(int *)(m + 4));
    if (o == 0)
        return 0;
    who[0] = AI_505DF0(*(int *)(m + 0xc));
    if (who[0] == 0)
        return 0;
    n = 1;
    if ((who[n] = AI_505DF0(*(int *)(m + 8))) == 0)
        n++;
    if ((who[n] = AI_505DF0(*(int *)(m2 + 0xc))) == 0)
        n++;
    if ((who[n] = AI_505DF0(*(int *)(m2 + 8))) == 0)
        n++;
    gm_SendMoneyDelta(p->netId, -cost);
    cm_SendSetField(p->netId, 0x1cc, 4, 8, 0);
    for (i = 0; i < n; i++)
        cm_SendRelations(o->id, who[i]->id, -(50 - (int)(gm_Mix(p, who[i], 4) * -50.0f)), 1);
    ai_GypsyDeliveryText(o, n, who);
    return 0x44;
}
