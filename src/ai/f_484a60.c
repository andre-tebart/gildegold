/* 0x484a60, evaluates the production of a master's workshop (the variant with the 0x400 entry flag) @deep B3 far draft (765 code / 24 regs / 18 slots / 9 fill hunks). The first blocks were read against tools/disasm.py 0x484a60 and tools/listing.py: statements, constants, callees and field offsets match; the whole diff is the same entry form as 0x483fc0 - the original keeps the AIENTRY byte offset (i*0x58) in esi and folds each field into the absolute address ([esi+0xf88a80] = capacity) while ours materializes &ai_Entries[i] and uses [esi+0x40] / [esi+0x54]; the compare operands and the esi/edi roles follow from it. Last-bytes pass lead (source spelling, measured one edit one build): the first prod loop spelled as ai_Entries[i].item / ai_Entries[i].capacity with &ai_Entries[i] as the argument took the asm diff 612 -> 559 lines and the code hunks 765 -> 715 (fill 9 -> 4) but the differing bytes 1839 -> 1861, so unitdiff said WORSE and it was reverted (source unchanged) - match-deep: the spelling moves the instruction text toward the original, the remaining blocks need the same rewrite plus the allocation/scheduling. Q1 blocked: the five production variants of 0x47e990 differ only in the AIENTRY flags they test and the kind-to-profession table is in no source (0x47e990's note). Q2 done: AIENTRY.unk1c / unk20 -> the ai.h need1c / need20 aliases (AIOBJ.lvl1c / lvl1d compare against them), byte-neutral. Q6 done. */

void ai_Unk484a60(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, n1, n2, found;

    found = 0;
    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    ai_UpdateProduction(ch, a, b);
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    e = &ai_Entries[i];
                    if (ai_Goods[e->item].batch <= e->capacity && ai_HasMaterials(ch, e, 1)) {
                        ai_Entries[i].flags |= 8;
                        ai_StartProduction(ch, e);
                        ch->prod = (unsigned short)e->item;
                        goto done;
                    }
                    break;
                }
            }
        }
    } else {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].item == ch->prod) {
                e = &ai_Entries[i];
                if (ai_Goods[e->item].batch <= e->capacity && ai_HasMaterials(ch, e, 1)) {
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, e);
                    ch->prod = (unsigned short)e->item;
                    found = 1;
                }
                break;
            }
        }
        ch->flags1b4 &= ~0x100;
        if (found)
            goto done;
    }
    if (ai_Chars[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (ai_Entries[i].need1c <= b->lvl1c && ai_Entries[i].need20 <= b->lvl1d) {
                    e = &ai_Entries[i];
                    if (ai_Goods[e->item].batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                        && ai_Goods[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)) {
                        if (!ai_HasMaterials(ch, e, 1)) {
                            if (ai_HasMaterials(ch, e, 0)) {
                                ai_StartProduction(ch, e);
                                ch->prod = (unsigned short)e->item;
                                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                        ai_Goods[e->item].name);
                                goto done;
                            }
                        } else {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_Goods[e->item].name);
                            goto done;
                        }
                    }
                }
                break;
            }
        }
    }
    for (i = 0; i < 32; i++)
        info.perm[i] = i;
    i = 0;
    do {
        r = util_Random(0x20);
        t = info.perm[i];
        i++;
        info.perm[i - 1] = info.perm[r];
        info.perm[r] = t;
    } while (i < 32);
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_Goods[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && e->need1c <= b->lvl1c
                && e->need20 <= b->lvl1d && g->batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                && ai_Goods[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)
                && ai_HasMaterials(ch, e, 1)) {
                k = info.perm[i];
                ai_Entries[k].flags |= 8;
                e = &ai_Entries[k];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten", ai_Goods[e->item].name);
                goto done;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_Goods[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && e->need1c <= b->lvl1c
                && e->need20 <= b->lvl1d && g->batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                && ai_Goods[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)
                && ai_HasMaterials(ch, e, 0)) {
                e = &ai_Entries[info.perm[i]];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_Goods[e->item].name);
                goto done;
            }
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
    if (0 < ai_NEntries) {
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].flags & 8) {
                for (k = 0; k < 4; k++) {
                    if (ai_Slots[ai_Entries[i].slot[k]].amount < ai_Goods[ai_Entries[i].item].need[k])
                        ai_Entries[i].flags &= ~8;
                }
            }
        }
        for (i = 0; i < ai_NEntries; i++) {
            if (ai_Entries[i].flags & 0x400)
                ai_Entries[i].flags &= ~8;
        }
    }
    for (i = 0; i < ai_NSlots; i++) {
        if ((ai_Slots[i].flags & 0x400) && (ai_Slots[i].flags & 0x10) && ai_Slots[i].entry >= 0)
            ai_Entries[ai_Slots[i].entry].flags |= 0x10;
    }
    if (ai_Watch.bld == ch->bld) {
        for (j = 0; ai_Watch.nslots < 4; j++) {
            if (ai_NEntries <= j)
                break;
            ai_Watch.ent[ai_Watch.nslots] = ai_Entries[j];
            for (k = 0; k < 4; k++) {
                if (ai_Entries[j].slot[k] >= 0) {
                    ai_Watch.slots[ai_Watch.cnt[ai_Watch.nslots] + ai_Watch.nslots * 4] =
                        ai_Slots[ai_Entries[j].slot[k]];
                    ai_Watch.cnt[ai_Watch.nslots]++;
                }
            }
            ai_Watch.nslots++;
        }
    }
    ai_FreeJobCharacters(ch, 4);
    n2 = 0;
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_Chars[j].health && ai_Chars[j].id != 0xffff && ai_Chars[j].kind != 10 && ai_Chars[j].bld == ch->bld
            && ai_Chars[j].title && ai_Chars[j].model && ai_Chars[j].model->bldId == ai_Chars[j].bld->id
            && (!ai_Chars[j].job || ai_Chars[j].job->kind == 4)) {
            sv_MoveToGroup(ch->bld, 0, ai_Chars[j].id, 0);
            n2 = info.n2 + 1;
            info.n2 = n2;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = n2;
    n1 = 0;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            n1++;
        if (ai_Entries[i].flags & 0x10)
            n1++;
        info.n1 = n1;
    }
    if (n2 <= info.n1)
        info.n1 = n2;
    if (n2 == 0 || info.n1 == 0) {
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    info.z1 = 0;
    info.z2 = 0;
    info.c = 0;
    info.ch = ch;
    info.b = b;
    for (j = 0; j < ai_NEntries; j++) {
        e = &ai_Entries[j];
        if (!(e->flags & 0x10)) {
            if (e->flags & 8) {
                info.e = e;
                info.type = 4;
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *e;
                    ai_Watch.nentries++;
                }
            }
        } else if (e->flags & 0x400) {
            info.e = e;
            info.type = 0x8c;
            ai_SendJobOrders((AIINFO *)&info);
            if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                ai_Watch.entries[ai_Watch.nentries] = *e;
                ai_Watch.nentries++;
            }
        }
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
