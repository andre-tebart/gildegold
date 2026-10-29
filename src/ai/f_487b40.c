/* 0x487b40 */

void ai_UpdateStaff(AICHAR *ch)
{
    AIMSG msg;
    AIBTYPE *bt;
    AICHAR *oc;
    AIJOB *job;
    int *jp;
    unsigned int fl;
    int i, n, k, v;
    unsigned int t;

    if (ai_Time.hour != 7 && ai_Time.hour != 0xc && ai_Time.hour != 0x12) {
        ch->flags1b4 &= ~0x40;
        return;
    }
    fl = ch->flags1b4;
    if (!(fl & 0x40)) {
        fl |= 0x40;
        ch->flags1b4 = fl;
        if ((ai_Chars[ch->bld->owner].kind != 6 && ai_Chars[ch->bld->owner].kind != 7) || (fl & 2)) {
            bt = &ai_BTypes[ch->bld->type];
            for (i = 0; i < 768; i++) {
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == ch->bld && ai_Chars[i].title != 0 &&
                    ai_Chars[i].title == (char)bt->k230 && *(int *)((char *)&ai_Chars[i] + 0x174) >= 4) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 0x32;
                    msg.to = ai_Chars[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.flags = 2;
                    msg.time = ai_Time;
                    msg.arg[0] = ai_Chars[i].netId;
                    cm_SendLongMessage(&msg);
                }
            }
            n = 0;
            for (i = 0; i < 768; i++) {
                if (ai_Chars[i].id != 0xffff && ai_Chars[i].bld == ch->bld && ai_Chars[i].title != 0)
                    n++;
            }
            job = he_FindEvent(2, 0, 6, 3, ch->bld->id);
            if (job != 0) {
                jp = (int *)((char *)job + 0xb0);
                for (k = 0; k < 12; k++) {
                    if (*jp != 0)
                        n++;
                    jp++;
                }
            }
            t = ch->bld->type;
            if (n < ai_BTypes[t].k231 + ai_BTypes[t].k232) {
                oc = &ai_Chars[ch->bld->owner];
                v = (int)gm_Unk50cd40(oc, t, -1, 1);
                if (n != 0) {
                    if (ch->flags1c8 & 8)
                        return;
                    if (ch->budget < v)
                        return;
                    if (util_Random(0x48) < (0x12 - n) * 2)
                        return;
                }
                if (job == 0) {
                    memset(&msg, 0, sizeof(msg));
                    msg.type = 6;
                    msg.to = ai_Chars[ch->bld->owner].netId;
                    msg.fromId = ch->bld->id;
                    msg.flags = 1;
                    msg.time = ai_Time;
                    msg.arg[1] = 1;
                    gm_IsHomeType(ch->bld);
                    *(int *)((char *)&msg + 0x8c) = -1;
                    ((unsigned char *)&msg.arg[0])[1] = 1;
                    ((unsigned char *)&msg.arg[0])[0] = ai_BTypes[ch->bld->type].k230;
                    cm_SendLongMessage(&msg);
                    ch->budget -= v;
                }
            }
        }
    }
}
