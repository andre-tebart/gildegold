/* 0x483a60, the kind-9 master's trade step (0x47e990 dispatches here on ai_BTypes[type].kind == 9): in every
   third hour it buys slots / capacity, collects the building's kind 0x1d objects without a running job into
   ai_Carts, lets ai_BuildTradeOffers decide the trade and runs ai_MeisterTradeFernEinkauf or ...2 by the ratio
   of the first slot and entry; a watched building gets its slots copied
   @deep byte, Q-only handover: Q6 description added this session (kind-9 trade step: buys slots / capacity, collects the free kind 0x1d objects into ai_Carts, ai_BuildTradeOffers decides the trade, then ai_MeisterTradeFernEinkauf or ...2 by the slot/entry ratio). Q1 no evidenced name: 0x47e990 dispatches here on ai_BTypes[type].kind == 9 only, the body has no string / text id, and no caller names the profession. Q2 evidence gaps: ai_BuildTradeOffers (the trade decision builder, no comment or named reader), ai_Carts / ai_NumCarts (the object list and its count this function fills; the reader is ai_BuildTradeOffers only), AIOBJ.unk1c (documented as bld in notes/units/ai.md round 22 but the rename across units is undecided against the other unk1c fields), AIOBJ.unk20 / AISLOT.unk34 / AIENTRY.unk4c / AIBTYPE.unk247 (the slot and entry amounts, cap247 is only an unsigned alias), AIJOB.unkac (union id). */

void ai_Unk483a60(AICHAR *ch)
{
    AIOBJ *obj, *o;
    AIJOB *job;
    AIOBJ *it;
    float f;
    int i, ok, c;

    if ((ch->netId + ai_TimeT.hour) % 3 == 0) {
        if (!(ch->flags1b4 & 0x20)) {
            obj = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
            if (!obj)
                return;
            ai_BuySlots(obj, ch, 0x14);
            ai_BuyCapacity(obj, ch, 0x32);
            ok = 1;
            it = ai_Objects;
            for (i = 0; ok && i < 0x5008; it++, i++) {
                if (it->id && ai_Goods[it->id].type == 0x1d && it->unk1c == ch->bld->id && it->id == 0x136)
                    ok = 0;
            }
            ai_NumCarts = 0;
            for (o = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); o; o = gm_QueryNext()) {
                if (o->unk1c == ch->bld->id) {
                    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
                        if (job->unkac == o->objId)
                            break;
                    }
                    if (!job && ai_NumCarts < 0x10)
                        ai_Carts[ai_NumCarts++] = o;
                }
            }
            if (ai_NumCarts == 0) {
                ai_MeisterCollectTrans(ch);
                return;
            }
            ch->flags1b4 |= 0x20;
            ai_BuildTradeOffers(ch, obj, ok);
            if (ai_BTypes[ch->bld->type].unk247 == 1)
                f = ch->bld->unk39 * 1.5625e-05f;
            else if (ai_BTypes[ch->bld->type].unk247 == 2)
                f = ch->bld->unk39 * 7.8125e-06f;
            else if (ai_BTypes[ch->bld->type].unk247 == 3)
                f = ch->bld->unk39 * 3.90625e-06f;
            f = f * 0.5f + 0.25f;
            if (ai_NSlots != 0) {
                if (ai_NEntries != 0) {
                    if (ai_Slots[0].buyAmount * ai_Slots[0].saving * f > (1.0f - f) * ai_Entries[0].buyAmount * ai_Entries[0].margin)
                        ai_MeisterTradeFernEinkauf(ch, obj);
                    else
                        ai_MeisterTradeFernEinkauf2(ch, obj);
                } else
                    ai_MeisterTradeFernEinkauf(ch, obj);
            } else if (ai_NEntries != 0)
                ai_MeisterTradeFernEinkauf2(ch, obj);
            if (ai_Watch.bld == ch->bld) {
                for (c = 0; ai_Watch.nslots < 4; c++) {
                    if (c >= ai_NSlots)
                        break;
                    ai_Watch.slots[ai_Watch.nslots] = ai_Slots[c];
                    ai_Watch.nslots++;
                }
            }
        }
    } else
        ch->flags1b4 &= ~0x20;
}
