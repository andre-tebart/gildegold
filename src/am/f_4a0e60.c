/* 0x4a0e60, execute callback 0x39: pays the credit (s1 kind 1, s2 kind 0x12) into the treasury s1->unk04, or
   upgrades the guild hall ("upgr_ai_zunfthaus", s1 kind 4, s2 kind 1) for 30 % of the price of type s1->unk04 */

int ai_Unk4a0e60(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    unsigned char *o;
    AIBLD *b;

    if (s1->kind == 1 && s2->kind == 0x12 && s2->unk08 > 0) {
        G8_4FEA50(0, (int)&o, 0, s1->unk04);
        if (o == 0)
            return 0;
        cm_SendMoveGoods(*(int *)(o + 2), p->netId, ai_OwnPlayer, s2->unk08);
        return 0x39;
    }
    if (s1->kind != 4 || s2->kind != 1)
        return 0;
    G8_4FEA50((int *)&b, 0, 0, s1->unk0c);
    if (b == 0)
        return 0;
    G8_4FEA50(0, (int)&o, 0, s2->unk04);
    if (o == 0)
        return 0;
    memset(&m, 0, sizeof(m));
    m.type = 0x1e;
    m.unk0c = s1->unk0c;
    m.to = p->netId;
    m.unk36 = 1;
    m.time = ai_TimeT;
    *(int *)&m.unk58 = s1->unk0c;
    cm_BeginGroup("upgr_ai_zunfthaus");
    cm_SendMoveGoods(-1, *(int *)(o + 2), ai_OwnPlayer, (int)(gm_BuildingKindValue(s1->unk04) * 0.3f));
    cm_SendLongMessage((AIMSG *)&m);
    cm_EndGroup("upgr_ai_zunfthaus");
    return 0x39;
}
