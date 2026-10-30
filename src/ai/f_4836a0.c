/* 0x4836a0, the bank master's money balance: moves money between the four banks / the town @deep byte, Q-only handover: Q2 only, after this session's readability work (Q1 named by role - the bank master of 0x47e990's kind-5 dispatch, its comment already described it; ai_Unk508660 -> gm_SumSlotAmounts with the f3gm.c definition). Q2 evidence gaps left, all three read in the callers, callees and data: cm_FieldObject (the object the cm_AddFieldDelta addresses are formed against - 'the object' in notes/units/cm.md, a char * in ai.md, an int in ai.h; no source names it), AIPOS.unk18 (the table record 0xe field the building's 0x65 value is steered towards within +-2, clamped to >= 4; no reader names it), AIBLD.unk65 (the value moved by +-1 per event and written as field delta 0x65; no source names it). */

void ai_CalcMeisterBank(AICHAR *ch)
{
    AIPOS p;
    AIJOB *job;
    AIBLD *b;
    int i, n, v, d, m, sum;

    if (ch->flags1c8 & 8)
        return;
    if (!gv_GetTableRecord(0xe, &p))
        return;
    n = 0;
    for (job = he_FindEvent(2, 0, 0x23, 3, ch->bld->id); job; job = he_NextEvent())
        n++;
    if (abs(ch->bld->unk65 - p.unk18) > 3) {
        if (ch->bld->unk65 > p.unk18)
            v = ch->bld->unk65 - 1;
        else
            v = ch->bld->unk65 + 1;
        cm_BeginFieldBlock(ch->bld, ch->bld->id);
        cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
        cm_SendFieldDeltas();
    } else if (n == 0) {
        if (ch->bld->unk65 > p.unk18 - 2) {
            v = ch->bld->unk65 - util_RandomU(3) - 1;
            if (p.unk18 - 2 > v)
                v = p.unk18 - 2;
            if (v < 4)
                v = 4;
            cm_BeginFieldBlock(ch->bld, ch->bld->id);
            cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
            cm_SendFieldDeltas();
        }
    } else if (n > 3 && ch->bld->unk65 < p.unk18) {
        v = util_RandomU(3) + 1 + ch->bld->unk65;
        if (v >= p.unk18 + 3)
            v = p.unk18 + 3;
        cm_BeginFieldBlock(ch->bld, ch->bld->id);
        cm_AddFieldDelta(4, 1, (short)ch->bld - (short)cm_FieldObject + 0x65, &v);
        cm_SendFieldDeltas();
    }
    sum = 0;
    for (i = 1; i < 4; i++) {
        v = gm_SumSlotAmounts(ch->bld->objList, i);
        m = ch->budget;
        if (v < m * 3 / 20) {
            d = m * 5 / 20 - v;
            sum += d;
            cm_BeginGroup("Bankmeister");
            cm_SendMoveGoods(ch->bld->id, -1, i, d);
            cm_SendMoveGoods(-1, ai_CharsT[ch->bld->owner].netId, ai_OwnPlayer, (int)(d * 1.03));
            cm_EndGroup("Bankmeister");
        } else if (v > m * 6 / 20) {
            d = v - m * 4 / 20;
            sum -= d;
            cm_BeginGroup("Bankmeister");
            cm_SendMoveGoods(ai_CharsT[ch->bld->owner].netId, -1, ai_OwnPlayer, d);
            cm_SendMoveGoods(-1, ch->bld->id, i, (int)(d * 1.03));
            cm_EndGroup("Bankmeister");
        }
    }
    v = gm_SumSlotAmounts(ch->bld->objList, ai_OwnPlayer);
    m = ch->budget;
    if (!(v < m * 1.5) || m - sum <= 0) {
        b = ch->bld;
        if (ai_CharsT[b->owner].kind != 6 && ai_CharsT[b->owner].kind != 7 && v > m * 2)
            cm_SendMoveGoods(ai_CharsT[b->owner].netId, b->id, ai_OwnPlayer, (int)(v - m * 2.25));
    } else {
        b = ch->bld;
        cm_SendMoveGoods(b->id, ai_CharsT[b->owner].netId, ai_OwnPlayer, (int)((m - sum) * 0.8));
    }
    ch->flags1c8 |= 8;
}
