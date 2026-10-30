/* 0x4854f0 @deep B2/B3 behaviour only (no Q failure: name, Q2, Q6 all pass): 176 code hunks, 172 asm diff lines, 3161 of 3248 bytes differ. Read against tools/disasm.py 0x4854f0 and tools/listing.py: statements, constants, callees and field offsets match - the diff is the entry-init block. The original hoists one pointer PER FIELD of the new ai_Entries entry into stack slots ([esp+0x10] item, 0x14 flags, 0x18 unk44, 0x24 pending, 0x28 unk48, 0x2c amount, 0x30 capacity) and bumps each by 0x58 per iteration (mov [esi],0 / [edi],0 / [ecx],abs / [ebx],0x21 / [ebx],0x24 at 0x48587f..0x4858c3), i.e. VC6s per-field loop-invariant pointer form; ours keeps the index and the absolute addressing. Last-bytes pass, measured one edit one build each (both reverted, source unchanged): (1) the memset(ai_Entries[n].slot, 0xff, 16) written as four slot[i] = -1 stores: differing bytes 2361 -> 2334 but asm lines 172 -> 244 and code hunks 176 -> 240, unitdiff WORSE; (2) the 0x24 block (ai_Slots[j].flags |= 0x24 + the entry loop) is present in the source in the same statement order - its hunks are the ebx slot-pointer caching and the block order, not a missing statement. What is left is that allocation / scheduling = match-deep. Q2 note: AISLOT.unk34 / unk38 (0x34 / 0x38) have no evidenced role yet; they are not touched here. */

void ai_UpdateProduction(AICHAR *ch, AIOBJ *a, AIOBJ *b)
{
    AIOBJ *it;
    AIOBJ *o;
    AIJOB *job;
    void *p;
    AIRECIPE rec;
    AIENTRY tmp;
    unsigned short g;
    char kind;
    int i, j, k, m, found;

    ai_NEntries = 0;
    for (it = gm_Query(a->list, 1, 5); it; it = gm_QueryNext()) {
        if (it->count > 1 && ai_NEntries < 32) {
            ai_Entries[ai_NEntries].item = it->id;
            memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
            ai_Entries[ai_NEntries].unk1c = 0;
            ai_Entries[ai_NEntries].unk20 = 0;
            ai_Entries[ai_NEntries].cost = -1e10f;
            ai_Entries[ai_NEntries].pending = 0;
            ai_Entries[ai_NEntries].amount = 0;
            ai_Entries[ai_NEntries].capacity = 0;
            ai_Entries[ai_NEntries].sellAmount = 0;
            ai_Entries[ai_NEntries].rank = 0x21;
            ai_Entries[ai_NEntries].flags = 0;
            ai_NEntries++;
        }
    }
    ai_NSlots = 0;
    for (i = 0; i < ai_NEntries; i++) {
        for (k = 0; k < 4; k++) {
            if (ai_GoodsT[ai_Entries[i].item].in[k]) {
                for (j = 0; j < ai_NSlots; j++) {
                    if (ai_Slots[j].item == ai_GoodsT[ai_Entries[i].item].in[k]) {
                        ai_Entries[i].slot[k] = j;
                        break;
                    }
                }
                if (ai_Entries[i].slot[k] == -1) {
                    if (ai_NSlots >= 128)
                        return;
                    ai_Slots[ai_NSlots].item = ai_GoodsT[ai_Entries[i].item].in[k];
                    ai_Slots[ai_NSlots].entry = -1;
                    ai_Slots[ai_NSlots].bld = 0;
                    ai_Slots[ai_NSlots].unk10 = 0;
                    ai_Slots[ai_NSlots].make = 0;
                    ai_Slots[ai_NSlots].sellAmount = 0;
                    ai_Slots[ai_NSlots].price = -1e10f;
                    ai_Slots[ai_NSlots].pending = 0;
                    ai_Entries[i].slot[k] = ai_NSlots;
                    ai_Slots[ai_NSlots].amount = 0;
                    ai_Slots[ai_NSlots].rank = 0x21;
                    ai_Slots[ai_NSlots].flags = 0;
                    ai_NSlots++;
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 14 || kind == 8 || kind == 32) {
        for (j = 0; j < ai_NSlots; j++) {
            if (ai_Slots[j].item == 0x1c7 || ai_Slots[j].item == 0x1c4 || ai_Slots[j].item == 0x1c5 || ai_Slots[j].item == 0x1c6 || ai_Slots[j].item == 0x2af || ai_Slots[j].item == 0x2b0) {
                ai_Slots[j].flags |= 0x24;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x24;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].unk1c = 0;
                    ai_Entries[ai_NEntries].unk20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x24;
                    ai_NEntries++;
                }
                continue;
            }
            if (ai_Slots[j].item == 0x1c1 || ai_Slots[j].item == 0x1c2 || ai_Slots[j].item == 0x1c3) {
                ai_Slots[j].flags |= 0x204;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x204;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].unk1c = 0;
                    ai_Entries[ai_NEntries].unk20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x204;
                    ai_NEntries++;
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 29 || kind == 31) {
        for (j = 0; j < ai_NSlots; j++) {
            if (ai_Slots[j].item == 0x225 || ai_Slots[j].item == 0x226 || ai_Slots[j].item == 0x227 || ai_Slots[j].item == 0x293
                || ai_Slots[j].item == 0x294 || ai_Slots[j].item == 0x295) {
                ai_Slots[j].flags |= 0x404;
                for (i = 0; i < ai_NEntries; i++) {
                    if (ai_Entries[i].item == ai_Slots[j].item) {
                        ai_Entries[i].flags |= 0x404;
                        break;
                    }
                }
                if (i >= ai_NEntries) {
                    ai_Entries[ai_NEntries].item = ai_Slots[j].item;
                    memset(ai_Entries[ai_NEntries].slot, 0xff, sizeof(ai_Entries[ai_NEntries].slot));
                    ai_Entries[ai_NEntries].unk1c = 0;
                    ai_Entries[ai_NEntries].unk20 = 0;
                    ai_Entries[ai_NEntries].cost = -1e10f;
                    ai_Entries[ai_NEntries].pending = 0;
                    ai_Entries[ai_NEntries].amount = 0;
                    ai_Entries[ai_NEntries].capacity = 0;
                    ai_Entries[ai_NEntries].sellAmount = 0;
                    ai_Entries[ai_NEntries].rank = 0x21;
                    ai_Entries[ai_NEntries].flags = 0x404;
                    ai_NEntries++;
                }
            }
        }
    }
    for (j = 0; j < ai_NSlots; j++) {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Slots[j].item == ai_Entries[i].item) {
                ai_Slots[j].entry = i;
                break;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        o = gm_Query(b->list, 1, 0, ai_Entries[i].item);
        if (!o)
            ai_Entries[i].amount = 0;
        else
            ai_Entries[i].amount = gm_ListAmount(b, o);
        ai_Entries[i].capacity = gm_FreeCapacity(b, ai_Entries[i].item, 1000000);
    }
    for (j = 0; j < ai_NSlots; j++) {
        o = gm_Query(b->list, 1, 0, ai_Slots[j].item);
        if (!o) {
            ai_Slots[j].amount = 0;
        } else {
            ai_Slots[j].amount = gm_ListAmount(b, o);
            if (!ai_Slots[j].amount)
                cm_SendRemoveGoodsI(b->objId, ai_Slots[j].item);
        }
        ai_Slots[j].capacity = gm_FreeCapacity(b, ai_Slots[j].item, 1000000);
    }
    for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (!p)
            continue;
        gm_StallGoodsList(p, &rec);
        for (k = 0; k < rec.n; k++) {
            g = rec.item[k];
            if (!g)
                continue;
            found = 0;
            for (m = 0; !found && m < 3; m++) {
                if (job->in[m] == g)
                    found = 1;
            }
            if (!found) {
                for (i = 0; i < ai_NEntries; i++) {
                    if (g == ai_Entries[i].item)
                        ai_Entries[i].pending += rec.amount[k];
                }
                for (j = 0; j < ai_NSlots; j++) {
                    if (g == ai_Slots[j].item)
                        ai_Slots[j].pending += rec.amount[k];
                }
            }
        }
        for (m = 0; m < 3; m++) {
            if (job->out[m]) {
                for (i = 0; i < ai_NEntries; i++) {
                    if (job->out[m] == ai_Entries[i].item)
                        ai_Entries[i].pending += job->amount[m];
                }
                for (j = 0; j < ai_NSlots; j++) {
                    if (job->out[m] == ai_Slots[j].item)
                        ai_Slots[j].pending += job->amount[m];
                }
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 8 || kind == 14) {
        for (job = he_FindEvent(2, 0, 0x14, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.hi)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    if (ai_BTypesT[ch->bld->type].kind == 8) {
        for (job = he_FindEvent(2, 0, 0x15, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.hi)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    kind = ai_BTypesT[ch->bld->type].kind;
    if (kind == 29 || kind == 31) {
        for (job = he_FindEvent(2, 0, 0x8c, 3, ch->bld->id); job; job = he_NextEvent()) {
            for (j = 0; j < ai_NSlots; j++) {
                if (ai_Slots[j].item == job->u.w.lo)
                    ai_Slots[j].pending += (gm_BuildingSkill(ch->bld, 11, 1) * 0.01f + 1.0f) * 7.5f;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_CalcEntryCost(&ai_Entries[i]);
    for (i = 0; i < ai_NEntries; i++) {
        for (j = i; j < ai_NEntries; j++) {
            if (ai_Entries[i].margin < ai_Entries[j].margin) {
                tmp = ai_Entries[i];
                ai_Entries[i] = ai_Entries[j];
                ai_Entries[j] = tmp;
            }
        }
    }
    for (j = 0; j < ai_NSlots; j++) {
        if (ai_Slots[j].entry != -1) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Slots[j].item == ai_Entries[i].item) {
                    ai_Slots[j].flags |= 0x40;
                    ai_Entries[i].flags |= 0x40;
                    ai_Slots[j].entry = i;
                }
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++)
        ai_Entries[i].rank = i;
}
