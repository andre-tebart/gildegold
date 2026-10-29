/* 0x489cb0, the transport master's collection check, run in every odd hour: it first (the flags1b4 bit 0x80
   guard, set and cleared each call) walks the 0x5008 object records for the goods objects of kind 0x1d that
   belong to this building, counts them (n) and the 0x136 transporters (n136), and for each cart that no job 11
   (building) or 15 (owner) already targets it walks the cart's +6 owner chain (gm_FindById) up to a building;
   a cart that belongs to another building gets the move message 0xf to that building. Then, for a kind-9 master,
   it buys a transporter (0x134 / 0x135 / 0x136) when the counts, the cap247 limit, the budget (twice the item
   price), the 1-in-0x2ee roll and the flags1c8 bit 8 allow it, and sets flags1c8 bit 8; on an even hour the
   flag 0x80 is cleared only.
   @deep behaviour draft: 593 code hunks of 1696 bytes; the diff start was read against tools/asmdiff.py - the statements and constants equal the original (the hour%2 gate, the flags1b4 bit 0x80 set / clear, the 0x5008-record walk with the 0x1d / 0x136 counts, the two he_FindEvent kinds 11 / 15, the +6 owner chain, the message 0xf fields, the kind-9 buy blocks with the 0x2ee roll and the price times 2), and the diff is allocation / scheduling: the zero register (original edi / ebp, ours ebp), the slot rotation (n / n136 at 0x1c / 0x18 vs 0x10 / 0x18) and the cmp cx,bp vs test cx,cx spelling. Q2 evidence gaps: AIOBJ.unk06 (a link id with three meanings in the sources - the owner chain here, a building id in am.c 4254, a good / object id in ai.c 0x481ef0), AIOBJ.unk1c (a building id here, a count / level elsewhere in its union), AIMSG.unka0 (only ever written -1 in the sources). Q4 cleared this session: q is AIOBJ* now (q->unk06), the two job->unkac uses are job->u.id (the union's generic id). Q6 rewritten. */


void ai_MeisterCollectTrans(AICHAR *ch)
{
    AIMSG msg;
    AIOBJ *t;
    AIJOB *job;
    AIBLD *own;
    AIOBJ *q;
    AIBTYPE *bt;
    char buf[1024];
    int i, n, n136, v, item;

    if (ai_Time.hour % 2) {
        if (ch->flags1b4 & 0x80)
            return;
        ch->flags1b4 |= 0x80;
        sprintf(buf, "ai_MeisterCollectTrans(): Check um %li:%li Uhr", ai_Time.hour, ai_Time.min);
        n = 0;
        n136 = 0;
        t = (AIOBJ *)ai_Objects;
        i = 0x5008;
        do {
            if (t->type && ai_Goods[t->type].type == 0x1d && t->unk1c == ch->bld->id) {
                n++;
                if (t->type == 0x136)
                    n136++;
                for (job = he_FindEvent(2, 0, 11, 3, ch->bld->id); job; job = he_NextEvent()) {
                    if (job->u.id == t->objId)
                        goto next;
                }
                for (job = he_FindEvent(2, 0, 15, 2, ch->bld->owner); job; job = he_NextEvent()) {
                    if (job->u.id == t->objId)
                        goto next;
                }
                v = t->unk06;
                own = NULL;
                do {
                    if (v == -1) {
                        if (!own)
                            goto next;
                        break;
                    }
                    if (!gm_FindById((int)&own, (int *)&q, 0, v))
                        v = -1;
                    else if (q)
                        v = q->unk06;
                } while (!own);
                if (own != ch->bld) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0xf;
                    msg.flags = 2;
                    msg.to = ai_Chars[ch->bld->owner].netId;
                    msg.time = ai_Time;
                    msg.car = t->objId;
                    msg.fromId = -1;
                    msg.n = own->id;
                    msg.ch = ch->bld->id;
                    msg.unka0 = -1;
                    cm_SendLongMessage(&msg);
                    sprintf(buf, "ai_MeisterCollectTrans(): Verlorener Transporter wird von %s nach %s geschickt",
                            own->name, ch->bld->name);
                }
            }
        next:
            t++;
        } while (--i);
        bt = &ai_BTypes[ch->bld->type];
        if (bt->kind == 9) {
            if (n == n136) {
                cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x134, 100));
                cm_SendExSellObject(ch->bld->id, -1, 0x134, 1, ai_OwnPlayer, 0);
                sprintf(buf, "ai_MeisterCollectTrans(): Transporter f\xfcr %s gekauft", ch->bld->name);
                return;
            }
            if (n136) {
                if (ch->flags1c8 & 8)
                    return;
                if (n >= bt->cap247)
                    return;
                if (util_Random(0x2ee) >= 2)
                    return;
                item = 0x136 - (util_Random(8) < 4);
                if (ch->budget <= (int)gm_ItemPrice(item, 100) * 2)
                    return;
                cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(item, 100));
                cm_SendExSellObject(ch->bld->id, -1, item, 1, ai_OwnPlayer, 0);
            } else {
                if (ch->flags1c8 & 8)
                    return;
                if (n >= bt->cap247)
                    return;
                if (util_Random(0x2ee) >= 2)
                    return;
                item = 0x136;
                if (ch->budget <= (int)gm_ItemPrice(item, 100) * 2)
                    return;
                cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(item, 100));
                cm_SendExSellObject(ch->bld->id, -1, item, 1, ai_OwnPlayer, 0);
            }
    } else {
        if (n == 0) {
            if ((ai_Chars[ch->bld->owner].kind == 6 || ai_Chars[ch->bld->owner].kind == 7) && !(ch->flags1b4 & 4))
                return;
            cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(0x134, 100));
            cm_SendExSellObject(ch->bld->id, -1, 0x134, 1, ai_OwnPlayer, 0);
            sprintf(buf, "ai_MeisterCollectTrans(): Transporter f\xfcr %s gekauft", ch->bld->name);
            return;
        }
        if ((ai_Chars[ch->bld->owner].kind == 6 || ai_Chars[ch->bld->owner].kind == 7) && !(ch->flags1b4 & 4))
            return;
        if (ch->flags1c8 & 8)
            return;
        if (n >= bt->cap247)
            return;
        if (util_Random(0x2ee) >= 2)
            return;
        item = 0x135;
        if (ch->budget <= (int)gm_ItemPrice(item, 100) * 2)
            return;
        cm_SendMoveGoods(-1, ai_Chars[ch->bld->owner].netId, ai_OwnPlayer, (int)gm_ItemPrice(item, 100));
        cm_SendExSellObject(ch->bld->id, -1, item, 1, ai_OwnPlayer, 0);
    }
    sprintf(buf, "ai_MeisterCollectTrans(): Um %li:%li Uhr zus\xe4tzlichen Transporter f\xfcr %s gekauft",
            ai_Time.hour, ai_Time.min, ch->bld->name);
    ch->flags1c8 |= 8;
    } else {
        ch->flags1b4 &= ~0x80;
    }
}
