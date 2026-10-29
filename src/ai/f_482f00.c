/* 0x482f00, ai_CalcMeisterRaeuber: the robber master (the Wache message names the guard master's room) @deep B3 draft, 207 code / 12 slots hunks after the strcpy fix (was 321 / 26): every hunk of tools/asmdiff was read against build/ghidra/482f00.c - statements, constants, callees and field offsets match. Last-bytes pass: the manual copy loop `src[i] / ((char *)msg.arg)[i]` was the meaning-class lead - the original's asm is VC6's inlined strcpy (sub ecx,eax / mov dl,[eax] / mov [ecx+eax],dl / inc eax / test dl,dl); replacing it by strcpy((char *)msg.arg, src) dropped the function from 321 code / 26 slots to 207 / 12 and its asm diff from 211 to 139 lines. The same idiom was applied to ai_CalcMeisterWache (byte-neutral there, its pointer-loop form already compiles as the inline copy). What is left is allocation / block order (the ebp/esi roles, [esp+0x1c..0x44] rotation, the best/rank block emitted before vs after the compare) = match-deep. Q2 gaps: ord_GetGridCell (ord.c's ord_GetGridCell; unit unnamed, @named_by guess), AIBLD.unk39 (a building amount used as the purchase limit, scaled by 1/64000 in ai.c - no source names it). */

void ai_CalcMeisterRaeuber(AICHAR *ch)
{
    AIOBJ *obj, *room;
    AIBLD *bb, *best;
    AIBTYPE *bt;
    int i, j, k, c, n, all, half, r, t;
    volatile int z;
    float max, v, w, bestw;
    char *src;
    AIMSG msg;
    char buf[1024];

    z = 0;
    obj = gm_Query(ch->bld->objList, 2, 6, 0, 0x116);
    if (obj) {
        ai_NEntries = 0;
        ai_NSlots = 0;
        ai_FillSlotTable(ch, obj);
        ai_AddEquipmentNeeds(ch, obj, ch->bld->unk39);
        ai_MeisterBewaffnen(ch, obj);
        ai_MeisterCollectTrans(ch);
        ai_MeisterTrade(ch, obj);
    }
    ai_UpdateStaff(ch);
    ai_BuySlots(obj, ch, 10);
    ai_BuyCapacity(obj, ch, 10);
    ai_BuyUpgrade(ch);
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
        sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s hat gerade keine Leute...", ch->name);
        return;
    }
    if (ai_Chars[ch->bld->owner].kind == 6 || ai_Chars[ch->bld->owner].kind == 7) {
        r = util_Random(2);
        if (r && ch->target != -1 && util_Random(2))
            goto lauer;
    } else {
        if (util_RandomF() > 0.85 && n >= 3 && ai_Time.year >= 7 - ai_Difficulty)
            goto attack;
        r = util_Random(4);
    }
    if (r == 3) {
attack:
        ai_CalcAngriff(ch, n);
    } else if (r == 0) {
        if (!(util_RandomF() < 0.75)) {
            sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s m\xf6" "chte Geld sch\xfctzen", ch->name);
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
                    bt = &ai_BTypes[bb->type];
                    if (bb->owner != ch->bld->owner && bb->owner != 0xffff && !(bb->flags5a & 1)
                        && bb->owner != *ai_PlayerChar && !gm_IsHomeType(bb) && bt->kind != '!') {
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
            ch->target = best->id;
            sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s sch\xfctzt Geld bei '%s'", ch->name, best->name);
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x62;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.fromId = ch->bld->id;
            msg.time = ai_Time;
            msg.flags = 2;
            msg.arg[0] = -1;
            msg.arg[1] = best->id;
            msg.arg[4] = ch->bld->id;
            room = ((void *(*)(unsigned char *))gm_GetGebaeudeHauptraum)(ch->bld);
            if (!room) {
                sprintf(buf, "ai_CalcMeisterWache() : gm_GetGebaeudeHauptraum() failed !!!");
                return;
            }
            msg.arg[5] = room->objId;
            k = z;
            for (c = 0; k == 0; c++) {
                if (c >= 768)
                    break;
                if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title
                    && !ai_Chars[c].job && ai_Chars[c].model && ai_Chars[c].model->bldId == ai_Chars[c].bld->id) {
                    msg.arg[0] = ai_Chars[c].netId;
                    k++;
                }
            }
            if (msg.arg[0] == -1)
                return;
            cm_SendLongMessage(&msg);
            return;
        }
    } else if (r < 1)
        return;
lauer:
    sprintf(buf, "ai_CalcMeisterR\xe4uber(): Meister %s m\xf6" "chte sich auf die Lauer legen", ch->name);
    memset(&msg, 0, sizeof(msg));
    msg.type = 0x48;
    msg.to = ai_Chars[ch->bld->owner].netId;
    msg.fromId = ch->bld->id;
    msg.time = ai_Time;
    msg.flags = 2;
    src = ai_AmbushSpots[util_Random(8)];
    strcpy((char *)msg.arg, src);
    if (n > 7)
        n = 8;
    k = 0;
    t = 0;
    for (c = 0; t < n; c++) {
        if (k >= 4 || c >= 768)
            break;
        if (ai_Chars[c].id != 0xffff && ai_Chars[c].bld == ch->bld && ai_Chars[c].title && !ai_Chars[c].job
            && ai_Chars[c].model && ai_Chars[c].model->bldId == ai_Chars[c].bld->id) {
            msg.ids[t++] = ai_Chars[c].netId;
            k++;
        }
    }
    for (i = t; i < 8; i++)
        msg.ids[i] = -1;
    if (msg.ids[0] != -1 && k > 1)
        cm_SendLongMessage(&msg);
}
