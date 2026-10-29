/* 0x4e3570, runs the scene for a ticks of 14 ms (d3_Time based): the end sets the stop flag 0x6f22a0 */

int ai_Unk4e3570(int a)
{
    int start;
    double lim;

    start = d3_Time;
    if (ai_Unk6f0d8c == 0) {
        if (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0) {
            lim = (double)a * (1.0 / 14) + start;
            do {
                if ((double)d3_Time >= lim)
                    sv_PopRequest = 1;
                ai_Unk4cee20();
                ai_Unk4e37a0(ai_Unk6f0d90);
            } while (sv_RunFrame(ai_Unk4e3570, ai_Unk60d360 | 0x8000) != 0);
        }
    }
}
