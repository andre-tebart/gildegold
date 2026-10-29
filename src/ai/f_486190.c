/* 0x486190, the workshop's slot table: the goods of two fixed lists, and what the running jobs bring in and out @deep B3 far draft (426 code / 7 fill hunks, 554 of 736 bytes differ). The first blocks were read against tools/disasm.py 0x486190 and the round-21 note: statements, constants, callees and field offsets match; what differs is exactly the allocation the note recorded - the original keeps ai_NSlots in ecx, the zero in esi and the two goods-list pointers in ebp / edi, ours the count in esi and the zero in edx - pure allocation, no meaning-class lead (the source already spells the item / price / amount stores as the original does at the same field offsets). Q2 done this session: gm_FindObjectInst -> gm_FindObjectInst (unified with the f3gm.c definition, 38 refs), gm_StallGoodsList -> gm_StallGoodsList (the f3gm.c definition: the list of the stall object 0x1dd of the object c), gm_ListAmountById -> gm_ListAmountById (the f3gm.c definition: the amount of the object id in the list l, beside gm_ListAmount) - all byte-neutral, 14 files. Q2 gaps left: AISLOT.unk10 / unk18 / unk30 (0 / 0 / 0x81 stores only, no reader in any source) and AISLOT.unk1c (a price - s->unk1c = gm_GoodsPrice(*q, ai_OwnPlayer) - but the rename collides with AIENTRY.unk1c, the workshop-level need, so it is a shared-header decision). Q1 and Q6 pass. */

void ai_FillSlotTable(AICHAR *ch, AIOBJ *obj)
{
    AIJOB *job;
    AIRECIPE rec;
    AISLOT *s;
    void *p;
    int *q;
    int i, j, k, id, cnt, found, v;

    cnt = ai_NSlots;
    for (q = ai_WeaponList; *q != 0; q++) {
        for (i = 0; i < cnt; i++) {
            if (ai_Slots[i].item == *q)
                break;
        }
        if (i < cnt)
            continue;
        s = &ai_Slots[cnt];
        s->item = *q;
        s->entry = -1;
        s->bld = 0;
        s->unk10 = 0;
        s->make = 0;
        s->sellAmount = 0;
        s->price = gm_GoodsPrice(*q, ai_OwnPlayer);
        s->pending = 0;
        s->amount = 0;
        s->rank = 0x81;
        s->flags = 0;
        ai_Slots[ai_NSlots].amount = gm_ListAmountById(obj, *q);
        v = gm_FreeCapacity(obj, *q, 1000000);
        cnt = ai_NSlots + 1;
        ai_Slots[ai_NSlots].capacity = v;
        ai_NSlots = cnt;
    }
    for (q = ai_WeaponList2, k = 0; *q != 0; q++, k++) {
        for (i = 0; i < cnt; i++) {
            if (ai_Slots[i].item == *q)
                break;
        }
        if (i < cnt)
            continue;
        s = &ai_Slots[cnt];
        s->item = *q;
        s->entry = -1;
        s->bld = 0;
        s->unk10 = 0;
        s->make = 0;
        s->sellAmount = 0;
        s->price = gm_GoodsPrice(ai_WeaponList[k], ai_OwnPlayer);
        s->pending = 0;
        s->amount = 0;
        s->rank = 0x81;
        s->flags = 0;
        id = gm_ListAmountById(obj, *q);
        ai_Slots[ai_NSlots].amount = id;
        v = gm_FreeCapacity(obj, *q, 1000000);
        cnt = ai_NSlots + 1;
        ai_Slots[ai_NSlots].capacity = v;
        ai_NSlots = cnt;
    }
    for (job = he_FindEvent(2, 0, 0xb, 3, ch->bld->id); job; job = he_NextEvent()) {
        p = gm_FindObjectInst(job->u.id);
        if (!p)
            continue;
        gm_StallGoodsList(p, &rec);
        for (i = 0; i < rec.n; i++) {
            id = rec.item[i];
            if (id != 0) {
                found = 0;
                for (k = 0; k < 3 && !found; k++) {
                    if (job->in[k] == id)
                        found = 1;
                }
                if (!found) {
                    for (j = 0; j < ai_NSlots; j++) {
                        if (id == ai_Slots[j].item)
                            ai_Slots[j].pending += rec.amount[i];
                    }
                }
            }
        }
        for (k = 0; k < 3; k++) {
            if (job->out[k] != 0) {
                for (j = 0; j < ai_NSlots; j++) {
                    if (job->out[k] == ai_Slots[j].item)
                        ai_Slots[j].pending += job->amount[k];
                }
            }
        }
    }
}
