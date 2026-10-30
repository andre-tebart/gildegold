/* 0x488740, buys a capacity upgrade */

void ai_BuyCapacity(AIOBJ *b, AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIMSG msg;

    if (b && !(c->flags1c8 & 8)) {
        if ((int)util_Random(100) >= n) {
            if (!he_FindEvent(2, 0, 0x13, 3, c->bld->id) && b->count < 3 && c->budget >= 0x9600) {
                memset(&msg, 0, sizeof(msg));
                msg.type = 0x13;
                msg.to = ai_Chars[c->bld->owner].netId;
                msg.fromId = c->bld->id;
                msg.flags = 1;
                msg.time = ai_TimeT;
                msg.arg[0] = b->objId;
                msg.arg[1] = 1;
                msg.arg[3] = 1;
                cm_BeginGroup("AI-kapazit\344t");
                cm_SendLongMessage(&msg);
                cm_SendMoveGoods(-1, ai_Chars[c->bld->owner].netId, ai_OwnPlayer, 0x3200);
                c->budget += -0x3200;
                cm_EndGroup("AI-kapazit\344t");
                c->flags1c8 |= 8;
            }
        }
    }
}
