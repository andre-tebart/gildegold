/* 0x49f370, execute callback 0x33: the AI credit ("AI Kredit"): message 0x23 from the lender's building s1->unk04
   to the owner of s1->unk08, the offer s2 (0x14) attached; the money is paid; always returns 0 */

int ai_Unk49f370(G5CH *p, AIARG *s1, AIARG *s2)
{
    G8MSG m;
    AIBLD *a;
    AIBLD *b;
    int v;

    if (s1->kind != 4 || s2->kind != 0x14)
        return 0;
    G8_4FEA50((int *)&a, 0, 0, s1->unk04);
    if (a == 0)
        return 0;
    G8_4FEA50((int *)&b, 0, 0, s1->unk08);
    memset(&m, 0, sizeof(m));
    m.type = 0x23;
    m.to = ai_Chars[a->owner].netId;
    m.unk0c = a->id;
    m.time = ai_TimeT;
    m.unk36 = 0x12;
    *(int *)&m.unk58 = a->id;
    m.from = p->netId;
    m.unk60 = s2->unk04;
    m.unk64 = s2->unk08;
    m.unk6c = s2->unk10;
    v = s2->unk04;
    cm_BeginGroup("AI Kredit");
    cm_SendMoveGoods(p->netId, a->id, ai_OwnPlayer, s2->unk04);
    cm_SendLongMessage((AIMSG *)&m);
    if (b != 0) {
        cm_BeginFieldBlock((AIPER *)b, b->id);
        cm_AddFieldValues(4, 1, (char *)b - cm_FieldObject + 0x39, &v);
        cm_SendFieldDeltas();
    }
    cm_EndGroup("AI Kredit");
    return 0;
}
