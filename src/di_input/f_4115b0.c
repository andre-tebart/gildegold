/* 0x4115b0, clear the click / button events selected by mask in dm_L and dm_P (left and right swapped when di_SwapLR) */

void di_ClearEvents(unsigned char mask)
{
    if (di_SwapLR != 0) {
        if (mask & 1) {
            dm_L.rclick = 0;
            dm_P.rclick = 0;
        }
        if (mask & 2) {
            dm_L.rup = 0;
            dm_P.rup = 0;
        }
        if (mask & 4) {
            dm_L.rdown = 0;
            dm_P.rdown = 0;
        }
        if (mask & 8) {
            dm_L.lclick = 0;
            dm_P.lclick = 0;
        }
        if (mask & 0x10) {
            dm_L.lup = 0;
            dm_P.lup = 0;
        }
        if (mask & 0x20) {
            dm_L.ldown = 0;
            dm_P.ldown = 0;
        }
    } else {
        if (mask & 1) {
            dm_L.lclick = 0;
            dm_P.lclick = 0;
        }
        if (mask & 2) {
            dm_L.lup = 0;
            dm_P.lup = 0;
        }
        if (mask & 4) {
            dm_L.ldown = 0;
            dm_P.ldown = 0;
        }
        if (mask & 8) {
            dm_L.rclick = 0;
            dm_P.rclick = 0;
        }
        if (mask & 0x10) {
            dm_L.rup = 0;
            dm_P.rup = 0;
        }
        if (mask & 0x20) {
            dm_L.rdown = 0;
            dm_P.rdown = 0;
        }
    }
}
