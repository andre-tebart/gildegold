/* 0x47edb0, evaluates the production of a master's workshop @deep B3 far draft (1608/2272 bytes, 702
   instructions, 547 differing; code 720 / regs 76 / slots 42 / fill 9 hunks). Read against 0x47edb0 (Ghidra
   + disassembly), every region compared (the two hourly branch chains, the 32-permuted entry loops, the
   human-player block, the ai_Watch block, the job loop, the two AIINFO loops and the tail): calls,
   arguments, constants, offsets and conditions equal. spell.py leads tried: dup-tail 47ee06 fixed by the
   if/else-if rewrite of the human-player block (1618 -> 1608, kept); reload 47edb7 / 47ee6c / 47ef54 /
   47efae / 47f4bc and dup-tail 47f09a / 47f1a8 / 47f39d / 47f53f name no concrete source form (locals vs
   re-reads, arm duplication of long tails); push-pop 47f120 is mid-function, no guard to move. What is
   left is register allocation: the original keeps ch in ebx and reloads it, ours keeps it in ebp -
   match-deep's slotmap / listing / c2prio territory. Readability done (Q1..Q7 pass): callees
   ai_UpdateProduction / ai_UpdateStaff / ai_BuySlots / ai_BuyCapacity / ai_BuyUpgrade / ai_HasMaterials /
   ai_StartProduction / ai_FreeJobCharacters / ai_SendJobOrders / ai_DrawSettingGood / sv_MoveToGroup /
   gm_FreeCapacity, fields AIENTRY.capacity / amount, AIGOOD.batch, AIBLD.objList, AICHAR.health / title /
   model, AITASK.bldId */

void ai_EvaluateProduction(AICHAR *ch)
{
    AIOBJ *a, *b, *x;
    AIENTRY *e;
    AIGOOD *g;
    AIINFOP info;
    char buf[256];
    int i, j, k, id, r, t, n1, n2, found;

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
    found = 0;
    if (ai_Time.hour == 8 || ai_Time.hour == 0xd) {
        if (!(ch->flags1b4 & 0x100)) {
            ch->prod = 0;
            ch->flags1b4 |= 0x100;
        } else if (ch->prod != 0) {
            for (i = 0; i < ai_NEntries; i++) {
                if (ai_Entries[i].item == ch->prod) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_Goods[e->item].batch && ai_HasMaterials(ch, e, 1)) {
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
                if (e->capacity >= ai_Goods[e->item].batch && ai_HasMaterials(ch, e, 1)) {
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
                if (b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20) {
                    e = &ai_Entries[i];
                    if (e->capacity >= ai_Goods[e->item].batch && (x = gm_FindMarketObject(e->item)) != 0
                        && gm_FreeCapacity(x, e->item, 1000000) >= ai_Goods[e->item].batch + e->amount) {
                        if (ai_HasMaterials(ch, e, 1)) {
                            ai_Entries[i].flags |= 8;
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten, da Humanplayer es so will.", ai_Goods[e->item].name);
                            goto done;
                        } else if (ai_HasMaterials(ch, e, 0)) {
                            ai_StartProduction(ch, e);
                            ch->prod = (unsigned short)e->item;
                            sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen, da der spieler es so will",
                                    ai_Goods[e->item].name);
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
        e = &ai_Entries[info.perm[i]];
        g = &ai_Goods[e->item];
        if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c && b->lvl1d >= e->need20
            && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
            && gm_FreeCapacity(x, e->item, 1000000) >= ai_Goods[e->item].batch + e->amount && ai_HasMaterials(ch, e, 1)) {
            k = info.perm[i];
            ai_Entries[k].flags |= 8;
            e = &ai_Entries[k];
            ai_StartProduction(ch, e);
            ch->prod = (unsigned short)e->item;
            sprintf(buf, "Produktion von %s starten", ai_Goods[e->item].name);
            goto done;
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        e = &ai_Entries[info.perm[i]];
        g = &ai_Goods[e->item];
        if (((g->type != 0x17 && g->type != '%') || e->amount < 5) && b->lvl1c >= e->need1c && b->lvl1d >= e->need20
            && e->capacity >= g->batch && (x = gm_FindMarketObject(e->item)) != 0
            && gm_FreeCapacity(x, e->item, 1000000) >= ai_Goods[e->item].batch + e->amount && ai_HasMaterials(ch, e, 0)) {
            e = &ai_Entries[info.perm[i]];
            ai_StartProduction(ch, e);
            ch->prod = (unsigned short)e->item;
            sprintf(buf, "Produktion von %s starten obwohl Rohstoffe fehlen", ai_Goods[e->item].name);
            goto done;
        }
    }
    ch->prod = 0;
done:
    ai_SendRemoveEmptyGoods(b);
    ai_PlanSlots(ch, b);
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
            && ai_Chars[j].title && ai_Chars[j].model && ai_Chars[j].model->bldId == ai_Chars[j].bld->id) {
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
    if (n2 != 0 && info.n1 != 0) {
        info.c = 0;
        info.z2 = 0;
        info.z1 = 0;
        info.ch = ch;
        info.b = b;
        for (j = 0; j < ai_NEntries; j++) {
            if (ai_Entries[j].flags & 8) {
                info.type = 4;
                info.e = &ai_Entries[j];
                ai_SendJobOrders((AIINFO *)&info);
                if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                    ai_Watch.entries[ai_Watch.nentries] = *info.e;
                    ai_Watch.nentries++;
                }
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
