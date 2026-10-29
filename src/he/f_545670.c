/* 0x545670, a Ghidra miss (disassembled): the patrol ("STREIFE_INIT: Target: %i, Way: %i, %i, %i, %i, Start: %i") event init: the record clock (+0x44) is set, the up to 8 figures of kind 6 (not the target / start / kind 0x47, with a record at +0x61) within 950.0 of the target (+0xac) are collected, 0x505ca0 picks up to 4 of them as the way (+0xb4 .. +0xc0), the event is posted (+0xd4 = -1, +0xd8 = 0, +0xdc = 0) (ROUGH) */

void gm_Shuffled(unsigned char n, int *out);

void he_Unk545670(int ev)
{
    int cand[8];
    int sel[8];
    char buf[0x100];
    int n = 0;
    int t, f, k, r, o;
    float dx, dy, dz;
    double dist;

    if ((*(unsigned char *)(ev + 0x78) & 4) == 0) {
        *(HETIME *)(ev + 0x44) = ai_Time;
        memset(cand, -1, sizeof(cand));
        t = gm_BuildQuery(1, 1, *(int *)(ev + 0xac));
        f = gm_BuildQuery(1, 6);
        while (f != 0) {
            if (n >= 8)
                break;
            r = *(int *)(f + 0x61);
            if (r != 0 && *(int *)(f + 1) != *(int *)(t + 1) && *(int *)(f + 1) != *(int *)(ev + 0xb0) && *(char *)f != 0x47) {
                o = *(int *)(t + 0x61);
                dx = *(float *)(r + 0x4c) - *(float *)(o + 0x4c);
                dy = *(float *)(r + 0x50) - *(float *)(o + 0x50);
                dz = *(float *)(r + 0x54) - *(float *)(o + 0x54);
                dist = sqrt(dx * dx + dy * dy + dz * dz);
                if (dist <= 950.0f) {
                    cand[n] = *(int *)(f + 1);
                    n++;
                }
            }
            f = gm_BuildNext();
        }
        if (n != 0) {
            *(int *)(ev + 0xb4) = -1;
            *(int *)(ev + 0xb8) = -1;
            *(int *)(ev + 0xbc) = -1;
            *(int *)(ev + 0xc0) = -1;
            gm_Shuffled(n, sel);
            for (k = 0; k < n && k < 4; k++)
                *(int *)(ev + 0xb4 + k * 4) = cand[sel[k]];
        }
        sprintf(buf, "STREIFE_INIT: Target: %i, Way: %i, %i, %i, %i, Start: %i", *(int *)(ev + 0xac), *(int *)(ev + 0xb4), *(int *)(ev + 0xb8),
                *(int *)(ev + 0xbc), *(int *)(ev + 0xc0), *(int *)(ev + 0xb0));
        *(HETIME *)(ev + 0x52) = ai_Time;
        gm_AddTime(ev + 0x52, 0, 0, 1);
        *(int *)(ev + 0xd8) = 0;
        *(char *)(ev + 0xdc) = 0;
        *(int *)(ev + 0xd4) = -1;
        *(int *)(ev + 0x84) = cm_SendExSetHE(0, ev);
    }
}
