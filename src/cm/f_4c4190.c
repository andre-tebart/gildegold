/* 0x4c4190, "cm_ExecCommand": runs the received messages (groups 5..6 checked first by 0x4c43c0), frees them;
   when the connection is closed, fails all of them */

int cm_ExecCommands(void)
{
    G17MSG *m, *t, *next, *grp;
    G17STAT *s;
    int ok;

    if (net_Conn.closed != 0) {
        while ((m = ai_Unka68d24) != 0) {
            if (m->idx != -1) {
                s = &ai_Unkf31160[m->idx & 0x7fff];
                s->state = 2;
                s->unk01 = 0;
                s->value = 0;
            }
            ai_Unk4c4130(m);
        }
        ai_Unka68d24 = 0;
        for (s = ai_Unkf31160; s < ai_Unkf31160 + 0x8000; s++)
            s->state = 2;
        if (ai_GameOver == 0) {
            ai_GameOver = 1;
            sv_PopTo(main_GameLoop);
        }
    }
    grp = 0;
    for (m = ai_Unka68d24; m != 0; m = next) {
        next = m->next;
        t = m;
        ok = 1;
        if (grp == 0) {
            if (m->type == 5)
                grp = m;
        } else if (m->type == 6) {
            ok = ai_Unk4c43c0(grp) != 0;
            t = grp;
            next = grp->next;
            grp = 0;
        }
        if (grp != 0)
            ok = 0;
        if (t->type != 7 && ai_Unk4c3850(t) != 0 && ok) {
            if (t == ai_Unka68d24) {
                ai_Unka68d24 = next;
            } else {
                t->prev->next = t->next;
                if (t->next != 0)
                    t->next->prev = t->prev;
            }
            t->prev = 0;
            t->next = ai_Unka68c00;
            ai_Unka68c00 = t;
            ai_Unk4c38e0(t);
            if (t->type < 0x6a) {
                if (t->idx == -1) {
                    s = 0;
                } else {
                    s = &ai_Unkf31160[t->idx & 0x7fff];
                    s->state = 2;
                    s->unk01 = 0;
                    s->value = 0;
                    if (t->seq != ai_Unka68d1c + 1) {
                        if (t->seq == (unsigned int)cm_LastSyncCount) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received a Command with Count == cm_LastSyncCount, cmd->Count is %li, LastRequCnt is %li", t->seq, ai_Unka68d1c);
                        } else if (t->type == 0x20 && t->data[0] == 0x0e) {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Received Sync, cmd->Count is %li, LastRequCnt is %li", t->seq, ai_Unka68d1c);
                        } else {
                            char buf[0x100];
                            sprintf(buf, "cm_ExecCommand(): Lost a Command, cmd->Count is %li, cm_LastRequCount is %li", t->seq, ai_Unka68d1c);
                            ai_Unka68d1c = t->seq;
                        }
                    } else {
                        ai_Unka68d1c = t->seq;
                    }
                }
                ai_Unk60bd98[t->type](t, s);
            }
        }
    }
    return 0;
}
