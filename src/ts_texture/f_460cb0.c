/* 0x460cb0, advances the texture scroll offsets (wrapped to 0..1) */

void ts_UpdateScroll(unsigned long time)
{
    unsigned long dt;
    float f, s;
    int i;

    if (dt = time - ts_LastTime) {
        f = (float)dt;
        ts_LastTime = time;
        for (i = 1; i < 16; i++) {
            s = f * d3_ScrollSpeed[i];
            ts_ScrollV[i] += s;
            if (ts_ScrollV[i] > 1.0f)
                ts_ScrollV[i] -= 1.0f;
            ts_ScrollU[i] += s;
            if (ts_ScrollU[i] > 1.0f)
                ts_ScrollU[i] -= 1.0f;
        }
        for (i = 1; i < 16; i++) {
            s = f * d3_ScrollSpeed[i];
            ts_ScrollV[i + 16] -= s;
            if (ts_ScrollV[i + 16] < 0.0f)
                ts_ScrollV[i + 16] += 1.0f;
            ts_ScrollU[i + 16] -= s;
            if (ts_ScrollU[i + 16] < 0.0f)
                ts_ScrollU[i + 16] += 1.0f;
        }
    }
}
