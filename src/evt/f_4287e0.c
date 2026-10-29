/* 0x4287e0 */

int evt_While(void)
{
    EVTTOKEN t;
    char *start, *pos;
    int cond;

    start = evt_Cur->pc - strlen(evt_Keywords[1]);
    evt_GetToken(evt_Cur->pc, &t);
    if (t.type == EVT_OP && t.u.i == EVT_OP_LPAREN) {
        cond = evt_Eval(0);
        evt_Cur->frame->loops[evt_Cur->frame->level].cond = cond;
        if (!cond) {
            evt_GetToken(evt_Cur->pc, &t);
            if (t.type == EVT_OP) {
                if (t.u.i == EVT_OP_LBRACE) {
                    evt_Cur->pc = start;
                    evt_SkipBlock(1);
                }
            } else if (!evt_SkipStatement()) {
                evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: ';' missing!");
            }
        } else {
            memset(&evt_Cur->frame->loops[evt_Cur->frame->level], 0, sizeof(EVTLOOP));
            evt_Cur->frame->loops[evt_Cur->frame->level].type = 1;
            evt_Cur->frame->loops[evt_Cur->frame->level].start = start;
            pos = evt_Cur->pc;
            evt_GetToken(pos, &t);
            if (t.type == EVT_OP && t.u.i == EVT_OP_LBRACE) {
                evt_Cur->frame->loops[evt_Cur->frame->level].braced = 0;
            } else {
                evt_Cur->frame->loops[evt_Cur->frame->level].braced = 1;
                evt_Cur->pc = pos;
            }
            evt_Cur->frame->level++;
            if (evt_Cur->frame->level > 8) {
                evt_Error(evt_Cur, evt_Cur->pc, "Too many looplevel...");
                evt_Cur->frame->level--;
            }
        }
    } else {
        evt_Error(evt_Cur, evt_Cur->pc, "Syntax error: '(' missing!");
    }
    return 0;
}
