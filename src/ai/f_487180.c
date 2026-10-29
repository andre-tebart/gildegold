/* 0x487180, whether the workshop can get the materials of entry e, 1 when every needed good is covered: per
   slot of the entry, when the slot's amount is below the need, f == 0 requires it in the workshop's own store
   (the slot capacity 0 counts as covered, so does a recursive check of the producing entry); with f != 0 and a
   good the workshop does not produce itself (the pending pieces 0 and none of the special items 0x1c1 .. 0x1c7 /
   0x2af / 0x2b0) it re-collects the offers of the good (gm_CollectOffers) and takes the first foreign offer (the
   master's own building, the kind-2 warehouses and offers without a goods object skipped) whose supply covers the
   need and whose buy amount - half the entry capacity, at least 1, above half the slot capacity minus the pending
   pieces, below the slot capacity - pending - 1 and the budget / price - is at least the need
   @deep B3 behaviour draft: 43 code hunks + 9 fill, all after instruction 88 - a stack-slot / block-order difference (the original keeps the row pointer of gm_Offers[j] in a slot and initializes the loop counter with a constant 0 store, ours zeroes through eax and keeps the row base in ebp: [esp+0x14]/[esp+0x18] roles swapped at instruction 179), no meaning-class lead left: statements, constants (the special item list 0x1c1 .. 0x1c7 / 0x2af / 0x2b0), callees and field offsets all match and the Q rules are clean after this session's work. Q2 done: gm_CollectOffers -> gm_CollectOffers (unified with the f3gm.c definition), gm_Offers -> gm_Offers, gm_NOffers -> gm_NOffers (the f3gm.c evidence: the offer table of 6-int rows and its count), AISTORE.unk08 -> goodsObj (the goods object of an offer), AISLOT.unk28 -> the amount member (the union alias removed). Q6 done (description above the definition). */

int ai_HasMaterials(AICHAR *ch, AIENTRY *e, int f)
{
    AISLOT *s;
    AISTORE *row;
    int k, j, found, cnt, v, m;

    for (k = 0; k < 4; k++) {
        if (e->slot[k] >= 0) {
            s = &ai_Slots[e->slot[k]];
            if (s->amount < ai_Goods[e->item].need[k]) {
                if (f != 0 || s->capacity == 0)
                    return 0;
                if (s->pending == 0 && s->item != 0x1c7 && s->item != 0x1c4 && s->item != 0x1c5 && s->item != 0x1c6 &&
                    s->item != 0x2af && s->item != 0x2b0 && s->item != 0x1c1 && s->item != 0x1c2 &&
                    s->item != 0x1c3) {
                    if (s->entry != -1) {
                        found = ai_HasMaterials(ch, &ai_Entries[s->entry], 0);
                    } else {
                        gm_CollectOffers(s->item);
                        j = found = 0;
                        do {
                            if (j >= gm_NOffers)
                                break;
                            row = &gm_Offers[j];
                            if (row->bld != ch->bld && AI_KIND(row->bld->type) != 2 && row->goodsObj != 0) {
                                cnt = row->count;
                                if (cnt >= ai_Goods[e->item].need[k]) {
                                    if (row->bld->owner != ch->bld->owner) {
                                        m = e->capacity >> 1;
                                        if (m < 1)
                                            m = 1;
                                        v = ai_Goods[e->item].need[j] * m - s->amount - s->pending;
                                        v = max(v, (s->capacity >> 1) - s->pending);
                                        v = min(s->capacity - s->pending - 1, v);
                                        v = (int)min((float)ch->budget / s->price, (float)v);
                                        v = min(cnt, v);
                                        if (v >= ai_Goods[e->item].need[j])
                                            found = 1;
                                    } else {
                                        found = 1;
                                    }
                                }
                            }
                            j++;
                        } while (!found);
                    }
                    if (!found)
                        return 0;
                }
            }
        }
    }
    return 1;
}
