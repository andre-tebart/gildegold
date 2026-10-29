/* 0x4e3490, like 0x4e3570 for a ticks, but a click (0x14673a0) ends it at once with cb(a) (1 without cb) as the result */

int ai_Unk4e3490(int a, int (*cb)(int))
{
    int start, res;
    double lim;

    start = d3_Time;
    res = 0;
    if (ai_Unk6f0d8c != 0)
        return 0;
    if (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0) {
        do {
            if (dm_L.rup != 0) {
                if (cb != 0)
                    res = cb(a);
                else
                    res = 1;
                sv_PopRequest = 1;
            } else {
                lim = (double)a * (1.0 / 14) + start;
                if ((double)d3_Time >= lim)
                    sv_PopRequest = 1;
            }
            ai_Unk4cee20();
            ai_Unk4e37a0(ai_Unk6f0d90);
        } while (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0);
    }
    return res;
}
