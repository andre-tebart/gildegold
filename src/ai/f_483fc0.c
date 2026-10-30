/* 0x483fc0, evaluates the production of a master's workshop (the variant with the 0x400 entry flag) @deep B3 far draft (668 code / 40 regs / 22 slots / 2 fill hunks). Every hunk of the first blocks was read against tools/disasm.py 0x483fc0 and the VC6 listing (tools/listing.py): statements, constants, callees and field offsets match; the whole diff is one form - the original keeps the AIENTRY byte offset (i*0x58) in esi and folds each field into the absolute address ([esi+0xf88a80] = capacity, [esi+0xf88a94] = flags, [esi+0xf88a40] = item) while ours materializes &ai_Entries[i] and uses [esi+0x40] / [esi+0x54]; the compare operands and the esi/edi roles follow from that. Last-bytes pass lead (source spelling, the meaning-class lead): spell the entry accesses as ai_Entries[i].item / ai_Entries[i].capacity and pass &ai_Entries[i] as the argument; the pattern occurs three times in this function and the trial edit was cut off by the session end, the source is unchanged (no WORSE: unitdiff ai.c 29/55, 37601 B before and after). What is left is that rewrite plus the allocation/scheduling of the other blocks = match-deep. Q1 blocked: the five production variants of 0x47e990 differ only in the AIENTRY flags they test and the kind-to-profession table is in no source (0x47e990's note). Q2 gaps: ai_Unk487ea0 / ai_Unk489480 / ai_SendFreeSpies (roles readable from the bodies but not evidenced by a name) and AIENTRY.unk1c / unk20 (the ai.h need1c / need20 aliases exist; the project-wide rename is undecided - AIOBJ has unk1c / unk20 with other roles). Q6 done (description above the definition). */

void ai_Unk483fc0(AICHAR *ch)
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
    ai_Unk487ea0(ch);
    ai_BuySlots(b, ch, 0x4b);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    ai_Unk489480(ch);
    ai_SendFreeSpies(ch);
    ai_EndWorkerPose(ch);
    if (ai_TimeT.hour == 8 || ai_TimeT.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    e = &ai_Entries[i];
                    if (ai_GoodsT[e->item].batch <= e->capacity && ai_HasMaterials(ch, e, 1)) {
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
                if (ai_GoodsT[e->item].batch <= e->capacity && ai_HasMaterials(ch, e, 1)) {
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
    if (ai_CharsT[ch->bld->owner].kind == 6 && (id = ai_DrawSettingGood(ch->bld)) != 0) {
        for (i = 0; i < 32; i++) {
            if (ai_Entries[i].item == id) {
                if (ai_Entries[i].unk1c <= b->lvl1c && ai_Entries[i].unk20 <= b->lvl1d) {
                    e = &ai_Entries[i];
                    if (ai_GoodsT[e->item].batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                        && ai_GoodsT[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)) {
                        if (!ai_HasMaterials(ch, e, 1)) {
                            if (ai_HasMaterials(ch, e, 0)) {
                                ai_StartProduction(ch, e);
                                ch->prod = (unsigned short)e->item;
                                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                        ai_GoodsT[e->item].name);
                                goto done;
                            }
                        } else {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_GoodsT[e->item].name);
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
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && e->unk1c <= b->lvl1c
                && e->unk20 <= b->lvl1d && g->batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                && ai_GoodsT[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)
                && ai_HasMaterials(ch, e, 1)) {
                k = info.perm[i];
                ai_Entries[k].flags |= 8;
                e = &ai_Entries[k];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten", ai_GoodsT[e->item].name);
                goto done;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        k = info.perm[i];
        if (!(ai_Entries[k].flags & 0x400)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && e->unk1c <= b->lvl1c
                && e->unk20 <= b->lvl1d && g->batch <= e->capacity && (x = gm_FindMarketObject(e->item)) != 0
                && ai_GoodsT[e->item].batch + e->amount <= gm_FreeCapacity(x, e->item, 1000000)
                && ai_HasMaterials(ch, e, 0)) {
                e = &ai_Entries[info.perm[i]];
                ai_StartProduction(ch, e);
                ch->prod = (unsigned short)e->item;
                sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_GoodsT[e->item].name);
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
                    if (ai_Slots[ai_Entries[i].slot[k]].amount < ai_GoodsT[ai_Entries[i].item].need[k])
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
        if (ai_CharsT[j].health && ai_CharsT[j].id != 0xffff && ai_CharsT[j].kind != 10 && ai_CharsT[j].bld == ch->bld
            && ai_CharsT[j].title && ai_CharsT[j].model && ai_CharsT[j].model->bldId == ai_CharsT[j].bld->id
            && (!ai_CharsT[j].job || ai_CharsT[j].job->kind == 4)) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
            n2 = info.n2 + 1;
            info.n2 = n2;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = n2;
    ai_StartBuildingWork(ch);
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
