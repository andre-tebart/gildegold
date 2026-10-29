/* 0x4889f0, sends the master's order of one kind (info->type) for the building: 0x28 is a harvest order (the
   plants of the building's vegetation map are counted down, the free grown records get one harvest job each; a
   full char table or no free plant record ends the round), 0x14 / 0x15 are goods orders (the entry's item and
   the building's main object go into the message, the spare characters of the building are collected), 0x8c and
   the trade kinds 3 / 4 / 8 name the carts' objects (info->b / info->c) and the building in the message.
   Side effects: info->z2 counts what was sent (also across calls), info->z1 is set as the round flag; the order
   message goes as one cm_SendLongMessage (0x28: one per free character, then the round ends).
   @deep behaviour draft: 519 code hunks (was 562); every branch read against tools/asmdiff.py and build/ghidra/4889f0.c - statements, constants, callees and field offsets equal the original for 0x28 (harvest: the n2/n1 split, the plant counting, the 64-record map scan, the message block), 0x14 / 0x15 (goods orders), 0x8c and 3 / 4 / 8 (cart objects). Left: the register allocation of the map pointer (original ecx / ebp, ours eax), the AIMSG store scheduling and the [esi+0x1c] reloads. Last-bytes leads tried: the map scan as the original do { } while (!found) with the k>=64 null return inside (kept, 562 -> 519 code hunks); spell.py reload / width leads (488b81, 488d44, 488ef8, 489240, 488f90) measured byte-identical; the typed itemId write compiles the same as the raw cast. Q2 evidence gaps: AIMAPENT.unk10 (the vegetation record +0x10, written -1 by veg_SetPlant, only compared == -1 here), AIMSG.unk14 (only ever written 0 / -1, no reader), AIJOB.unkb8 (a per-kind payload word: the plant id here, counters / ids / news ids in he.c). Q4 blocked header: the AIMSG 0x58 count byte and 0x5a item word need an anonymous struct in the AIMSG.arg union, and that flips ai_Unk483a60 to 5 differing bytes (the [esp+0x10]/[esp+0x18] slot swap; measured again this session, reverted). */

void ai_SendJobOrders(AIINFO *info)
{
    AIMSG msg;
    AIJOB *job;
    AIOBJ *o;
    AIMAPENT *me, *found;
    AIMAPENT *rec;
    int map, type, n, total, base, i, k, v;

    type = info->type;
    if (type == 0x28) {
        n = info->n1;
        map = info->ch->bld->plants;
        if (n <= 1 || info->z1 != 0) {
            total = info->n2 / n;
            base = info->z2;
        } else {
            base = info->n2 / n;
            total = info->z2 + info->n2 % n;
        }
        total += base;
        for (job = he_FindEvent(2, 0, 0x28, 3, info->ch->bld->id); job; job = he_NextEvent()) {
            rec = (AIMAPENT *)veg_FindPlantId((struct G15PLANT *)map, job->unkb8);
            if (rec && rec->type == (unsigned int)info->e->item)
                total--;
        }
        if (info->z2 < total) {
            i = 0;
            do {
                if (i >= 768)
                    break;
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == info->ch->bld && ai_Chars[i].title && ai_Chars[i].job == 0 && ai_Chars[i].model &&
                    ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                    found = 0;
                    me = (AIMAPENT *)map;
                    k = 0;
                    do {
                        if (k >= 64) {
                            if (!found)
                                return;
                            break;
                        }
                        if (me->state > 1 && me->unk10 == -1)
                            found = me;
                        k++;
                        me++;
                    } while (!found);
                    k = found->id;
                    memset(&msg, 0, sizeof(msg));
                    msg.type = info->type;
                    msg.to = ai_Chars[info->ch->bld->owner].netId;
                    msg.fromId = info->ch->bld->id;
                    msg.time = ai_Time;
                    msg.nIds = 1;
                    msg.unk14 = 0;
                    msg.flags = 1;
                    msg.arg[3] = k;
                    msg.arg[0] = info->ch->bld->id;
                    msg.arg[1] = ai_Chars[i].netId;
                    cm_SendLongMessage(&msg);
                    info->z2++;
                }
                i++;
            } while (info->z2 < total);
        }
    } else if (type == 0x14) {
        n = info->n1;
        if (n <= 1 || info->z1 != 0)
            total = info->n2 / n + info->z2;
        else
            total = info->z2 + info->n2 / n + info->n2 % n;
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_Chars[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.time = ai_Time;
        msg.nIds = 0;
        msg.unk14 = 0;
        msg.flags = 1;
        *((unsigned short *)&msg.arg[0] + 1) = *(unsigned short *)info->e;
        o = (AIOBJ *)gm_BuildingMainObject(info->ch->bld);
        msg.arg[1] = o->objId;
        if (info->z2 < total) {
            i = 0;
            do {
                if (i >= 768)
                    break;
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == info->ch->bld && ai_Chars[i].title && ai_Chars[i].job == 0 &&
                    ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                    msg.ids[msg.nIds] = ai_Chars[i].netId;
                    msg.nIds++;
                    info->z2++;
                }
                i++;
            } while (info->z2 < total);
        }
        *(unsigned char *)&msg.arg[0] = (unsigned char)msg.nIds;
        if (msg.nIds != 0 && *((unsigned short *)&msg.arg[0] + 1) != 0) {
            cm_SendLongMessage(&msg);
            info->z1++;
            return;
        }
    } else if (type == 0x15) {
        n = info->n1;
        if (n <= 1 || info->z1 != 0) {
            total = info->n2 / n;
            base = info->z2;
        } else {
            base = info->n2 / n;
            total = info->z2 + info->n2 % n;
        }
        o = gm_Query(info->ch->bld->objList, 2, 6, 0, 0xdd);
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_Chars[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.nIds = 0;
        msg.time = ai_Time;
        msg.unk14 = 0;
        msg.flags = 1;
        *((unsigned short *)&msg.arg[0] + 1) = *(unsigned short *)info->e;
        if (!o) {
            o = (AIOBJ *)gm_BuildingMainObject(info->ch->bld);
            if (!o)
                msg.arg[2] = -1;
            else
                msg.arg[2] = o->objId;
        } else
            msg.arg[2] = o->objId;
        total += base;
        if (info->z2 < total) {
            i = 0;
            do {
                if (i >= 768)
                    break;
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == info->ch->bld && ai_Chars[i].title && ai_Chars[i].job == 0 &&
                    ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                    msg.ids[msg.nIds] = ai_Chars[i].netId;
                    msg.nIds++;
                    info->z2++;
                }
                i++;
            } while (info->z2 < total);
        }
        *(unsigned char *)&msg.arg[0] = (unsigned char)msg.nIds;
        if (msg.nIds != 0 && *((unsigned short *)&msg.arg[0] + 1) != 0) {
            cm_SendLongMessage(&msg);
            info->z1++;
            return;
        }
    } else if (type == 0x8c) {
        gm_Query(info->ch->bld->objList, 1, 0, 0xfd);
        n = info->n1;
        if (n <= 1 || info->z1 != 0) {
            base = info->z2;
            total = info->n2 / n + base;
        } else {
            base = info->z2;
            total = info->n2 % n + base + info->n2 / n;
        }
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_Chars[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        msg.time = ai_Time;
        msg.nIds = 0;
        msg.unk14 = 0;
        msg.flags = 1;
        msg.itemId = info->e->item;
        if (base < total) {
            i = 0;
            do {
                if (i >= 768)
                    break;
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == info->ch->bld && ai_Chars[i].title && ai_Chars[i].job == 0 &&
                    ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                    msg.ids[msg.nIds] = ai_Chars[i].netId;
                    msg.nIds++;
                    info->z2++;
                }
                i++;
            } while (info->z2 < total);
            if (msg.nIds != 0 && msg.itemId != 0) {
                cm_SendLongMessage(&msg);
                info->z1++;
                return;
            }
        }
    } else {
        for (job = he_FindEvent(2, 0, type, 3, info->ch->bld->id); job; job = he_NextEvent()) {
            n = info->type;
            if ((n == 4 && job->u.w.lo == (unsigned int)info->e->item) ||
                (n == 8 && job->u.w.lo == (unsigned int)info->e->item) ||
                (n == 3 && job->u.w.lo == (unsigned int)info->e->item)) {
                n = info->n1;
                if (n <= 1 || info->z1 != 0)
                    total = info->n2 / n + info->z2;
                else
                    total = info->z2 + info->n2 / n + info->n2 % n;
                k = info->type;
                v = 0;
                if (k == 8 || k == 3)
                    v = (int)gm_Query(info->ch->bld->objList, 1, 0, 0x37);
                else if (k == 4)
                    v = gm_BuildingMainObject(info->ch->bld);
                if (total <= info->z2)
                    goto done;
                i = 0;
                while (1) {
                    if (i >= 768)
                        break;
                    if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == info->ch->bld && ai_Chars[i].title && ai_Chars[i].job == 0) {
                        sv_MoveToGroup(info->ch->bld, v, ai_Chars[i].id, job);
                        info->z2++;
                    }
                    i++;
                    if (total <= info->z2)
                        break;
                }
                goto done;
            }
        }
        memset(&msg, 0, sizeof(msg));
        msg.type = info->type;
        msg.to = ai_Chars[info->ch->bld->owner].netId;
        msg.fromId = info->ch->bld->id;
        type = info->type;
        msg.nIds = 0;
        msg.unk14 = 0;
        msg.flags = 1;
        if (type == 4) {
            msg.arg[1] = -1;
            msg.itemId = info->e->item;
            msg.arg[4] = info->ch->bld->id;
            msg.arg[5] = info->b->objId;
        }
        if (type == 8) {
            msg.itemId = info->e->item;
            msg.arg[1] = info->ch->bld->id;
            msg.arg[2] = info->c->objId;
        }
        msg.time = ai_Time;
        if (type == 3) {
            msg.itemId = info->e->item;
            msg.arg[3] = info->ch->bld->id;
            msg.arg[5] = info->b->objId;
            msg.arg[4] = info->c->objId;
        }
        cm_SendLongMessage(&msg);
    }
done:
    info->z1++;
}
