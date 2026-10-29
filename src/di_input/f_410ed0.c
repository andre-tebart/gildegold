/* 0x410ed0, with di_SwapLR set, exchange the left / right click and release flags of dm_S */

void di_SwapButtons(void)
{
    DIMOUSEST t;

    if (di_SwapLR != 0) {
        t = dm_S;
        dm_S.lup = dm_S.rup;
        dm_S.lclick = dm_S.rclick;
        dm_S.rclick = t.lclick;
        dm_S.rup = t.lup;
    }
}
