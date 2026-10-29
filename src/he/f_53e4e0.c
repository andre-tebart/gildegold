/* 0x53e4e0, spreads the gfx infos (0x7588a0, 16 bytes each, owner = `obj`) of one object on a circle (radius 100, 0 for a single one) around its position (ROUGH) */

void he_Unk53e4e0(int obj)
{
    int cnt = 0;
    int *p = (int *)((unsigned)&he_RunHandlers + 0x2a0);
    int list[32];
    float vec[3];
    float mn, mx, rad, x, y, z;
    int i, e;
    double ang;
    float out[3];

    do {
        if (cnt >= 0x20)
            break;
        if (p[2] == obj) {
            list[cnt] = (int)p;
            cnt++;
        }
        p += 4;
    } while ((int)p < ((unsigned)&he_RunHandlers + 0x6a0));
    rad = 0.0f;
    if (cnt != 1)
        rad = 100.0f;
    d3_PointToWorld(obj, obj + 0x4c, vec);
    d3t_GetHeightRange(obj, &mn, &mx);
    vec[1] = mx;
    for (i = 0; i < cnt; i++) {
        e = list[i];
        d3t_GetHeightRange(*(int *)(e + 0xc), &mn, &mx);
        ang = ((double)i * 6.2831855f) / (double)cnt;
        out[0] = (float)(sin(ang) * rad + vec[0]);
        out[1] = (mx - mn) * 0.5 + vec[1];
        out[2] = (float)(cos(ang) * rad + vec[2]);
        d3_SetObjectPos(*(int *)(e + 0xc), out);
    }
}
