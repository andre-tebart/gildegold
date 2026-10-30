/* 0x4884b0, buys more slots of the building */

void ai_BuySlots(AIOBJ *b, AICHAR *ch, int n)
{
    AICHAR *c = (AICHAR *)ch;
    AIJOB *job;
    AIBLD *bld;
    AIMSG msg;
    char buf[1024];
    int a, d, ma, mb, x, y;
    int cnt, r;

    if (b && !(c->flags1c8 & 8) && (int)util_Random(100) >= n) {
        a = b->lvl1c;
        d = b->lvl1d;
        for (job = he_FindEvent(2, 0, 0x12, 3, c->bld->id); job; job = he_NextEvent()) {
            a += job->unkb0;
            d += job->unkb4;
        }
        bld = c->bld;
        mb = ai_BTypesT[bld->type].min1;
        y = mb - d;
        ma = ai_BTypesT[bld->type].min0;
        x = ma - a;
        if ((0 < x || 0 < y) && c->budget >= 24000) {
            memset(&msg, 0, sizeof(msg));
            msg.type = 0x12;
            msg.to = ai_CharsT[bld->owner].netId;
            msg.fromId = bld->id;
            msg.flags = 1;
            msg.time = ai_TimeT;
            msg.arg[0] = b->objId;
            msg.arg[1] = x != 0;
            msg.arg[2] = y != 0;
            cnt = msg.arg[2] + msg.arg[1];
            msg.arg[3] = 1;
            cm_BeginGroup("AI-Slots");
            cm_SendLongMessage(&msg);
            cm_SendMoveGoods(-1, ai_CharsT[c->bld->owner].netId, ai_OwnPlayer, 8000);
            switch (ls_GetLanguage()) {
            case 0:
                if (cnt == 1)
                    txt_Format(buf, 0x1bbc, c->id, c->bld);
                else
                    txt_Format(buf, 0x1bbd, c->id, c->bld, cnt);
                break;
            case 1:
                if (cnt == 1)
                    txt_Format(buf, 0x1bbc, c->id, c->bld);
                else
                    txt_Format(buf, 0x1bbd, c->id, c->bld, cnt);
                break;
            }
            he_MessageBoxQuickjumpI(ai_CharsT[c->bld->owner].netId, ai_CharsT[c->bld->owner].netId, buf, 0, 0x60b,
                         c->bld->id, -1, 0, 0);
            c->budget += cnt * -8000;
            cm_EndGroup("AI-Slots");
            c->flags1c8 |= 8;
        }
    }
}
