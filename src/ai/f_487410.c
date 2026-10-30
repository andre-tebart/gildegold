/* 0x487410, starts the production the workshop entry e needs, returns 1: per slot of the entry the slot gets the
   entry's rank and flag 2 (selected); a special item (a good the workshop makes itself, 0x1c1 .. 0x1c7 / 0x2af /
   0x2b0) gets flag 0x10 when its entry's store has room (4 x pending + amount below the capacity) and the slot
   holds / expects less than 4 x the needed amount; a slot assigned to an entry whose store has room starts that
   entry's production (recursively) and flags the entry 8 when the store holds a whole batch; a slot without an
   entry whose pending + stock is below 4 x the need collects the goods' offers (gm_CollectOffers) and scans
   them: an offer of a foreign building (not the master's, not a kind-2 warehouse) with a goods object that
   supplies at least the need becomes the slot's bld, an own one fills the slot's make to the capacity, the else
   buy amount - half the entry capacity at least 1, at least half the slot capacity minus the pending, below the
   capacity - pending - 1, at most the budget / price and the offer's count - goes into make when it covers the
   need
   @deep behaviour draft, 2 code hunks only, both the block layout of the item chain: the original keeps the special-item block inline after the 15 je (0x4874cb..0x48751b) and puts the entry!=-1 and the offer-scan blocks OUT of line after the loop epilogue/ret (0x48754a, 0x4875e7), ours emits the entry block inline and the special block at $L4532 before the loop tail; 8 slots hunks are the k / j / row slot choice (k 0x18 vs 0x10, j 0x10 vs 0x18, row 0x14 vs 0x1c). Read whole against tools/disasm.py 0x487410 0x370, Ghidra build/ghidra/487410.c and the VC6 listing; statements, constants, callees, field offsets and the item list order all match. Last-bytes pass, one edit one build each, all measured byte-identical (183 asm diff lines, 129 code / 68 slots hunks unchanged): (1) positive == || chain with the special block as then-branch and the entry/offer blocks as else-if, (2) nested else (if (special) {} else { if (entry != -1) .. else if (..) }), (3) special block + continue then a separate if / else if. spell.py branch / dup-tail / reload / slots entries: dup-tail 4877ac is the padding + next function in the window; slots swap of the k / j declaration order tried, identical. Readability done: Q2 AIENTRY.unk38 -> pending, Q6 description. Left: allocation / scheduling / block ordering and the stack slot choice = match-deep */

int ai_StartProduction(AICHAR *ch, AIENTRY *e)
{
    AISLOT *s;
    AIENTRY *e2;
    AISTORE *row;
    int k, j, cnt, v, m;

    for (k = 0; k < 4; k++) {
        if (e->slot[k] >= 0) {
            s = &ai_Slots[e->slot[k]];
            s->rank = s->rank < e->rank ? s->rank : e->rank;
            s->flags |= 2;
            if (s->item != 0x1c7 && s->item != 0x1c4 && s->item != 0x1c5 && s->item != 0x1c6 &&
                s->item != 0x2af && s->item != 0x2b0 && s->item != 0x1c1 && s->item != 0x1c2 &&
                s->item != 0x1c3 && s->item != 0x225 && s->item != 0x226 && s->item != 0x227 &&
                s->item != 0x293 && s->item != 0x294 && s->item != 0x295) {
                if (s->entry != -1) {
                    ai_Entries[s->entry].rank = ai_Entries[s->entry].rank < e->rank ? ai_Entries[s->entry].rank : e->rank;
                    ai_Entries[s->entry].flags |= 2;
                    if ((ai_Entries[s->entry].pending + ai_Entries[s->entry].amount) * 4 < ai_Entries[s->entry].capacity) {
                        if (ai_Entries[s->entry].capacity > ai_GoodsT[ai_Entries[s->entry].item].batch)
                            ai_Entries[s->entry].flags |= 8;
                        ai_StartProduction(ch, &ai_Entries[s->entry]);
                    }
                } else if (s->pending + s->amount < ai_GoodsT[e->item].need[k] * 4) {
                    gm_CollectOffers(s->item);
                    j = 0;
                    while (1) {
                        if (j >= gm_NOffers)
                            break;
                        row = &gm_Offers[j];
                        if (row->bld != ch->bld && AI_KIND(row->bld->type) != 2 && row->goodsObj != 0) {
                            cnt = row->count;
                            if (cnt >= ai_GoodsT[e->item].need[k]) {
                                s->bld = row->bld;
                                if (row->bld->owner == ch->bld->owner) {
                                    s->make = s->capacity;
                                    j++;
                                    continue;
                                }
                                m = e->capacity >> 1;
                                if (m < 1)
                                    m = 1;
                                v = ai_GoodsT[e->item].need[k] * m - s->pending - s->amount;
                                v = max(v, (s->capacity >> 1) - s->pending);
                                v = min(s->capacity - s->pending - 1, v);
                                v = (int)min((float)ch->budget / s->price, (float)v);
                                v = min(cnt, v);
                                if (v >= ai_GoodsT[e->item].need[k])
                                    s->make = max(s->make, v);
                            }
                        }
                        j++;
                    }
                }
            } else {
                if ((ai_Entries[s->entry].pending + ai_Entries[s->entry].amount) * 4 < ai_Entries[s->entry].capacity &&
                    s->amount + s->pending < ai_GoodsT[e->item].need[k] * 4)
                    s->flags |= 0x10;
            }
        }
    }
    return 1;
}
