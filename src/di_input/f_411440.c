/* 0x411440: latch the next queued mouse event into dm_L */

void di_GetMouse(void)
{
    int i;

    if (di_Absolute != 0) {
        dm_L.x = dm_P.x;
        dm_L.y = dm_P.y;
        dm_L.z = dm_P.z;
    }
    i = 0;
    dm_L.lup = 0;
    dm_L.lclick = 0;
    dm_L.rup = 0;
    dm_L.rclick = 0;
    dm_L.mup = 0;
    dm_L.mclick = 0;
    for (; i < 32; i++) {
        if (dm_Q[i].valid != 0) {
            dm_L.lup = dm_Q[i].lup;
            dm_L.ldown = dm_Q[i].ldown;
            dm_L.lhold = dm_Q[i].lhold;
            dm_L.lclick = dm_Q[i].lclick;
            dm_L.lextra = dm_Q[i].lextra;
            dm_L.rup = dm_Q[i].rup;
            dm_L.rdown = dm_Q[i].rdown;
            dm_L.rhold = dm_Q[i].rhold;
            dm_L.rclick = dm_Q[i].rclick;
            dm_L.mup = dm_Q[i].mup;
            dm_L.mdown = dm_Q[i].mdown;
            dm_L.mhold = dm_Q[i].mhold;
            dm_L.mclick = dm_Q[i].mclick;
            dm_L.wup = dm_Q[i].wup;
            dm_L.wdown = dm_Q[i].wdown;
            dm_L.dx = dm_Q[i].dx;
            dm_L.dy = dm_Q[i].dy;
            dm_L.dz = dm_Q[i].dz;
            dm_Q[i].valid = 0;
            di_PollKeyboard();
            return;
        }
    }
    di_PollKeyboard();
}
