/* 0x480e70, the guard master: patrols the town, checks a workshop or patrols the surroundings @deep B3 far draft (frame 0x508 vs the original 0x50c; diffclass 886 code / 32 regs / 20 order / 14 slots): the body was read against build/ghidra/480e70.c and tools/asmdiff - same statements, constants, callees and field offsets; the diff is register allocation (ours ch=ebp, original ch=edi), the slot rotation and the schedule it causes, plus one 4-byte frame difference with no missing / extra source statement found in the read (the original's larger frame comes out of its allocation, not out of a statement ours lacks). spell.py on this function: no meaning-class lead (wrong type / signedness / offset / width) in the reload / dup-tail entries; they describe that same allocation. Readability advanced: ai_Unk486190 -> ai_FillSlotTable, ai_Unk487780 -> ai_AddEquipmentNeeds, ai_Unk489300 -> ai_CheckFreeThieves, ai_Unk489660 -> ai_SendFreeThieves, ai_Unk501190 -> gm_BuildQuery, ai_Unk501030 -> gm_BuildNext, ai_Unk502020 -> gm_FindBuildingById, ai_Unk50c520 -> gm_Opinion, ai_Unk501ce0 -> gm_GetGebaeudeHauptraum, ai_Unk625720 -> ai_AmbushSpots. Q2 evidence gaps left: ord_GetGridCell is ord.c's ord_GetGridCell (the role - the grid cell of a scene object - is read, but the unit has no name, @named_by guess, so no prefix can be chosen here), AIMSG.unk89 (written util_Random(4) + 2 only; no reader names it), AIBLD.unk39 (a building amount used as the purchase limit and scaled by 1/64000 in ai.c, compared against the capacity limits in am.c; no source names it). */

void ai_CalcMeisterWache(AICHAR *ch)
{
    AIOBJ *b, *o;
    AIBLD *sel, *bl;
    AIGRID *best, *cell, *row;
    AIMSG msg;
    char buf[1024];
    char *name;
    int gx, gy;
    int i, j, k, c, cnt, n, m, total, idle, half, r1, r2;
    int r;
    char q;

    b = gm_Query(ch->bld->objList, 2, 6, 0, 0x2a);
    if (b) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, b);
        ai_AddEquipmentNeeds(ch, b, ch->bld->unk39);
        ai_MeisterBewaffnen(ch, b);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, b);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(b, ch, 10);
    ai_BuyCapacity(b, ch, 10);
    ai_BuyUpgrade(ch);
    ai_CheckFreeThieves(ch);
    ai_SendFreeThieves(ch);
    if (ai_TimeT.hour <= 6 || (ch->netId + ai_TimeT.hour) % 3 != 0) {
        ch->flags1b4 &= ~0x20;
        return;
    }
    if (ch->flags1b4 & 0x20)
        return;
    total = 0;
    idle = 0;
    ch->flags1b4 |= 0x20;
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].health > 0 && ai_Chars[i].id != 0xffff && ai_Chars[i].kind != 10 && ai_Chars[i].bld == ch->bld
            && ai_Chars[i].title && ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
            total++;
            if (!ai_Chars[i].job)
                idle++;
        }
    }
    if (ai_Watch.bld == ch->bld) {
        ai_Watch.n = idle;
        if (ch->target != -1)
            ai_Watch.target = gm_FindBuildingById(ch->target);
    }
    half = total >> 1;
    if (total - half <= half)
        half = total - half;
    if (idle < half) {
        sprintf(buf, "ai_CalcMeisterWache(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    if (ai_Chars[ch->bld->owner].kind == 6 || ai_Chars[ch->bld->owner].kind == 7) {
        r = util_Random(3);
        if (r == 0 || ch->target == -1 || util_Random(2) == 0)
            goto choose;
        goto city;
    } else {
        if (util_RandomF() <= 0.85 || idle < 3 || ai_TimeT.year < 7 - ai_Difficulty) {
            r = util_Random(3);
choose:
            if (r == 4)
                goto guard;
            if (r == 0)
                goto city;
            if (r == 1)
                goto check;
            if (r < 2)
                return;
        } else {
guard:
            if (ai_CalcAngriff(ch, idle) == 0)
                goto city;
        }
    }
    goto umland;

city:
    sprintf(buf, "ai_CalcMeisterWache(): Meister %s m\xf6" "chte in der Stadt patroullieren", ch->name);
    gx = 8;
    gy = 8;
    best = &ai_Grid[0][0];
    for (row = &ai_Grid[0][0]; row < &ai_Grid[1][0]; row++) {
        cell = row;
        for (j = 8; j; j--) {
            if (!best->used || (cell->used && (best->a >> 3) + best->b < (cell->a >> 3) + cell->b))
                best = cell;
            cell += 8;
        }
    }
    sel = 0;
    bl = gm_BuildQuery(1, 6);
    if (bl) {
        do {
            if (bl->object && ord_GetGridCell(bl->object + 0x4c, &gx, &gy) && best == &ai_Grid[gx][gy]) {
                if (!sel) {
                    sel = bl;
                } else if (bl->owner != 0xffff) {
                    r1 = gm_Opinion(ch->id, sel->owner);
                    r2 = gm_Opinion(ch->id, bl->owner);
                    if (r1 < r2)
                        sel = bl;
                }
            }
            bl = gm_BuildNext();
        } while (bl);
        if (sel) {
            sprintf(buf, "ai_CalcMeisterWache(): Meister %s patroulliert bei '%s'", ch->name, sel->name);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x43;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_TimeT;
            msg.flags = 2;
            msg.arg[0] = sel->id;
            msg.arg[1] = ch->bld->id;
            msg.unk89 = util_Random(4) + 2;
            n = idle;
            if (n > 3)
                n = 4;
            k = 0;
            if (0 < n) {
                for (c = 0; k < n; c++) {
                    if (c >= 768)
                        break;
                    if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title
                        && !ai_Chars[c].job && ai_Chars[c].model && ai_Chars[c].model->bldId == ai_Chars[c].bld->id)
                        msg.arg[6 + k++] = ai_Chars[c].netId;
                }
                if (k > 3)
                    goto send;
            }
            for (; k < 4; k++)
                msg.arg[6 + k] = -1;
send:
            if (msg.arg[6] == -1)
                return;
            cm_SendLongMessage(&msg);
            return;
        }
    }
    goto umland;

check:
    name = ch->name;
    sprintf(buf, "ai_CalcMeisterWache(): Meister %s m\xf6" "chte Betrieb pr\xfc" "fen", name);
    gx = 8;
    gy = 8;
    best = &ai_Grid[0][0];
    for (row = &ai_Grid[0][0]; row < &ai_Grid[1][0]; row++) {
        cell = row;
        for (j = 8; j; j--) {
            if (!best->used || (cell->used && (best->a >> 3) + best->b < (cell->a >> 3) + cell->b))
                best = cell;
            cell += 8;
        }
    }
    sel = 0;
    bl = gm_BuildQuery(1, 6);
    if (bl) {
        do {
            if (bl->object && ord_GetGridCell(bl->object + 0x4c, &gx, &gy) && best == &ai_Grid[gx][gy]) {
                if (!sel) {
                    sel = bl;
                } else if (bl->owner != 0xffff && !(bl->flags5a & 1)) {
                    r1 = gm_Opinion(ch->id, sel->owner);
                    r2 = gm_Opinion(ch->id, bl->owner);
                    if (r2 < r1)
                        sel = bl;
                }
            }
            bl = gm_BuildNext();
        } while (bl);
        if (sel) {
            sprintf(buf, "ai_CalcMeisterWache(): Meister %s \xfc" "berpr\xfc" "ft '%s'", name, sel->name);
            memset(&msg, 0, sizeof(msg));
            msg.type = 100;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_TimeT;
            msg.flags = 1;
            msg.arg[1] = sel->id;
            msg.arg[2] = ch->bld->id;
            o = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(ch->bld);
            if (!o)
                goto failed;
            msg.arg[3] = o->objId;
            msg.arg[6] = -1;
            msg.arg[0] = -1;
            if (name)
                return;
            k = 0;
            for (c = 0; k == 0; c++) {
                if (c >= 768)
                    break;
                if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title
                    && !ai_Chars[c].job && ai_Chars[c].model && ai_Chars[c].model->bldId == ai_Chars[c].bld->id) {
                    msg.arg[6] = ai_Chars[c].netId;
                    k++;
                    msg.arg[0] = msg.arg[6];
                }
            }
            goto send2;
        }
    }
    goto umland;
send2:
    if (msg.arg[6] == -1)
        return;
    cm_SendLongMessage(&msg);
    return;

umland:
    sprintf(buf, "ai_CalcMeisterWache(): Meister %s patroulliert im Umland", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x65;
    msg.to = ai_Chars[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_TimeT;
    msg.flags = 2;
    r = util_Random(8);
    strcpy((char *)&msg.arg[0], ai_AmbushSpots[r]);
    msg.arg[8] = ch->bld->id;
    o = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(ch->bld);
    if (!o) {
failed:
        sprintf(buf, "ai_CalcMeisterWache() : gm_GetGebaeudeHauptraum() failed !!!");
        return;
    }
    msg.arg[9] = o->objId;
    n = idle;
    if (n > 5)
        n = 6;
    cnt = 0;
    k = 0;
    if (0 < n) {
        for (c = 0; cnt < n; c++) {
            if (c >= 768)
                break;
            if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title && !ai_Chars[c].job
                && ai_Chars[c].model && ai_Chars[c].model->bldId == ai_Chars[c].bld->id) {
                msg.ids[k++] = ai_Chars[c].netId;
                cnt++;
            }
        }
        if (k > 5)
            goto send3;
    }
    for (; k < 6; k++)
        msg.ids[k] = -1;
send3:
    if (msg.ids[0] != -1 && 1 < cnt)
        cm_SendLongMessage(&msg);
}
