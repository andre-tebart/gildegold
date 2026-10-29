/* 0x56f600, the running event 0x18 (of the person a: its +4) not owned by c (its +4) whose building (0x4fea50 of +0xb4) is nearest to the place +0x4c .. of the workplace (+0x184 -> +0x34) of b; 0 when none */

int he_Unk56f600(int a, int b, int c)
{
    float best = 1.0e7f;
    float x, y, z, dx, dy, dz, d;
    int job, found;
    char *ev;

    if (HM_I(b, 0x184) == 0)
        return 0;
    job = HM_I(HM_I(b, 0x184), 0x34);
    x = *(float *)(job + 0x4c);
    y = *(float *)(job + 0x50);
    z = *(float *)(job + 0x54);
    he_FindEvent(1, 0, 0x18);
    found = 0;
    for (ev = he_FindEvent(1, 0, 0x18); ev != 0; ev = he_NextEvent()) {
        if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2 || c == HM_I(ev, 4))
            continue;
        if (HM_I(ev, 0xc4) != HM_I(a, 4))
            continue;
        gm_FindById(&b, 0, 0, HM_I(ev, 0xb4));
        if (b == 0)
            continue;
        job = HM_I(b, 0x61);
        dx = *(float *)(job + 0x4c) - x;
        dy = *(float *)(job + 0x50) - y;
        dz = *(float *)(job + 0x54) - z;
        d = sqrt(dx * dx + dy * dy + dz * dz);
        if (found == 0 || d < best) {
            found = (int)ev;
            best = d;
        }
    }
    return found;
}
