/* 0x487fb0, buys a renovation of the building or an upgrade of the workshop @deep behaviour draft, now near: 52 code / 10 regs / 6 order / 6 fill hunks = 48 differing instructions of 361 (baseline 99). This session's fix: the two slot loops were a do-while over a hand-walked p with a separate i - VC6 spilled BOTH (frame 0x200 vs 0x1fc) and the loop counter went to the frame; rewritten as for (i = 0; i < bt->nslots; i++) with bt->slot[i] (index only) the frame matched and 99 -> 48 differing instructions in one edit (code 151 -> 52, every slots hunk gone). Tried and reverted / identical, one edit one build each: all-c->bld (81 code hunks, worse), all-bld (377, worse), int n (identical), dropping the redundant AICHAR *c = (AICHAR *)ch alias (identical, the cleaner ch form kept). What is left, all read against tools/disasm.py 0x487fb0 0x4c0 and build/ghidra/487fb0.c: the register pair of the building pointer (original edi, ours ebp; knock-on hunks at 0x487fc0 zero register, 0x488060 the lim registers, 0x4881c3 test vs cmp against the zero register, 0x488217 the re-zero, 0x48824c / 0x4882e3 the ai_Goods[n] address form), the store order at 0x488101 and the 3-instruction dup-tail at 0x4884aa; every statement, constant, callee, field offset and the float block (0.005 * 0.005, max 128, budget * 0.75) match Ghidra. Readability done this session: Q2 gm_PlayerRecord -> gm_PlayerRecord (unified with the f3gm.c definition), gm_BuildingLevel -> gm_BuildingLevel (f3gm.c definition renamed too; it returns the building's level byte +0x5c), gm_EquipmentValue -> gm_EquipmentValue (the f3gm.c gm_EquipmentValue definition and every ai_ alias), AIBLD.unk25 -> masterId (am.c 0x497cf0 looks the AI's buildings up by it, the upgrade message goes to that character), AIBLD.level -> level (0..100, renovation threshold, 100 - level in the renovation message), AIFIN.unk40 / unk44 -> upgradeSpent / renovationSpent; Q4 *(unsigned short *)&msg2.arg[0] = ret -> the new AIMSG.itemId union view; Q6 already there */

void ai_BuyUpgrade(AICHAR *ch)
{
    AIBLD *bld;
    AIBTYPE *bt;
    AIOBJ *o;
    AIFIN *fin;
    AIMSG msg, msg2;
    unsigned short *p;
    float f;
    unsigned int flags;
    int i, cost, ret;
    unsigned int n;
    unsigned short rv;
    int lim;
    int first;

    i = 0;
    ret = 0;
    first = 0xfd;
    if (ch->flags1c8 & 8)
        return;
    if (ai_Time.hour != 7 && ai_Time.hour != 0xc && ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x400;
        return;
    }
    flags = ch->flags1b4;
    if (flags & 0x400)
        return;
    bld = ch->bld;
    flags |= 0x400;
    ch->flags1b4 = flags;
    if (ai_Chars[bld->owner].kind != 6 && ai_Chars[bld->owner].kind != 7)
        return;
    if (!(flags & 8))
        return;
    rv = util_Random(0x14);
    lim = rv + 0x4b;
    if (gm_BuildingLevel((AIBLD *)bld) < lim) {
        f = (float)gm_EquipmentValue(ch->bld, 100) * 0.005f;
        f = f * 0.005f;
        if (128.0f > f)
            f = 128.0f;
        if ((float)ch->budget * 0.75f > f) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x1c;
            msg.fromId = -1;
            msg.flags = 1;
            msg.to = ai_Chars[ch->bld->owner].netId;
            msg.arg[0] = ch->bld->id;
            msg.arg[1] = 100 - ch->bld->level;
            cm_BeginGroup("renovieren");
            cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)f);
            cm_SendLongMessage(&msg);
            cm_EndGroup("renovieren");
            fin = gm_PlayerRecord(&ai_Chars[ch->bld->owner]);
            if (fin)
                fin->renovationSpent = (int)(fin->renovationSpent + f);
            ch->budget = (int)(ch->budget - f);
        }
    }
    bt = &ai_BTypes[ch->bld->type];
    if (bt->nslots > 0) {
        for (i = 0; i < bt->nslots; i++) {
            if (ret)
                goto found;
            if ((bt->slot[i] & 0x7fff) != 0) {
                n = bt->slot[i] & 0x7fff;
                if (ai_Goods[n].type == 2 || ai_Goods[n].type == 6)
                    first = n;
                else if (first != 0xfd && !gm_Query(ch->bld->objList, 2, 6, 0, n))
                    ret = bt->slot[i] & 0x7fff;
            }
        }
        if (ret)
            goto found;
    }
    i = 0;
    first = 0;
    if (bt->nslots <= 0)
        return;
    for (i = 0; i < bt->nslots; i++) {
        if (ret)
            goto found;
        if ((bt->slot[i] & 0x7fff) != 0) {
            n = bt->slot[i] & 0x7fff;
            if (ai_Goods[n].type == 2 || ai_Goods[n].type == 6)
                first = n;
            else if (first == 0xfd && !gm_Query(ch->bld->objList, 2, 6, 0, n))
                ret = bt->slot[i] & 0x7fff;
        }
    }
    if (!ret)
        return;
found:
    o = gm_Query(ch->bld->objList, 1, 0, first);
    if (o) {
        cost = (int)gm_ItemPrice(ret, 100);
        if (ch->budget > cost * 3) {
            memset(&msg2, 0, sizeof(msg2));
            msg2.type = 2;
            msg2.to = ai_Chars[ch->bld->masterId].netId;
            msg2.fromId = ch->bld->id;
            msg2.time = ai_Time;
            msg2.flags = 1;
            msg2.itemId = ret;
            msg2.arg[1] = o->objId;
            msg2.arg[3] = 1;
            fin = gm_PlayerRecord(&ai_Chars[ch->bld->masterId]);
            if (fin)
                fin->upgradeSpent += cost;
            cm_BeginGroup("upgrade_meister_ai");
            cm_SendMoveGoods(-1, ai_Chars[ch->bld->masterId].netId, ai_OwnPlayer, cost);
            cm_SendLongMessage(&msg2);
            cm_EndGroup("upgrade_meister_ai");
            ch->budget -= cost;
        }
    }
    ch->flags1c8 |= 8;
}
