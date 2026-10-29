/* 0x428d10 */

int evt_Else(void)
{
    EVTTOKEN t;

    if (evt_Cur->frame->loops[evt_Cur->frame->level + 1].cond) {
        evt_GetToken(evt_Cur->pc, &t);
        if (t.type == EVT_KEYWORD && t.u.i == 4) {
            evt_SkipBlock(1);
            return 0;
        }
        evt_SkipStatement();
    }
    return 0;
}
