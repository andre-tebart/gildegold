/* 0x48d810, ai_MeisterTradeFernEinkauf, the variant that sells entry 0 first (the entries are searched before
   the slots): the kind-9 master's long-distance purchase - for every cart in ai_Carts, a free one (fill < 0x21)
   gets the unload order (message 0x10); a loaded one first unloads its goods at the market (a fake sale at the
   market's price credits the budget), then buys entry 0 and up to two more entries whose building is dst[3],
   then the slots (up to three buys per player, the expected value exp[] per player from the banks of the type
   'G'), and sends the trade message 0xb (unless ai_TradeMsgOff)
   @deep behaviour: 610/611 insns, 69 differing, 6 code hunks, all allocation / scheduling - 48db76 + 48dbc2 (the original keeps b in edi and amt in esi, ours market in esi / amt in edi, b->objId reloaded from [ebp+0xc]), 48dd6f (the original loads nB into eax before the exp[] / i zero stores and zeroes from edx, ours zeroes from eax and loads nB after), 48ddbc (eax / ecx roles), 48de19 (the original cmp BYTE PTR [ecx],0x47, ours mov al,[ecx]; cmp al,0x47) plus an extra amt home store at the gm_ListStock result in ours, 48e06d (3 nop fill). Every hunk read against asmdiff / disasm.py: no missing / extra statement, constant, callee or field offset. spell.py: dup-tail 48db76 (no if/else at that hunk - the sides differ by b vs market in a register), reload 48dd6f (both sides load nB, only the schedule differs), code 48de19 / 48e06d (no rule). One edit one build each: bl as AIBLD* with bl->id / bl->type / bl->unk65 instead of the char* view - byte-identical, kept (Q4); buf[1028] (Ghidra local_408) frame 0x69c / 71 differing, reverted; reusing d for amt in the slots block - byte-identical, reverted. Q2 left: AIBLD.unk65 (a player slot 0..4 in the shop / bank uses - gm_GoodsPrice's p, this function's exp[] per player - and a char* work record for a kind-0x1d building, gv_EntryIndex's r; a union playerSlot / work compiled byte-identical here but flipped ai_Unk483a60 (byte) to 5 differing bytes - the header tie-break - reverted), AIMSG.u.s.unk84 and AIMSG.unk9d (written only, no reader in any source). */

int ai_MeisterTradeFernEinkauf2(AICHAR *ch, AIOBJ *b)
{
    AIMSG msg;
    AIRECIPE stA, rec;
    AIOBJ *car, *market;
    AISLOT *hit, *sp;
    AIBLD *bl;
    AISLOT *sl;
    char buf[1024];
    int exp[4];
    int k, i, m, n, nA, nB, amt, d, v;

    nA = 0;
    nB = 0;
    if (ai_Entries[0].margin <= 0 || ai_Entries[0].buyAmount == 0)
        return 0;
    gm_BuildingGoodsList(ch->bld, &stA, 0);
    for (k = 0; k < ai_NumCarts; k++) {
        car = ai_Carts[k];
        if (car->fill < 0x21) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x10;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.flags = 1;
            msg.time = ai_Time;
            gm_AddTime(&msg.time, 0, 0, 10);
            msg.car = car->objId;
            msg.dst[0] = 5;
            msg.u.s.unk84 = 0;
            msg.n = 100 - car->fill;
            cm_SendLongMessage(&msg);
        } else {
            memset(&msg, 0, sizeof(msg));
            if (ai_WorkLoad() > ai_WorkLimit())
                msg.unk9d = 1;
            for (i = 0; i < 3; i++) {
                msg.u.item[i] = 0;
                msg.dst[i] = -1;
                msg.u.item[i + 3] = 0;
                msg.dst[i + 3] = -1;
            }
            gm_StallGoodsList(car, &rec);
            market = gm_Query(car->list, 1, 0, 0x1dd);
            for (i = 0; i < rec.n; i++) {
                if (rec.item[i]) {
                    sprintf(buf, "ai_MeisterTradeFernEinkauf(): Von Auto in \"%s\" %li Einheiten %s entladen",
                            ch->bld->name, rec.amount[i], ai_Goods[rec.item[i]].name);
                    hit = NULL;
                    sp = ai_Slots;
                    while (!hit) {
                        if ((int)sp >= (int)&ai_Slots[128])
                            break;
                        if (sp->item == rec.item[i])
                            hit = sp;
                        sp++;
                    }
                    if (rec.amount[i]) {
                        sprintf(buf, "ai_MeisterTradeFernEinkauf(): Fake-Verkauf aus Auto in \"%s\" von %li Einheiten %s",
                                ch->bld->name, rec.amount[i], ai_Goods[rec.item[i]].name);
                        sprintf(buf, SELL_FMT, -1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                (double)(int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        cm_SendExSellObject(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
            }
            amt = gm_ListStock(market, ai_Entries[0].item);
            n = ai_Entries[0].buyAmount;
            if (amt < n)
                n = amt;
            sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[0].item, n, ai_OwnPlayer, 0);
            if (amt >= ai_Entries[0].buyAmount)
                amt = ai_Entries[0].buyAmount;
            cm_SendExSellObject(market->objId, b->objId, ai_Entries[0].item, amt, ai_OwnPlayer, 0);
            msg.u.item[nA] = ai_Entries[0].item;
            msg.dst[nA] = ai_Entries[0].bld->id;
            nA++;
            rec.item[0] = ai_Entries[0].item;
            i = 1;
            if (nA < 3) {
                do {
                    if (i >= ai_NEntries || !(ai_Entries[i].margin <= 0))
                        break;
                    if (msg.dst[3] == ai_Entries[i].bld->id && ai_Entries[i].buyAmount) {
                        amt = gm_ListStock(market, ai_Entries[i].item);
                        for (m = 0; m < rec.n; m++) {
                            if (!rec.item[m]) {
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                cm_SendExSellObject(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nA] = ai_Entries[i].item;
                                msg.dst[nA] = ai_Entries[i].bld->id;
                                nA++;
                                rec.item[m] = ai_Entries[i].item;
                            }
                        }
                    }
                    i++;
                } while (nA < 3);
            }
            exp[0] = 0;
            exp[1] = 0;
            exp[2] = 0;
            exp[3] = 0;
            i = 0;
            if (nB < 3) {
                sl = ai_Slots;
                do {
                    if (i >= ai_NSlots || !(sl->saving <= 0))
                        break;
                    if (msg.dst[0] == sl->bld->id && sl->buyAmount) {
                        amt = gm_ListStock(market, sl->item);
                        bl = sl->bld;
                        if (bl) {
                            msg.u.item[nB + 3] = sl->item;
                            msg.dst[nB + 3] = bl->id;
                            if (amt >= sl->buyAmount)
                                amt = sl->buyAmount;
                            msg.amount[nB] = amt;
                            if (bl->type == 'G')
                                exp[bl->unk65] = (int)(amt * sl->price + exp[bl->unk65]);
                            nB++;
                        }
                    }
                    i++;
                    sl++;
                } while (nB < 3);
            }
            for (i = 1; i < 4; i++) {
                v = gm_SumSlotAmounts(ai_Chars[ch->bld->owner].inventory, i);
                if (v < exp[i]) {
                    d = exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_Chars[ch->bld->owner].netId, -1, i, d);
                    cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                } else if (v > exp[i] * 2) {
                    d = v - exp[i] * 9 / 8;
                    cm_BeginGroup("Fernh\xe4ndler");
                    cm_SendMoveGoods(ai_Chars[ch->bld->owner].netId, -1, ai_OwnPlayer, d);
                    cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, i, (int)(d * 1.03));
                    cm_EndGroup("Fernh\xe4ndler");
                }
            }
            n = msg.dst[0];
            if (n == -1)
                n = msg.dst[3];
            msg.type = 0xb;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
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
    return 1;
}
