/* 0x47f700, evaluates the production of a master's workshop, the variant with the 0x20 / 0x200 entry flags
   @deep B3 far draft (1867/2608 bytes, 820 instructions, 545 differing; code 730 / regs 16 / order 10 /
   fill 3 hunks). Read region by region against Ghidra 47f700: the hourly chains, the 0x20 / 0x200 entry-flag
   tests and loops, the human-player block, the flags-8 cleanup and slot loop, the ai_Watch copy blocks, the
   job loop (kind != 0x14 / 0x15), the n1 / n2 clamp and the three AIINFO job-order branches (flags 0x10 / 8
   -> type 4, 0x20 / 0x200 -> 0x15, else 0x14) - calls, arguments, constants and conditions equal. The done
   label verified in the disassembly (0x47fddc: [esp+0x14] = b is pushed to both 0x47ed60 and 0x47ec20); the
   Ghidra iVar3 there is scratch reuse, not a wrong argument. spell.py leads: the human-player if/else-if
   rewrite (the 47edb0 fix) is 1867 -> 1877 = WORSE, reverted; reload 47f7d2 / 47f9fb (locals across
   calls), dup-tail 47f8fa / 47faf2 / 47fd25 / 47fe4c (long tails with no shared source tail) and cmp-test
   47ff45 (je vs jbe) name no concrete source form. What is left is register allocation / scheduling (820
   instructions, regs only 16 hunks) - match-deep's slotmap / listing / c2prio. Readability done: Q1 named
   ai_EvaluateProductionFlags, Q2 / Q6 clean (all callees and fields named in the sibling session, the
   comment states the variant). */

void ai_EvaluateProductionFlags(AICHAR *ch)
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
    if (ai_TimeT.hour == 8 || ai_TimeT.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_GoodsT[e->item].batch && ai_HasMaterials(ch, e, 1)) {
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
                if (e->capacity >= ai_GoodsT[e->item].batch && ai_HasMaterials(ch, e, 1)) {
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
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_GoodsT[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount) {
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
        if (!(ai_Entries[k].flags & 0x20) && !(ai_Entries[k].flags & 0x200)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c
                && b->lvl1d >= e->need20 && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount
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
        if (!(ai_Entries[k].flags & 0x20) && !(ai_Entries[k].flags & 0x200)) {
            e = &ai_Entries[k];
            g = &ai_GoodsT[e->item];
            if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c
                && b->lvl1d >= e->need20 && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
                && gm_FreeCapacity(x, e->item, 1000000) >= ai_GoodsT[e->item].batch + e->amount
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
            if ((ai_Entries[i].flags & 0x20) || (ai_Entries[i].flags & 0x200))
                ai_Entries[i].flags &= ~8;
        }
    }
    for (i = 0; i < ai_NSlots; i++) {
        if (((ai_Slots[i].flags & 0x20) || (ai_Slots[i].flags & 0x200)) && (ai_Slots[i].flags & 0x10)
            && ai_Slots[i].entry >= 0)
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
            && (!ai_CharsT[j].job || (ai_CharsT[j].job->kind != 0x14 && ai_CharsT[j].job->kind != 0x15))) {
            sv_MoveToGroup(ch->bld, 0, ai_CharsT[j].id, 0);
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
    info.c = 0;
    info.z2 = 0;
    info.z1 = 0;
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
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        } else if (!(e->flags & 0x20)) {
            if (e->flags & 0x200) {
                info.e = e;
                info.type = 0x15;
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        } else {
            info.e = e;
            info.type = 0x14;
            ai_SendJobOrders((AIINFO *)&info);
            if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                ai_Watch.entries[ai_Watch.nentries] = *info.e;
                ai_Watch.nentries++;
            }
        }
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
