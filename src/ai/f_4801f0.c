/* 0x4801f0, evaluates the production of a master's workshop of the building category 2 (the dispatcher
   0x47e990 comes here when gm_BuildingClass(type) == 2): buys slots / capacity / upgrades, starts the entries'
   production, sends the job orders and finishes with the trade (ai_MeisterCollectTrans / ai_MeisterTrade);
   the same production steps as ai_EvaluateProduction for this category's AIENTRY flags
   @deep B3 far draft (970/975 instructions, 447 differing; diffclass 708 code / 42 regs / 38 slots / 5 fill): every hunk read against build/ghidra/4801f0.c and tools/asmdiff - no missing / extra statement, constant, callee or field offset; the whole diff is the c/ch register swap (ours c=ebp ch=ebx, original c=ebx ch=ebp) with the [esp+0x10]/[0x1c]/[0x20]/[0x24] slot rotation and the reloads it causes, i.e. allocation / scheduling. spell.py: the reload (19x) and dup-tail (6x) entries all describe that same allocation; no meaning-class lead (wrong type / signedness / offset / width) exists to try one edit on, so the last-bytes pass leaves nothing to measure. Q1 named by role (0x47e990 dispatches here on gm_BuildingClass(type) == 2). Q2 gap: AIMSG.unk14 is only ever written 0 by the ai.c senders (hst.c -1) and has no reader that names it. */

void ai_EvaluateProductionCategory2(AICHAR *ch)
{
    AIOBJ *a, *b, *c, *o, *o2;
    AIJOB *job;
    AIGOOD *g;
    AIMAP *map;
    AIPLANT *plant;
    void *x;
    AIINFO info;
    AIMSG msg;
    int i, j, k, n, id, found, v71;
    int r, y, idx;

    a = gm_Query(ch->bld->objList, 2, 6, 0, 0xff);
    if (!a)
        return;
    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (!b)
        return;
    c = gm_Query(ch->bld->objList, 1, 0, 0xfe);
    if (!c)
        return;
    ai_UpdateProduction(ch, a, b);
    for (i = 0; i < ai_NEntries; i++) {
        for (k = 0; k < 4; k++) {
            if (ai_Entries[i].slot[k] >= 0 && ai_Slots[ai_Entries[i].slot[k]].entry >= 0) {
                ai_Entries[ai_Slots[ai_Entries[i].slot[k]].entry].flags |= 4;
                for (j = 0; j < ai_NSlots; j++) {
                    if (ai_Entries[ai_Slots[ai_Entries[i].slot[k]].entry].item == ai_Slots[j].item)
                        ai_Slots[j].flags |= 4;
                }
            }
        }
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 10);
    ai_BuyCapacity(b, ch, 0x19);
    ai_BuyUpgrade(ch);
    o = a;
    if (!(ch->flags1c8 & 0x10) && ai_BTypes[ch->bld->type].kind != 11) {
        for (o = gm_Query(a->list, 1, 4, 0x17); o; o = gm_QueryNext()) {
            if (o->count == 1) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 9;
                msg.to = ai_Chars[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.nIds = 0;
                msg.unk14 = 0;
                msg.time = ai_TimeT;
                msg.flags = 1;
                msg.arg[0] = ch->bld->id;
                msg.arg[1] = a->objId;
                msg.arg[2] = c->objId;
                cm_SendLongMessage(&msg);
                break;
            }
        }
        ch->flags1c8 |= 0x10;
    }
    found = 0;
    job = he_FindEvent(2, 0, 9, 3, ch->bld->id);
    if (job) {
        if (ch->job != job)
            sv_MoveToGroup(ch->bld, 0, ch->id, job);
        found = 1;
    }
    if (ai_Chars[ch->bld->owner].kind == 6) {
        id = ai_DrawSettingGood(ch->bld);
        if (o) {
            for (i = 0; i < 32; i++) {
                if (ai_Entries[i].item == id) {
                    if (b->lvl1c < ai_Entries[i].need1c || b->lvl1d < ai_Entries[i].need20)
                        break;
                    if (ai_Entries[i].capacity < ai_Goods[ai_Entries[i].item].batch)
                        break;
                    x = gm_FindMarketObject(ai_Entries[i].item);
                    if (!x)
                        break;
                    r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
                    g = &ai_Goods[ai_Entries[i].item];
                    if (r < g->batch + ai_Entries[i].amount)
                        break;
                    if (g->type == 0x17) {
                        o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                        if (!o2 || o2->count < ai_Goods[ai_Entries[i].item].batch)
                            break;
                    } else if (!ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                        if (ai_HasMaterials(ch, &ai_Entries[i], 0)) {
                            ai_StartProduction(ch, &ai_Entries[i]);
                            goto check;
                        }
                        break;
                    }
                    ai_Entries[i].flags |= 8;
                    ai_StartProduction(ch, &ai_Entries[i]);
                    goto check;
                }
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (!(ai_Entries[i].flags & 4) && b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20
            && ai_Entries[i].capacity >= ai_Goods[ai_Entries[i].item].batch
            && (x = gm_FindMarketObject(ai_Entries[i].item)) != 0) {
            r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
            g = &ai_Goods[ai_Entries[i].item];
            if (r >= g->batch + ai_Entries[i].amount) {
                if (g->type == 0x17) {
                    o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                    if (!o2 || o2->count < ai_Goods[ai_Entries[i].item].batch)
                        continue;
                } else if (!ai_HasMaterials(ch, &ai_Entries[i], 1)) {
                    continue;
                }
                ai_Entries[i].flags |= 8;
                ai_StartProduction(ch, &ai_Entries[i]);
                break;
            }
        }
    }
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            break;
        if (!(ai_Entries[i].flags & 4) && b->lvl1c >= ai_Entries[i].need1c && b->lvl1d >= ai_Entries[i].need20
            && ai_Entries[i].capacity >= ai_Goods[ai_Entries[i].item].batch
            && (x = gm_FindMarketObject(ai_Entries[i].item)) != 0) {
            r = gm_FreeCapacity(x, ai_Entries[i].item, 1000000);
            g = &ai_Goods[ai_Entries[i].item];
            if (r >= g->batch + ai_Entries[i].amount) {
                if (g->type == 0x17) {
                    ai_Entries[i].flags |= 0x10;
                    break;
                }
                if (ai_HasMaterials(ch, &ai_Entries[i], 0)) {
                    ai_StartProduction(ch, &ai_Entries[i]);
                    break;
                }
            }
        }
    }
check:
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8) {
            g = &ai_Goods[ai_Entries[i].item];
            if (g->type == 0x17) {
                if (ai_BTypes[ch->bld->type].kind == 11) {
                    ai_Entries[i].flags |= 0x10;
                } else {
                    o2 = gm_Query(c->list, 1, 0, ai_Entries[i].item);
                    if (!o2 || o2->count < ai_Goods[ai_Entries[i].item].batch)
                        ai_Entries[i].flags = ai_Entries[i].flags & ~8 | 0x10;
                }
            } else {
                for (k = 0; k < 4; k++) {
                    if (ai_Slots[ai_Entries[i].slot[k]].amount < g->need[k])
                        ai_Entries[i].flags &= ~8;
                }
            }
        }
    }
    ai_FreeJobCharacters(ch, 4);
    ai_FreeJobCharacters(ch, 8);
    ai_FreeJobCharacters(ch, 3);
    info.n2 = 0;
    for (j = 0; j < 768; j++) {
        if (ai_Chars[j].health > 0 && ai_Chars[j].id != 0xffff && ai_Chars[j].kind != 10
            && ai_Chars[j].bld == ch->bld && ai_Chars[j].title && ai_Chars[j].model
            && ai_Chars[j].model->bldId == ai_Chars[j].bld->id && (!ai_Chars[j].job || ai_Chars[j].job->kind != '(')
            && (!found || ch != &ai_Chars[j])) {
            sv_MoveToGroup(ch->bld, 0, ai_Chars[j].id, 0);
            info.n2++;
        }
    }
    if (ai_Watch.bld == ch->bld)
        ai_Watch.n = info.n2;
    info.n1 = 0;
    for (i = 0; i < ai_NEntries; i++) {
        if (ai_Entries[i].flags & 8)
            info.n1++;
        if ((ai_Entries[i].flags & 0x10) && ai_BTypes[ch->bld->type].kind != 11)
            info.n1++;
    }
    if (info.n1 >= info.n2)
        info.n1 = info.n2;
    if (info.n2 && info.n1) {
        info.b = b;
        info.c = c;
        info.ch = ch;
        info.z2 = 0;
        info.z1 = 0;
        if (ai_BTypes[ch->bld->type].kind == 11) {
            if (ai_TimeT.hour % 2 == 0) {
                ch->flags1b4 &= ~0x20;
            } else if (!(ch->flags1b4 & 0x20)) {
                map = ((AIMAPREF *)&ai_MapRefs)[init_BuildingSceneUniverse(ch->bld)].map;
                if (!map)
                    return;
                v71 = ch->bld->plants;
                for (j = 0; j < 8; j++) {
                    if (strstr(map->names[j], "PFLANZBAR"))
                        idx = j;
                }
                for (i = 0; i < ai_NEntries * 4; i++) {
                    r = util_Random(ai_NEntries);
                    if (ai_Goods[ai_Entries[r].item].type == 0x17)
                        break;
                }
                if (ai_Goods[ai_Entries[r].item].type == 0x17) {
                    plant = veg_FindType(ai_Entries[r].item);
                    n = 0;
                    for (y = 0; n < 4; y++) {
                        if (y >= map->w)
                            break;
                        for (i = 0; n < 4 && i < map->w; i++) {
                            if (map->cells[map->w * y + i] == idx && veg_Fits(v71, i, y, plant->size)) {
                                cm_SetPlant(ch->bld->id, i, y, plant->id, 0, -1);
                                n++;
                            }
                        }
                    }
                }
                ch->flags1b4 |= 0x20;
            }
        }
        for (i = 0; i < ai_NEntries; i++) {
            if ((ai_Entries[i].flags & 0x10) && ai_BTypes[ch->bld->type].kind != 11) {
                info.type = 8;
                info.e = &ai_Entries[i];
                ai_SendJobOrders(&info);
            }
            if (ai_Entries[i].flags & 8) {
                if (ai_Goods[ai_Entries[i].item].type == 0x17) {
                    if (ai_BTypes[ch->bld->type].kind != 11) {
                        info.type = 3;
                        info.e = &ai_Entries[i];
                        ai_SendJobOrders(&info);
                        if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                            ai_Watch.entries[ai_Watch.nentries] = *info.e;
                            ai_Watch.nentries++;
                        }
                    } else {
                        info.type = 0x28;
                        info.e = &ai_Entries[i];
                        ai_SendJobOrders(&info);
                        if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                            ai_Watch.entries[ai_Watch.nentries] = *info.e;
                            ai_Watch.nentries++;
                        }
                    }
                } else {
                    info.type = 4;
                    info.e = &ai_Entries[i];
                    ai_SendJobOrders(&info);
                    if (ai_Watch.bld == ch->bld && ai_Watch.nentries < 4) {
                        ai_Watch.entries[ai_Watch.nentries] = *info.e;
                        ai_Watch.nentries++;
                    }
                }
            }
        }
        ai_SendRemoveEmptyGoods(b);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
        return;
    }
    ai_MeisterCollectTrans(ch);
    ai_MeisterTrade(ch, b);
}
