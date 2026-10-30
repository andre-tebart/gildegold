/* 0x48d010, ai_MeisterTradeFernEinkauf: the long-distance buyer sends every cart that is in the world to a
   market store (unloads it, then orders the purchases of the slots / entries)
   @deep behaviour draft: 547 code + 32 slots hunks (1460 of 2064 bytes). The diff was read with tools/asmdiff.py: the whole delta is the frame (sub esp,0x69c vs the original 0x694 = one local too many) and the stack-slot rotation it causes - the spell.py 'code (no menu rule)' hunks are the same instructions at shifted slots (48d170 BYTE [esp+0xdd] vs [esp+0xd5], 48d24f [esp+edi*4+0x1a0] vs +0x198, 48d30a +0x1cc vs +0x1c4), no missing / extra statement, constant or callee. Lead for match-deep: merge two locals that never overlap to shrink the frame by 8. Q2 evidence gaps: ai_TradeMsgOff (a gate global, no writer), ai_Carts / ai_NumCarts (ledger globals, roles documented in the ai.c header comment but no source names them), AISLOT.unk20 (the buy sort key, no name in any source), AIENTRY.unk4c (how many to buy, the slot's unk34 is already buyAmount - a shared-header decision), AIMSG.u.s.unk84 / AIMSG.unk9d (written only). Q4: the two raw accesses *(int *)(bl + 1) and *(int *)(bl + 0x65) - bl is a char * view of an AIBLD record; AIBLD.id covers the first, the second is AIBLD.unk65 which has no evidenced role (a work record pointer in gv.c / f3gm.c, a number in ai_CalcMeisterBank). */

int ai_MeisterTradeFernEinkauf(AICHAR *ch, AIOBJ *b)
{
    AIMSG msg;
    AIRECIPE stA, rec;
    AIOBJ *car, *market;
    AISLOT *hit, *sp;
    char *bl;
    AISLOT *sl;
    char buf[1024];
    int exp[4];
    int k, i, m, n, nA, nB, amt, d, v;

    nA = 0;
    nB = 0;
    if (ai_Slots[0].saving <= 0 || ai_Slots[0].buyAmount == 0)
        return 0;
    gm_BuildingGoodsList(ch->bld, &stA, 0);
    for (k = 0; k < ai_NumCarts; k++) {
        if (nB > 3)
            __asm int 3
        car = ai_Carts[k];
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
                        cm_SendExSellObjectI(-1, market->objId, rec.item[i], rec.amount[i], ai_OwnPlayer,
                                     (int)gm_GoodsPrice(rec.item[i], ai_OwnPlayer));
                        ch->budget += gm_GoodsPrice(rec.item[i], ai_OwnPlayer);
                        rec.item[i] = 0;
                        rec.amount[i] = 0;
                    }
                }
            }
            exp[0] = 0;
            exp[1] = 0;
            exp[2] = 0;
            nB = 0;
            exp[3] = 0;
            i = 0;
            if (ai_Slots[0].item) {
                amt = gm_ListStock(market, ai_Slots[0].item);
                msg.u.item[3] = ai_Slots[0].item;
                msg.dst[3] = ai_Slots[0].bld->id;
                if (amt >= ai_Slots[0].buyAmount)
                    amt = ai_Slots[0].buyAmount;
                msg.amount[0] = amt;
                nB = 1;
                i = 1;
            }
            sl = &ai_Slots[i];
            do {
                if (i >= ai_NSlots || !(sl->saving <= 0))
                    break;
                if (msg.dst[3] == sl->bld->id && sl->buyAmount) {
                    amt = gm_ListStock(market, sl->item);
                    bl = (char *)sl->bld;
                    if (bl) {
                        msg.u.item[nB + 3] = sl->item;
                        msg.dst[nB + 3] = *(int *)(bl + 1);
                        if (amt >= sl->buyAmount)
                            amt = sl->buyAmount;
                        msg.amount[nB] = amt;
                        if (*bl == 'G')
                            exp[*(int *)(bl + 0x65)] = (int)(amt * sl->price + exp[*(int *)(bl + 0x65)]);
                        nB++;
                    }
                }
                i++;
                sl++;
            } while (nB < 3);
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
            i = 0;
            if (nA < 3) {
                for (; i < ai_NEntries && ai_Entries[i].margin <= 0; i++) {
                    if (msg.dst[3] == ai_Entries[i].bld->id && ai_Entries[i].buyAmount) {
                        amt = gm_ListStock(market, ai_Entries[i].item);
                        for (m = 0; m < rec.n; m++) {
                            if (!rec.item[m]) {
                                n = amt;
                                if (ai_Entries[i].buyAmount < n)
                                    n = ai_Entries[i].buyAmount;
                                sprintf(buf, SELL_FMT, market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                n = ai_Entries[i].buyAmount;
                                if (amt < n)
                                    n = amt;
                                cm_SendExSellObjectI(market->objId, b->objId, ai_Entries[i].item, n, ai_OwnPlayer, 0);
                                msg.u.item[nA] = ai_Entries[i].item;
                                msg.dst[nA] = ai_Entries[i].bld->id;
                                nA++;
                                rec.item[m] = ai_Entries[i].item;
                            }
                        }
                    }
                    if (nA >= 3)
                        break;
                }
            }
            n = msg.dst[0];
            if (n == -1)
                n = msg.dst[3];
            msg.type = 0xb;
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
    return 1;
}
