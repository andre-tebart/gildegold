/* 0x481ef0 @deep B3 far draft (3261/4144 bytes; diffclass 1453 code / 96 slots / 52 regs): every hunk of tools/asmdiff (1538 lines) was read against build/ghidra/481ef0.c - no missing / extra statement, constant, callee or field offset; the diff is the ebp<->edi swap, the stack slot rotation, the block emission order (ours puts the 0x482674 block where the original has its 0x4827e7 copy) and equivalent compare spellings (cmp 0x8 / jae vs 0x7 / ja, cmp 0x5e / ja vs 0x5f / jae); the fpu instructions are identical. Allocation / scheduling = match-deep. Readability this session: ai_Unk4fea50 -> gm_FindById, ai_Unk502520 -> gm_IsHomeType (both with their f3gm.c definitions). Q2 gaps left: ord_GetGridCell (ord.c's ord_GetGridCell; the unit has no name, @named_by guess), gm_BuildingClass (the building-category mapper: no source, no Ghidra export, its defining unit is undrafted), he_Unk553cc0 (he.c: 1 unless an event 0x62 with +0xb0 = the id and +0xb8 == 1 runs; no role name in the sources), AIOBJ.unk06 / unk2a / unk36 / unk37, AIBLD.unk39 / unk65, AIJOB.unkac (a union id: a building id here, a netId in am.c). */

void ai_CalcMeisterDiebe(AICHAR *ch)
{
    AIOBJ *o, *obj;
    AIBLD *bb, *best;
    AIJOB *job;
    unsigned short *item;
    char *name;
    int i, j, k, c, n, all, half, r, t;
    float max, v, w, bestw;
    AIMSG msg;
    char buf[1024];

    obj = 0;
    o = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
    for (; !obj && o; o = gm_QueryNext()) {
        if (gm_FindById(0, &item, 0, o->unk06) && ai_Goods[*item].type == 2)
            obj = o;
    }
    if (obj) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, obj);
        ai_AddEquipmentNeeds(ch, obj, ch->bld->unk39);
        ai_MeisterBewaffnen(ch, obj);
        if (ai_Watch.bld == ch->bld) {
            for (c = 0; ai_Watch.nslots < 4; c++) {
                if (c >= ai_NSlots)
                    break;
                ai_Watch.slots[ai_Watch.nslots] = ai_Slots[c];
                ai_Watch.nslots++;
            }
        }
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, obj);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(obj, ch, 10);
    ai_BuyCapacity(obj, ch, 10);
    ai_BuyUpgrade(ch);
    if (gm_Query(ch->bld->objList, 2, 6, 0, 0x60))
        ai_CheckFreeThieves(ch);
    ai_SendFreeThieves(ch);
    if (ai_Time.hour <= 6 || (ch->netId + ai_Time.hour) % 3 != 0) {
        ch->flags1b4 &= ~0x20;
        return;
    }
    if (ch->flags1b4 & 0x20)
        return;
    ch->flags1b4 |= 0x20;
    all = 0;
    n = 0;
    for (c = 0; c < 768; c++) {
        if (ai_Chars[c].health > 0 && ai_Chars[c].id != 0xffff && ai_Chars[c].kind != 10
            && ai_Chars[c].bld == ch->bld && ai_Chars[c].title && ai_Chars[c].model
            && ai_Chars[c].model->bldId == ai_Chars[c].bld->id) {
            all++;
            if (!ai_Chars[c].job)
                n++;
        }
    }
    if (ai_Watch.bld == ch->bld) {
        ai_Watch.n = n;
        if (ch->target != -1)
            ai_Watch.target = gm_FindBuildingById(ch->target);
    }
    half = all >> 1;
    if (half >= all - half)
        half = all - half;
    if (n < half) {
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    if (ai_Chars[ch->bld->owner].kind == 6 || ai_Chars[ch->bld->owner].kind == 7) {
        r = util_Random(2);
        if (r && ch->target != -1 && util_Random(2))
            goto burgle;
    } else {
        if (util_RandomF() > 0.85 && n >= 3 && ai_Time.year >= 7 - ai_Difficulty)
            goto other;
        r = util_Random(3);
    }
    if (r == 1) {
        if (!(util_RandomF() < 0.95)) {
            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte entf\xfc" "hren", ch->name);
            max = 0.0f;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                    if (!(max > v))
                        max = v;
                }
            }
            if (max == 0.0f)
                max = 1.0f;
            best = 0;
            bb = ai_Blds;
            for (t = 0; t < 256; t++, bb++) {
                if (bb->type) {
                    r = gm_BuildingClass(bb->type);
                    if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1) && r != 3 && r != 5
                        && r != 0
                        && (!ai_Chars[bb->owner].bld || ai_Chars[bb->owner].bld->owner != ch->bld->owner)
                        && ai_Chars[bb->owner].kind < 8) {
                        if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                            w = 0.0f;
                        else
                            w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                        w = util_Random(100) * w * 0.01;
                        if (best) {
                            if (!((float)gm_GetGoodsValue(&ai_Chars[bb->owner]) * w > (float)gm_GetGoodsValue(&ai_Chars[best->owner]) * bestw))
                                continue;
                        }
                        bestw = w;
                        best = bb;
                    }
                }
            }
            if (gm_Query(ch->bld->objList, 1, 0, 0x65) && best && ch->bld->unk65 == -1) {
                if (he_FindEvent(2, 0, 0x3c, 3, ch->bld->id))
                    return;
                sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s entf\xfc" "hrt %s aus '%s'", ch->name,
                        ai_Chars[best->owner].name, best->name);
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x3c;
                msg.to = ai_Chars[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.time = ai_Time;
                msg.flags = 1;
                msg.arg[0] = ai_Chars[best->owner].netId;
                msg.arg[1] = best->id;
                msg.arg[2] = ch->bld->id;
                if (n >= util_Random(3) + 1)
                    n = util_Random(3) + 1;
                AI_FILL_IDS(n)
                if (msg.ids[0] != -1) {
                    cm_SendLongMessage(&msg);
                    ch->target = -1;
                }
                return;
            }
        }
    } else if (r == 0) {
burgle:
        name = ch->name;
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte einbrechen", name);
        if (ch->target != -1) {
            best = gm_FindBuildingById(ch->target);
            if (!best)
                ch->target = -1;
            else if (best->owner == ch->bld->owner)
                ch->target = -1;
        }
        if (ch->target == -1) {
            max = 0.0f;
            for (j = 0; j < 8; j++) {
                for (i = 0; i < 8; i++) {
                    v = ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b;
                    if (!(max > v))
                        max = v;
                }
            }
            if (max == 0.0f)
                max = 1.0f;
            best = 0;
            bb = ai_Blds;
            for (t = 0; t < 256; t++, bb++) {
                if (bb->type && bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                    && bb->owner != *ai_PlayerChar && !gm_IsHomeType(bb) && ai_BTypes[bb->type].kind != '!'
                    && he_Unk553cc0(bb)) {
                    if (!bb->object || !ord_GetGridCell(bb->object + 0x4c, &i, &j))
                        w = 0.0f;
                    else
                        w = 1.0f - (ai_Grid[i][j].a * 0.125f + ai_Grid[i][j].b) / max;
                    r = gm_BuildingRank(bb);
                    w = (8.0f - r) * w * 0.5;
                    w = util_Random(100) * w * 0.01;
                    if (best) {
                        if (!((float)gm_OwnerWorth(bb, obj != 0) * w > (float)gm_OwnerWorth(best, obj != 0) * bestw))
                            continue;
                    }
                    bestw = w;
                    best = bb;
                }
            }
            if (!best) {
                ch->target = -1;
                goto pick;
            }
            ch->target = best->id;
        }
        if (!best)
            goto pick;
        for (o = gm_Query(ch->bld->objList, 1, 0, 0xca); o; o = gm_QueryNext()) {
            if (o->unk2a == best->id) {
                if (o->unk37 > 0x5e || (float)o->unk36 + (float)o->unk36 + (float)util_Random(20) <= (float)o->unk37) {
                    for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent()) {
                        if (job->unkac == best->id) {
                            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat aber noch eine Spionage bei '%s' "
                                    "nicht beendet", name, best->name);
                            return;
                        }
                    }
                    for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent()) {
                        if (job->unkac == best->id) {
                            sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat aber noch eine Spionage bei '%s' "
                                    "nicht beendet", name, best->name);
                            return;
                        }
                    }
                    sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s bricht bei '%s' ein", name, best->name);
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x3f;
                    msg.to = ai_Chars[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.time = ai_Time;
                    msg.flags = 1;
                    msg.arg[0] = best->id;
                    msg.arg[1] = ch->bld->id;
                    if (n > 7)
                        n = 8;
                    AI_FILL_IDS(n)
                    if (msg.ids[0] != -1) {
                        cm_SendLongMessage(&msg);
                        ch->target = -1;
                    }
                    return;
                }
                break;
            }
        }
        for (job = he_FindEvent(2, 0, 0x40, 3, ch->bld->id); job; job = he_NextEvent()) {
            if (job->unkac == best->id) {
                sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat schon eine Spionage bei '%s' laufen", name,
                        best->name);
                return;
            }
        }
        for (job = he_FindEvent(2, 0, 0x3f, 3, ch->bld->id); job; job = he_NextEvent()) {
            if (job->unkac == best->id) {
                sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s hat schon eine Spionage bei '%s' laufen", name,
                        best->name);
                return;
            }
        }
        sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s spioniert bei '%s'", name, best->name);
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x40;
        msg.to = ai_Chars[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[0] = best->id;
        msg.arg[1] = ch->bld->id;
        if (n > 7)
            n = 8;
        AI_FILL_IDS(n)
        cm_SendLongMessage(&msg);
        return;
    } else if (r == 3) {
other:
        ai_CalcAngriff(ch, n);
    } else if (r <= 1)
        return;
pick:
    sprintf(buf, "ai_CalcMeisterDiebe(): Meister %s m\xf6" "chte Taschen dieben", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x61;
    msg.to = ai_Chars[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 1;
    msg.arg[10] = ai_Chars[ch->bld->owner].netId;
    if (n > 7)
        n = 8;
    AI_FILL_IDS(n)
    if (msg.ids[0] != -1)
        cm_SendLongMessage(&msg);
}
