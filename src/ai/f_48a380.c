/* 0x48a380, the unload order of the stolen goods carts of a master's building */

void ai_UnloadCarts(AICHAR *ch)
{
    AIOBJ *car;
    AIMSG msg;
    int id, fill;

    if (ai_IsLocalJob(ch) && (ch->flags1b4 & 1)) {
        for (car = gm_Query(ch->bld->objList, 3, 6, 4, 0x1d, 3, ch->bld->owner); car;
             car = gm_QueryNext()) {
            if (car->fill < util_Random(0x20) + 0x28) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x10;
                msg.to = ai_CharsT[ch->bld->owner].netId;
                msg.fromId = ch->bld->id;
                msg.flags = 1;
                msg.time = ai_TimeT;
                gm_AddTime(&msg.time, 0, 0, 10);
                id = car->objId;
                fill = car->fill;
                msg.dst[0] = 5;
                msg.car = id;
                msg.u.s.unk84 = 0;
                msg.n = 100 - fill;
                cm_SendLongMessage(&msg);
            }
        }
    }
}
