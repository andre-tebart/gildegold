/* 0x487040, the production cost of an entry (a good the workshop makes), once per entry (the guard: only while
   AIENTRY.unk24 is still the -1e10 default): per needed good (the four AIENTRY.slot indices into ai_Slots) the
   slot price (gm_GoodsPrice, 90% for a good the workshop produces itself - the slot has an entry) times the need
   amount, summed and divided by the good's batch = the material cost in AIENTRY.unk28; plus the object value
   (gm_ObjectValue) in AIENTRY.unk2c = the cost AIENTRY.unk24; the sale price (gm_GoodsPrice) in AIENTRY.unk30;
   the margin per unit (price - cost) / AIGOOD.unk22 in AIENTRY.margin
   @deep NAMES item (byte already, readability fails only Q2): the single remaining identifier is AIGOOD.unk22, the divisor of the margin formula (e->margin = (e->price - e->cost) / ai_Goods[e->item].unk22). Read: it is the dword at AIGOOD +0x22 (the tail of the 0x25-byte name field, ai.h) and this is its ONLY reader in src/ - no second use, no name anywhere, so no evidenced name exists. Q1 done this session (ai_CalcEntryCost -> ai_CalcEntryCost: it computes the material cost, object value, total cost, sale price and margin of an entry), Q6 done (description above the definition), Q2 done for the rest: gm_ObjectValue -> gm_ObjectValue, and the field renames AIENTRY.unk24 -> cost, unk28 -> materialCost, unk2c -> objectValue, unk30 -> price, unk34 -> margin, unk48 -> rank, AISLOT.unk1c -> price, unk30 -> rank, unk34 -> buyAmount (project-wide, byte-neutral). */

void ai_CalcEntryCost(AIENTRY *e0)
{
    AIENTRY *e = (AIENTRY *)e0;
    AISLOT *s;
    int k;

    if (!(e->cost > -1e10)) {
        e->materialCost = 0;
        for (k = 0; k < 4; k++) {
            if (e->slot[k] >= 0) {
                s = &ai_Slots[e->slot[k]];
                if (!(s->price > -1e10)) {
                    if (s->entry != -1)
                        s->price = gm_GoodsPrice(s->item, ai_OwnPlayer) * 0.9;
                    else
                        s->price = gm_GoodsPrice(s->item, ai_OwnPlayer);
                }
                e->materialCost = ai_GoodsT[e->item].need[k] * s->price + e->materialCost;
            }
        }
        e->materialCost = e->materialCost / ai_GoodsT[e->item].batch;
        e->objectValue = gm_ObjectValue(e->item);
        e->cost = e->objectValue + e->materialCost;
        e->price = gm_GoodsPrice(e->item, ai_OwnPlayer);
        e->margin = (e->price - e->cost) / ai_GoodsT[e->item].unk22;
    }
}
