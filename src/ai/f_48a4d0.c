/* 0x48a4d0, the trade master's round (returns 1; a building of type 0x116 goes to ai_MeisterTradeAllgemein):
   on the odd hour it plans the sells - the slots whose goods the store holds enough of get a sellAmount (half for
   a kind-22 master), the entries get theirs from the price ratio of the boss / the money shortage / a full
   store - and sets the flags1b4 bit 0x200 (cleared on the even hour); then it refreshes the two goods lists of
   the building (gm_BuildingGoodsList, kind 1 / 0) and resets the field +0x1c of the goods objects of the buy
   slots (and of the kinds 13 / 11 / 12 entries). Every cart of the building (gm_Query kind 3, the 0x1d goods,
   the owner) is then handled: a cart below a third full gets the reload message 0x10 (10 seconds ahead); a full
   cart without a running job 11 is unloaded - the goods are sold to the market stalls (cm_SendExSellObject, the
   sellAmount counts down) and the buys of the slots with a make plan are collected into one message 11 (up to
   three sells and three buys), sent unless the ai_TradeMsgOff gate is set (then ai_TradeMsgCount counts it).
   The gm_Query lists are walked to their end (the second loop drains the cursor).
   @deep behaviour draft: 1955 code hunks / 4851 of 6032 bytes. Meaning fix found and kept this session: the field-delta block compared o->unk1c with 4 and wrote 4; the original tests o->unk1c != 0 and writes 0 (0x48aa34: mov eax,[esi+0x1c]; test eax,eax; je; mov [esp+0x10],0; cm_AddFieldDelta(4,1,o - cm_FieldObject + 0x1c,&v)) - 4894 -> 4851 bytes, 1316 -> 1310 asm lines. The rest of the diff (tools/asmdiff.py) is allocation / scheduling: the frame 0x74c vs 0x754 (one spilled local), the ebp / edi register pair and the block order. Q2 evidence gaps: ai_TradeMsgOff (a gate global, read only, no writer in any source, no reader names it), AIOBJ.unk1c (a state here, a building id / count / level elsewhere in its union), AIMSG.u.s.unk84 (only ever written 0 in the trade messages), AIMSG.unk9d (written 1 in the 0x10 messages, no other reader). Readability advanced this session: AIENTRY.unk44 -> sellAmount, AISLOT.unk18 -> sellAmount, AIOBJ.unk12 -> fill, AISLOT.unk08 -> the existing bld alias, gm_AddTime / gm_BuildingGoodsList / ai_SettingFactor names, Q6 rewritten. */

int ai_MeisterTrade(AICHAR *ch, AIOBJ *b)
{
    AIOBJ *car, *market, *o;
    AISLOT *s;
    AIJOB *job;
    AIRECIPE stA, rec, stB;
    AIMSG msg;
    char buf[1024];
    unsigned short it;
    float f, g;
    int i, j, k, m, n, v, lim, found, nsell, nbuy, sold;

    if (b->type == 0x116)
        return ai_MeisterTradeAllgemein(ch, b);
    for (i = 0; i < ai_NSlots; i++) {
        if (!(ai_Slots[i].flags & 6) && (ai_Slots[i].flags & 8)) {
            v = ai_Slots[i].amount;
            if (v > gm_ListStock(b, ai_Slots[i].item) / 3) {
                if (ai_BTypes[ch->bld->type].kind == 22)
                    ai_Slots[i].sellAmount = v / 2;
                else
                    ai_Slots[i].sellAmount = v;
            }
        }
    }
    if (ai_TimeT.hour % 2) {
        if (!(ch->flags1b4 & 0x200)) {
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6) && (!(ai_Entries[i].flags & 8) || ai_Entries[i].amount > 3)) {
                    if (ai_Chars[ch->bld->owner].kind == 6) {
                        f = ai_SettingFactor(ch->bld);
                        g = gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        if (f <= g / gm_ItemPrice(ai_Entries[i].item, 100)) {
                            ai_Entries[i].sellAmount = ai_Entries[i].amount;
                            if (ch == ai_LoggedMaster)
                                sprintf(buf, "Verkauf von %li Einheiten %s, weil der Preis f\xfcr den Boss ok ist",
                                        ai_Entries[i].amount, ai_Goods[ai_Entries[i].item].name);
                        }
                    } else {
                        g = gm_ItemPrice(ai_Entries[i].item, 100);
                        f = g / gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        if (f < 1.0f && f <= util_RandomF() * 1.25f + 0.25) {
                            ai_Entries[i].sellAmount = ai_Entries[i].amount;
                            if (ch == ai_LoggedMaster)
                                sprintf(buf, "Verkauf wegen guter Gewinnspanne von %li Einheiten %s",
                                        ai_Entries[i].sellAmount, ai_Goods[ai_Entries[i].item].name);
                        }
                    }
                }
            }
            if (ch->budget < 3200 || gm_GetGoodsValue(&ai_Chars[ch->bld->owner]) < 8000) {
                v = min(ch->budget, gm_GetGoodsValue(&ai_Chars[ch->bld->owner]));
                for (i = 0; v < 32000 && i < ai_NEntries; i++) {
                    if (!(ai_Entries[i].flags & 6)) {
                        k = (int)gm_GoodsPrice(ai_Entries[i].item, ai_OwnPlayer);
                        n = 32000 / k;
                        if (n < 1)
                            n = 1;
                        ai_Entries[i].sellAmount = min(n, ai_Entries[i].amount);
                        v += n * k;
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s", ai_Entries[i].sellAmount,
                                    ai_Goods[ai_Entries[i].item].name);
                    }
                }
            }
            for (i = 0; i < ai_NEntries; i++) {
                if (!(ai_Entries[i].flags & 6)) {
                    if (ai_Entries[i].amount > gm_ListStock(b, ai_Entries[i].item) * 3 / 4) {
                        ai_Entries[i].sellAmount = max(5, ai_Entries[i].amount / 4);
                        ai_Entries[i].sellAmount = min(ai_Entries[i].sellAmount, ai_Entries[i].amount);
                        if (ch == ai_LoggedMaster)
                            sprintf(buf, "Notverkauf von %li Einheiten %s wegen \xfc" "berf\xfc" "lltem Lager",
                                    ai_Entries[i].sellAmount, ai_Goods[ai_Entries[i].item].name);
                    }
                }
            }
        }
        ch->flags1b4 |= 0x200;
    } else {
        ch->flags1b4 &= ~0x200;
    }
    gm_BuildingGoodsList(ch->bld, &stA, 1);
    gm_BuildingGoodsList(ch->bld, &stB, 0);
    for (j = 0; j < ai_NSlots; j++) {
        if ((ai_Slots[j].flags & 6) && (o = gm_Query(b->list, 1, 0, ai_Slots[j].item)) != NULL) {
            if (o->unk1c != 0) {
                v = 0;
                cm_BeginFieldBlock(o, o->objId);
                cm_AddFieldDelta(4, 1, (char *)o - cm_FieldObject + 0x1c, &v);
                cm_SendFieldDeltas();
            }
        }
    }
    k = ai_BTypes[ch->bld->type].kind;
    if (k == 13 || k == 11 || k == 12) {
        for (i = 0; i < ai_NEntries; i++) {
            if (!(ai_Entries[i].flags & 6) && (o = gm_Query(b->list, 1, 0, ai_Entries[i].item)) != NULL
                && o->unk1c != 0) {
                v = 0;
                cm_BeginFieldBlock(o, o->objId);
                cm_AddFieldDelta(4, 1, (char *)o - cm_FieldObject + 0x1c, &v);
                cm_SendFieldDeltas();
            }
        }
    }
    for (car = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); car;
         car = gm_QueryNext()) {
        if (car->unk1c == ch->bld->id) {
            if (car->fill < 0x21) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x10;
                msg.to = ai_Chars[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.flags = 1;
                msg.time = ai_TimeT;
                gm_AddTime(&msg.time, 0, 0, 10);
                msg.car = car->objId;
                msg.dst[0] = 5;
                msg.u.s.unk84 = 0;
                msg.n = 100 - car->fill;
                cm_SendLongMessage(&msg);
                continue;
            }
            for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
                if (job->u.id == car->objId)
                    break;
            }
            if (!job) {
                memset(&msg, 0, sizeof(msg));
                nsell = 0;
                nbuy = 0;
                if (ai_WorkLoad() > ai_WorkLimit())
                    msg.unk9d = 1;
                for (k = 0; k < 3; k++) {
                    msg.u.item[k] = 0;
                    msg.dst[k] = -1;
                    msg.u.item[k + 3] = 0;
                    msg.dst[k + 3] = -1;
                }
                gm_StallGoodsList(car, &rec);
                market = gm_Query(car->list, 1, 0, 0x1dd);
                if (!market)
                    market = gm_Query(car->list, 1, 0, 0x1dd);
                sold = 0;
                for (i = 0; i < rec.n; i++) {
                    if (!rec.item[i])
                        continue;
                    sprintf(buf, "ai_MeisterTrade(): Von Auto in \"%s\" %li Einheiten %s entladen", ch->bld->name,
                            rec.amount[i], ai_Goods[rec.item[i]].name);
                    s = NULL;
                    for (m = 0; !s && m < 128; m++) {
                        if (ai_Slots[m].item == rec.item[i])
                            s = &ai_Slots[m];
                    }
                    if (s && (s->flags & 2)) {
                        found = 0;
                        for (j = 0; !found && j < stA.n; j++) {
                            if (stA.item[j] == rec.item[i]) {
                                sprintf(buf, "ai_MeisterTrade(): %s ist in \"%s\" schon vorhanden",
                                        ai_Goods[rec.item[i]].name, ch->bld->name);
                                n = min(rec.amount[i], stA.max[j] - stA.amount[j]);
                                if (n) {
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                }
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (!stA.item[j]) {
                                sprintf(buf, "ai_MeisterTrade(): \"%s\" hat leeren Slot f\xfcr %s", ch->bld->name,
                                        ai_Goods[rec.item[i]].name);
                                n = gm_ListStock(b, rec.item[i]);
                                if (rec.amount[i] < n)
                                    n = rec.amount[i];
                                sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                rec.amount[i] -= n;
                                if (!rec.amount[i])
                                    rec.item[i] = 0;
                                stA.item[j] = rec.item[i];
                                stA.amount[j] += n;
                                found = 1;
                            }
                        }
                        for (j = 0; !found && j < stA.n; j++) {
                            if (found)
                                continue;
                            for (m = 0; !found && m < 128; m++) {
                                if (ai_Slots[m].item == stA.item[j]
                                    && (!(ai_Slots[m].flags & 2) || ai_Slots[m].rank > s->rank)) {
                                    sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf aus Lager in \"%s\" von %li Einheiten %s",
                                            ch->bld->name, stA.amount[j], ai_Goods[stA.item[j]].name);
                                    sprintf(buf, SELL_FMT, -1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                            (double)(int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    cm_SendExSellObjectI(-1, b->objId, stA.item[j], stA.amount[j], ai_OwnPlayer,
                                                 (int)gm_GoodsPrice(stA.item[j], ai_OwnPlayer));
                                    ch->budget += gm_GoodsPrice(stA.item[j], ai_OwnPlayer);
                                    stA.item[j] = 0;
                                    stA.amount[j] = 0;
                                    n = gm_ListStock(b, rec.item[i]);
                                    if (rec.amount[i] < n)
                                        n = rec.amount[i];
                                    sprintf(buf, SELL_FMT, b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    cm_SendExSellObjectI(b->objId, market->objId, rec.item[i], n, ai_OwnPlayer, 0);
                                    rec.amount[i] -= n;
                                    if (!rec.amount[i])
                                        rec.item[i] = 0;
                                    stA.item[j] = rec.item[i];
                                    stA.amount[j] += n;
                                    found = 1;
                                }
                            }
                        }
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_Goods[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
                for (i = 0; nsell < 3 && i < stA.n; i++) {
                    it = stA.item[i];
                    if (!it)
                        continue;
                    found = 0;
                    for (j = 0; !found && j < ai_NEntries; j++) {
                        if (it == ai_Entries[j].item)
                            found = 1;
                    }
                    for (j = 0; !found && j < ai_NSlots; j++) {
                        if (it == ai_Slots[j].item)
                            found = 1;
                    }
                    if (!found) {
                        lim = gm_ListStock(market, it);
                        for (j = 0; stA.amount[i] && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == stA.item[i]) {
                                n = stA.amount[i];
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, stA.item[i], n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = stA.item[i];
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                sold = 1;
                                stA.amount[i] -= n;
                                rec.item[j] = stA.item[i];
                            }
                        }
                        if (!stA.amount[i]) {
                            cm_SendRemoveGoodsI(b->objId, stA.item[i]);
                            stA.item[i] = 0;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < stB.n; i++) {
                    it = stB.item[i];
                    if (!it)
                        continue;
                    found = 0;
                    for (j = 0; !found && j < ai_NEntries; j++) {
                        if (it == ai_Entries[j].item)
                            found = 1;
                    }
                    for (j = 0; !found && j < ai_NSlots; j++) {
                        if (it == ai_Slots[j].item)
                            found = 1;
                    }
                    if (!found) {
                        lim = gm_ListStock(market, it);
                        for (j = 0; stB.amount[i] && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == stB.item[i]) {
                                n = stB.amount[i];
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, stB.item[i], n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, stB.item[i], n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = stB.item[i];
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                sold = 1;
                                stB.amount[i] -= n;
                                rec.item[j] = stB.item[i];
                            }
                        }
                        if (!stB.amount[i]) {
                            cm_SendRemoveGoodsI(b->objId, stB.item[i]);
                            stB.item[i] = 0;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NEntries; i++) {
                    if (!ai_Entries[i].sellAmount)
                        continue;
                    lim = gm_ListStock(market, ai_Entries[i].item);
                    for (j = 0; ai_Entries[i].sellAmount && j < rec.n; j++) {
                        if (!rec.item[j] || rec.item[j] == ai_Entries[i].item) {
                            n = ai_Entries[i].sellAmount;
                            if (lim < n)
                                n = lim;
                            sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                            msg.u.item[nsell] = ai_Entries[i].item;
                            msg.dst[nsell] = ai_MarketBuilding->id;
                            nsell++;
                            ai_Entries[i].sellAmount -= n;
                            sold = 1;
                            rec.item[j] = ai_Entries[i].item;
                        }
                    }
                }
                for (i = 0; nsell < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].sellAmount)
                        continue;
                    if (!gm_FindMarketObject(ai_Slots[i].item)) {
                        sprintf(buf, "ai_MeisterTrade(): Fake-Verkauf in \"%s\" von %li Einheiten %s, weil kein Marktlager",
                                ch->bld->name, ai_Slots[i].sellAmount, ai_Goods[ai_Slots[i].item].name);
                        sprintf(buf, SELL_FMT, -1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        cm_SendExSellObjectI(-1, b->objId, ai_Slots[i].item, ai_Slots[i].sellAmount, ai_OwnPlayer,
                                     (int)gm_GoodsPrice(ai_Slots[i].item, ai_OwnPlayer));
                        ai_Slots[i].sellAmount = 0;
                    } else {
                        lim = gm_ListStock(market, ai_Slots[i].item);
                        for (j = 0; ai_Slots[i].sellAmount && j < rec.n; j++) {
                            if (!rec.item[j] || rec.item[j] == ai_Slots[i].item) {
                                n = ai_Slots[i].sellAmount;
                                if (lim < n)
                                    n = lim;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Slots[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nsell] = ai_Slots[i].item;
                                msg.dst[nsell] = ai_MarketBuilding->id;
                                nsell++;
                                ai_Slots[i].sellAmount -= n;
                                sold = 1;
                                rec.item[j] = ai_Slots[i].item;
                            }
                        }
                    }
                }
                for (i = 0; nbuy < 3 && i < ai_NSlots; i++) {
                    if (!ai_Slots[i].make)
                        continue;
                    n = gm_ListStock(market, ai_Slots[i].item);
                    if (ai_Slots[i].bld) {
                        msg.u.item[nbuy + 3] = ai_Slots[i].item;
                        msg.dst[nbuy + 3] = ai_Slots[i].bld->id;
                        msg.amount[nbuy] = min(n, ai_Slots[i].make);
                        nbuy++;
                        ai_Slots[i].make = 0;
                        sold = 1;
                    }
                }
                if (sold) {
                    if (ch == ai_LoggedMaster) {
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k])
                                sprintf(buf, "Verkauf von %s", ai_Goods[msg.u.item[k]].name);
                        }
                        for (k = 0; k < 3; k++) {
                            if (msg.u.item[k + 3])
                                sprintf(buf, "Einkauf von %li Einheiten %s", msg.amount[k],
                                        ai_Goods[msg.u.item[k + 3]].name);
                        }
                    }
                    n = msg.dst[0];
                    if (n == -1)
                        n = msg.dst[3];
                    msg.type = 11;
                    msg.to = ai_Chars[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.time = ai_TimeT;
                    msg.flags = 1;
                    msg.car = car->objId;
                    msg.n = n;
                    msg.ch = ch->netId;
                    if (!ai_TradeMsgOff) {
                        cm_SendLongMessage(&msg);
                        ai_TradeMsgCount++;
                    }
                }
            }
        }
        for (o = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); o != car;
             o = gm_QueryNext())
            ;
    }
    return 1;
}
