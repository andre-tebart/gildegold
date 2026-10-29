/* 0x489870, sends the free spies out: every character of the building with the espionage job kind 0x8f (he.c
   0x571ac0) or marked free (flags1b4 bit 0x10) is counted; when one is free and the building's main object
   0x211 exists, each spare character (no job, its model in this building) loses the free mark, gets the used
   mark (flags1c8 bit 4) and one message 0x8f goes to the owner with the spy class in arg[1] (1: sk81 < sk82,
   else 2, 4 when sk82 >= sk84) and its netId in arg[0].
   @deep behaviour draft: 62 code hunks, 38 differing asm lines of 150 instructions; the body was read against tools/asmdiff.py (every statement, constant, callee and field offset equals the original: the n = 1 count over the 0x8f-job characters, the kind 0x1d main-object 0x211 gate, the flags1b4 bit 0x10 free mark, the sk81 / sk82 / sk84 classes 1 / 2 / 4, flags1c8 bit 4). What is left is the prologue register pair (original: ebp = the parameter, ebx = n; ours: ebx = the parameter, ebp = n; spell.py 48987c push-pop) and the reloads of [ch+0x16c] (spell.py 4898b5 / 489924 / 4899a5 reload, the original re-reads ch->bld at each use while ours keeps it). Tried, byte-identical (kept where cleaner): dropping the AICHAR *c alias for the parameter, swapping the i / n declaration order. Readability: Q1 cleared with the evidenced name ai_SendFreeSpies (the job kind 0x8f is he.c's espionage event 0x571ac0, the same band as ai_SendFreeThieves), Q6 description rewritten. */

void ai_SendFreeSpies(AICHAR *ch)
{
    AIOBJ *o;
    AIMSG msg;
    int i, n;

    n = 1;
    for (i = 0; i < 768; i++) {
        if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == ch->bld && ai_Chars[i].title && ai_Chars[i].job && ai_Chars[i].job->kind == -0x71)
            n--;
    }
    if (0 < n && ai_BTypes[ch->bld->type].kind == 0x1d && (o = gm_Query(ch->bld->objList, 2, 6, 0, 0x211)) != 0) {
        memset(&msg, 0, sizeof(msg));
        msg.type = 0x8f;
        msg.to = ai_Chars[ch->bld->owner].netId;
        msg.fromId = ch->bld->id;
        msg.time = ai_Time;
        msg.flags = 1;
        msg.arg[2] = o->objId;
        i = 0;
        while (1) {
            if (i >= 768)
                break;
            if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == ch->bld && ai_Chars[i].title && (ai_Chars[i].flags1b4 & 0x10)) {
                if (ai_Chars[i].job) {
                    if (ai_Chars[i].job->kind == -0x71)
                        ai_Chars[i].flags1b4 &= ~0x10;
                } else {
                    if (ai_Chars[i].model && ai_Chars[i].model->bldId == ai_Chars[i].bld->id) {
                        ai_Chars[i].flags1b4 &= ~0x10;
                        if (ai_Chars[i].sk81 < ai_Chars[i].sk82) {
                            msg.arg[0] = ai_Chars[i].netId;
                            msg.arg[1] = 1;
                            cm_SendLongMessage(&msg);
                        } else {
                            msg.arg[1] = 2;
                            msg.arg[0] = ai_Chars[i].netId;
                            if (ai_Chars[i].sk82 >= ai_Chars[i].sk84)
                                msg.arg[1] = 4;
                            cm_SendLongMessage(&msg);
                        }
                        n--;
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
