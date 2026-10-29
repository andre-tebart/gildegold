/* 0x4864b0, builds the master's two ranked trade offer tables (ai_Slots = what to buy where, ai_Entries = what
   the workshop makes / sells, both sorted best first): both counts are cleared first; with f == 0 every shop
   (the buildings 0x47 and their 0x1db store object) contributes the goods its gm_FindGoods record knows, with the
   difference to the item price (gm_ItemPrice) as the ai_Slots sort key; every building of the kind 2 (its 0x2a
   store object) contributes to both tables, with the margin (goods price - item price) as the ai_Entries sort key;
   the running jobs (he_FindEvent 0xb) add their in goods against the entries' stock (AIENTRY.pending) and the
   slots' pending pieces and their out goods likewise; then each slot's amount = min (its capacity, the entry's
   other-store capacity, budget / 4 / price), each entry's amount = min (amount, the other store's capacity), and
   both tables are selection-sorted by amount * sort key, the result positions kept in AIENTRY.unk48 /
   AISLOT.unk30
   @deep B3 far draft, behaviour-side: 461 code / 52 slots / 22 regs / 10 fpu hunks and a 4-byte frame difference (sub esp,0x124 original vs 0x120 ours = one local too many, the unit's known variable-reuse lesson from 47e560, where the original kept one int for two purposes). Last-bytes pass meaning-class lead: the frame (merge two int locals that never overlap - candidates read: i / j / k / m / off / found; k and m are only used in the job loop, off only in the ai_Blds walk) - not tried, a rewrite this size needs a build per candidate. The rest of the diff is slots / allocation of the three table pointers. Q1 done this session: ai_BuildTradeOffers -> ai_BuildTradeOffers (role: it clears and rebuilds the two ranked trade offer tables ai_Slots / ai_Entries with their sort keys, amounts and positions; the only caller ai_Unk483a60 and the notes call it the trade decision builder). Q6 done (description above the definition). Q2 done: gm_FindGoods -> gm_FindGoods (unified with the f3gm.c definition, 21 refs in 7 files). Q2 gaps left: the AIENTRY / AISLOT amount and key fields (unk10, unk18, unk1c, unk20, unk24, unk28, unk30, unk34, unk38, unk44, unk48, unk4c, unk50) - their roles differ per struct and several already have union aliases (need1c / need20 / amount / capacity / pending / make), so the renames are a shared-header decision; AIBLD.unk65 (an index 1..3, type 'G' = bank). Q4 gaps: 3 raw accesses - the AISLOT swap through the AIENTRY scratch etmp (*(AISLOT *)&etmp), fixable with a typed AISLOT tmp but it changes the copy size, and *(int *)((char *)bld + 0x45) (a percent factor: price * that * 0.01, no evidenced AIBLD field name). */

void ai_BuildTradeOffers(AICHAR *ch, AIOBJ *store, int f)
{
    AIBLD *bld;
    AIOBJ *o, *o2;
    AIOBJ *it, *it2;
    int *q;
    AIJOB *job;
    void *p;
    AIRECIPE rec;
    AIENTRY etmp;
    int i, j, k, m, off, found;
    float price, diff;

    ai_NEntries = 0;
    ai_NSlots = 0;
    if (f == 0) {
        for (bld = gm_BuildQuery(1, 0, 0x47); bld; bld = gm_BuildNext()) {
            o = gm_Query(bld->objList, 2, 0, 0x1db, 6);
            if (o) {
                for (it = gm_Query(o->list, 1, 5); it; it = gm_QueryNext()) {
                    q = gm_FindGoods(bld->unk65, it->id);
                    if (q && q[1]) {
                        price = gm_GoodsPrice(it->id, (unsigned char)bld->unk65);
                        diff = gm_ItemPrice(it->id, 100) - price;
                        i = 0;
                        if (ai_NSlots > 0) {
                            for (; i < ai_NSlots; i++) {
                                if (ai_Slots[i].item == it->id) {
                                    if (i < ai_NSlots && diff < ai_Slots[i].saving)
                                        goto next1;
                                    break;
                                }
                            }
                            if (i > 0x7f)
                                goto next1;
                        }
                        ai_Slots[i].saving = diff;
                        ai_Slots[i].item = it->id;
                        ai_Slots[i].bld = bld;
                        ai_Slots[i].entry = -1;
                        ai_Slots[i].store = o;
                        ai_Slots[i].unk10 = 0;
                        ai_Slots[i].make = 0;
                        ai_Slots[i].sellAmount = 0;
                        ai_Slots[i].price = price;
                        ai_Slots[i].pending = 0;
                        ai_Slots[i].rank = 0x81;
                        ai_Slots[i].flags = 0x80;
                        ai_Slots[i].amount = gm_ListAmountById(store, it->id);
                        ai_Slots[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                        ai_Slots[i].buyAmount = 0;
                        ai_Slots[i].unk38 = gm_ListAmountById(o, it->id);
                        if (i == ai_NSlots)
                            ai_NSlots++;
                        it2 = gm_Query(o->list, 1, 5);
                        while (it2 && it2 != it)
                            it2 = gm_QueryNext();
                    }
                next1:;
                }
                o2 = gm_Query(bld->objList, 2, 0, 0x1db, 6);
                if (o2) {
                    for (it = gm_Query(o2->list, 1, 5); it; it = gm_QueryNext()) {
                        q = gm_FindGoods(bld->unk65, it->id);
                        if (q && q[1]) {
                            diff = gm_ItemPrice(it->id, 100);
                            price = gm_GoodsPrice(it->id, (unsigned char)bld->unk65);
                            i = 0;
                            if (ai_NEntries > 0) {
                                for (; i < ai_NEntries; i++) {
                                    if (ai_Entries[i].item == it->id) {
                                        if (i < ai_NEntries && price - diff < ai_Entries[i].margin)
                                            goto next2;
                                        break;
                                    }
                                }
                            }
                            if (ai_NEntries < 0x20) {
                                ai_Entries[i].item = it->id;
                                ai_Entries[i].slot[0] = -1;
                                ai_Entries[i].slot[1] = -1;
                                ai_Entries[i].slot[2] = -1;
                                ai_Entries[i].slot[3] = -1;
                                ai_Entries[i].margin = price - diff;
                                ai_Entries[i].bld = bld;
                                ai_Entries[i].store = o2;
                                ai_Entries[i].sellAmount = 0;
                                ai_Entries[i].cost = 0;
                                ai_Entries[i].pending = 0;
                                ai_Entries[i].rank = 0x21;
                                ai_Entries[i].flags = 0x100;
                                ai_Entries[i].amount = gm_ListAmountById(store, it->id);
                                ai_Entries[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                                ai_Entries[i].buyAmount = 0;
                                ai_Entries[i].unk50 = gm_FreeCapacity(o2, it->id, 1000000);
                                if (i == ai_NEntries)
                                    ai_NEntries++;
                                it2 = gm_Query(o->list, 1, 5);
                                while (it2 && it2 != it)
                                    it2 = gm_QueryNext();
                            }
                        }
                    next2:;
                    }
                }
            }
        }
    }
    off = 0;
    bld = ai_Blds;
    do {
        if (AI_KIND(bld->type) == 2) {
            o = gm_Query(bld->objList, 2, 0, 0x2a, 6);
            if (o) {
                for (it = gm_Query(o->list, 1, 5); it; it = gm_QueryNext()) {
                    q = gm_FindGoods(ai_OwnPlayer, it->id);
                    if (q && q[1]) {
                        price = gm_GoodsPrice(it->id, ai_OwnPlayer);
                        price = price * *(int *)((char *)bld + 0x45) * 0.01;
                        diff = gm_ItemPrice(it->id, 100) - price;
                        i = 0;
                        if (ai_NSlots > 0) {
                            for (; i < ai_NSlots; i++) {
                                if (ai_Slots[i].item == it->id) {
                                    if (i < ai_NSlots && diff < ai_Slots[i].saving)
                                        goto next3;
                                    break;
                                }
                            }
                            if (i > 0x7f)
                                goto next3;
                        }
                        ai_Slots[i].saving = diff;
                        ai_Slots[i].bld = bld;
                        ai_Slots[i].item = it->id;
                        ai_Slots[i].entry = -1;
                        ai_Slots[i].store = o;
                        ai_Slots[i].unk10 = 0;
                        ai_Slots[i].make = 0;
                        ai_Slots[i].sellAmount = 0;
                        ai_Slots[i].price = price;
                        ai_Slots[i].pending = 0;
                        ai_Slots[i].rank = 0x81;
                        ai_Slots[i].flags = 0;
                        ai_Slots[i].amount = gm_ListAmountById(store, it->id);
                        ai_Slots[i].capacity = gm_FreeCapacity(store, it->id, 1000000);
                        ai_Slots[i].buyAmount = 0;
                        ai_Slots[i].unk38 = gm_ListAmountById(o, it->id);
                        if (i == ai_NSlots)
                            ai_NSlots++;
                        it2 = gm_Query(o->list, 1, 5);
                        while (it2 && it2 != it)
                            it2 = gm_QueryNext();
                    }
                next3:;
                }
            }
        }
        off += 0xa9;
        bld = (AIBLD *)((char *)ai_Blds + off);
    } while (off < 0xa900);
    for (i = 0; i < ai_NSlots; i++) {
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Slots[i].item == ai_Entries[j].item)
                break;
        }
        if (j < ai_NEntries)
            continue;
        p = gm_FindMarketObject(ai_Slots[i].item);
        if (p == 0) {
            ai_Slots[i].saving = -1000000.0f;
        } else {
            price = gm_ItemPrice(ai_Slots[i].item, 100);
            diff = gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer);
            if (ai_NEntries < 0x20) {
                ai_Entries[ai_NEntries].item = ai_Slots[i].item;
                ai_Entries[ai_NEntries].slot[0] = -1;
                ai_Entries[ai_NEntries].slot[1] = -1;
                ai_Entries[ai_NEntries].slot[2] = -1;
                ai_Entries[ai_NEntries].slot[3] = -1;
                ai_Entries[ai_NEntries].bld = ai_MarketBuilding;
                ai_Entries[ai_NEntries].store = p;
                ai_Entries[ai_NEntries].sellAmount = 0;
                ai_Entries[ai_NEntries].cost = 0;
                ai_Entries[ai_NEntries].margin = diff - price;
                ai_Entries[ai_NEntries].pending = 0;
                ai_Entries[ai_NEntries].rank = 0x21;
                ai_Entries[ai_NEntries].flags = 0;
                ai_Entries[ai_NEntries].amount = gm_ListAmountById(store, ai_Slots[i].item);
                ai_Entries[ai_NEntries].capacity = gm_FreeCapacity(store, ai_Slots[i].item, 1000000);
                ai_Entries[ai_NEntries].buyAmount = 0;
                ai_Entries[ai_NEntries].unk50 = gm_FreeCapacity(p, ai_Slots[i].item, 1000000);
                ai_NEntries++;
            }
        }
    }
    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (p) {
            gm_StallGoodsList(p, &rec);
            for (i = 0; i < rec.n; i++) {
                if (rec.item[i]) {
                    found = 0;
                    for (k = 0; k < 3 && !found; k++) {
                        if (job->in[k] == rec.item[i])
                            found = 1;
                    }
                    if (!found) {
                        for (m = 0; m < ai_NEntries; m++) {
                            if (rec.item[i] == ai_Entries[m].item)
                                ai_Entries[m].pending += rec.amount[i];
                        }
                        for (m = 0; m < ai_NSlots; m++) {
                            if (rec.item[i] == ai_Slots[m].item)
                                ai_Slots[m].pending += rec.amount[i];
                        }
                    }
                }
            }
            for (k = 0; k < 3; k++) {
                if (job->out[k]) {
                    for (m = 0; m < ai_NEntries; m++) {
                        if (job->out[k] == ai_Entries[m].item)
                            ai_Entries[m].pending += job->amount[k];
                    }
                    for (m = 0; m < ai_NSlots; m++) {
                        if (job->out[k] == ai_Slots[m].item)
                            ai_Slots[m].pending += job->amount[k];
                    }
                }
            }
        }
    }
    for (i = 0; i < ai_NSlots; i++) {
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Slots[i].item == ai_Entries[j].item) {
                k = ai_Slots[i].capacity < ai_Entries[j].unk50 ? ai_Slots[i].capacity : ai_Entries[j].unk50;
                ai_Slots[i].buyAmount = k;
                ai_Slots[i].buyAmount = (int)min((float)ai_Slots[i].buyAmount, (float)(ch->budget / 4) / ai_Slots[i].price);
            }
        }
    }
    for (j = 0; j < ai_NEntries; j++)
        ai_Entries[j].buyAmount = ai_Entries[j].amount < ai_Entries[j].unk50 ? ai_Entries[j].amount : ai_Entries[j].unk50;
    for (i = 0; i < ai_NSlots; i++) {
        for (j = i; j < ai_NSlots; j++) {
            if ((float)ai_Slots[i].buyAmount * ai_Slots[i].saving < (float)ai_Slots[j].buyAmount * ai_Slots[j].saving) {
                *(AISLOT *)&etmp = ai_Slots[i];
                ai_Slots[i] = ai_Slots[j];
                ai_Slots[j] = *(AISLOT *)&etmp;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        for (j = i; j < ai_NEntries; j++) {
            if ((float)ai_Entries[i].buyAmount * ai_Entries[i].margin < (float)ai_Entries[j].buyAmount * ai_Entries[j].margin) {
                etmp = ai_Entries[i];
                ai_Entries[i] = ai_Entries[j];
                ai_Entries[j] = etmp;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_Entries[i].rank = i;
    for (i = 0; i < ai_NSlots; i++)
        ai_Slots[i].rank = i;
}
