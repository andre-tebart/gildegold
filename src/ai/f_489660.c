/* 0x489660, sends the free thieves out */

void ai_SendFreeThieves(AICHAR *ch)
{
    AICHAR *c = (AICHAR *)ch;
    AIOBJ *o;
    AIMSG msg;
    int i, n;
    char k;
    AIBTYPE *bt;

    n = 2;
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == c->bld && ai_Chars[i].title && ai_Chars[i].job && ai_Chars[i].job->kind == 0x16)
            n--;
    }
    if (0 < n) {
        bt = &ai_BTypes[c->bld->type];
        k = ((volatile AIBTYPE *)bt)->kind;
        if (k == 4)
            o = gm_Query(c->bld->objList, 2, 6, 0, 0x60);
        else if (k == 0x13)
            o = gm_Query(c->bld->objList, 2, 6, 0, 0x145);
        else
            o = 0;
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x16;
        msg.to = ai_Chars[c->bld->owner].netId;
        msg.fromId = c->bld->id;
        msg.time = ai_TimeT;
        msg.flags = 1;
        if (o)
            msg.arg[2] = o->objId;
        else
            msg.arg[2] = -1;
        i = 0;
        while (1) {
            if (i >= 768)
                break;
            if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == c->bld && ai_Chars[i].title && (ai_Chars[i].flags1b4 & 0x10)) {
                if (ai_Chars[i].job) {
                    if (ai_Chars[i].job->kind == 0x16)
                        ai_Chars[i].flags1b4 &= ~0x10;
                } else {
                    if (ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                        if (ai_Chars[i].sk83 < ai_Chars[i].sk82) {
                            msg.arg[1] = ai_Chars[i].netId;
                            *(char *)&msg.arg[0] = 3;
                            cm_SendLongMessage(&msg);
                            n--;
                        } else {
                            msg.arg[1] = ai_Chars[i].netId;
                            *(char *)&msg.arg[0] = 2;
                            cm_SendLongMessage(&msg);
                            n--;
                        }
                        ai_Chars[i].flags1c8 |= 4;
                    }
                }
            }
            i++;
            if (n <= 0)
                break;
        }
    }
}
