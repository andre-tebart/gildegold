/* 0x494ac0, rates a group of a leader and up to four members: the relation d (-2..2) of each member
   to the leader sends a message with a random strength (when send is set), the group's own value goes
   to unk2c. Returns 0, or -1 .. -6 for a bad group */

int ai_Unk494ac0(AIGRP *g, int send)
{
    int i, lv, base, d, v, w, p, r;

    if (!g)
        return -1;
    if (!g->leader)
        return -2;
    if (g->unk24 > 5)
        return -3;
    if (g->unk28 > 5)
        return -4;
    if (g->n == 0 || g->n > 4)
        return -5;
    r = 5 - g->unk28;
    p = 5 - g->unk24;
    lv = gm_Unk505e60S(g->leader->id, 0);
    base = lv - lv / 3 - lv / 4 + p + r;
    for (i = 0; i < g->n; i++) {
        if (!g->mem[i].ch)
            return -6;
        d = gm_Unk505e60S(g->mem[i].ch->id, 0);
        g->mem[i].d = max(min(base - d, 2), -2);
    }
    for (i = 0; i < g->n; i++) {
        switch (g->mem[i].d) {
        case -2:
            v = -0x10 - util_Random(0x10);
            break;
        case -1:
            v = -8 - util_Random(8);
            break;
        case 0:
            v = util_Random(8) + 8;
            break;
        case 1:
            v = util_Random(0x10) + 0x10;
            break;
        case 2:
            v = util_Random(0x10) + 0x18;
        }
        if (send)
            cm_SendRelations(g->leader->netId, g->mem[i].ch->netId, v, 1);
    }
    w = gm_GetWorth(g->leader->id);
    g->unk2c = (int)((float)(10 - g->unk28 - g->unk24) * w * g->n * 0.001f);
    if (send)
        cm_SendMoveGoods(-1, g->leader->netId, ai_OwnPlayer, g->unk2c);
    return 0;
}
