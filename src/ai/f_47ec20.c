/* 0x47ec20, the master's open slots: how much of an item to make */

void ai_PlanSlots(AICHAR *ch, AIOBJ *b)
{
    int i, n, m;
    float f;
    AISLOT *s;
    AIOBJ *x;
    AIOBJ *o;

    for (i = 0; i < ai_NSlots; i++) {
        s = &ai_Slots[i];
        if (!(s->flags & 0x20) && !(s->flags & 0x200) && s->pending == 0 && s->amount < gm_ListStock(b, s->item) / 4) {
            f = gm_GoodsPrice(s->item, ai_OwnPlayer) / gm_ItemPrice(s->item, 100);
            if (f < 0.8f) {
                n = s->capacity;
                s->bld = 0;
                m = (int)((1.0f - f) * n);
                s->make = m;
                if (m >= n)
                    m = n;
                s->make = m;
                x = gm_FindMarketObject(s->item);
                if (x && (o = gm_Query(x->list, 1, 0, s->item)) != 0) {
                    if (gm_ListAmount(x, o) - (int)(gm_ListStock(x, o->id) * (unsigned)o->cnt >> 2) >= 5)
                        s->bld = ai_MarketBuilding;
                }
            }
        }
    }
}
