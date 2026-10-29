/* 0x553ae0 (ROUGH): `q` (+0 an object with a scene record at +0x61, +4 the range, +8 the other person) looks for a running record of type 0x43 (no +0xd4 target yet, another +0xc, with one of its four persons +0xc4 .. closer than the range, 3D distance of the scene positions); stores it in q+0xc, sends the four persons (message 0x4c5230, "gestik/herranwinken" for the person itself, 0x4c52e0 "Detected", "gestik/abfaellig" 1 .. 2), sets the target +0xd4 (the other person's id), the due clock now + 8 and the record's message 0x400; 1 = found */

void ai_Unk4c5410(int a, const char *s, unsigned char c);
int ai_Unk4c52e0();
void ai_Unk4c5230(int a);

int he_Unk553ae0(int *q)
{
    char *e;
    int best = 0, found = 0, i, h, pp;
    float *a, *b;
    float dx, dy, dz, dist;

    if (q[0] == 0 || *(int *)(q[0] + 0x61) == 0)
        return 0;
    e = he_FindEvent(1, 0, 0x43);
    for (; e != 0; e = he_NextEvent()) {
        if (best == 0) {
            if (*(int *)(e + 0xd4) == -1 && *(int *)(e + 0xc) != *(int *)(q[2] + 0xc)) {
                for (i = 0; i < 4; i++) {
                    h = *(int *)(e + 0xc4 + i * 4);
                    if (h == -1)
                        continue;
                    pp = gm_FindChar(h);
                    found = pp;
                    if (pp == 0)
                        continue;
                    a = (float *)*(int *)(q[0] + 0x61);
                    b = (float *)*(int *)(*(int *)(*(int *)(pp + 0x184) + 0x34));
                    dx = a[0x4c / 4] - b[0x4c / 4];
                    dy = a[0x50 / 4] - b[0x50 / 4];
                    dz = a[0x54 / 4] - b[0x54 / 4];
                    dist = (float)sqrt(dx * dx + dy * dy + dz * dz);
                    if (dist < *(float *)&q[1]) {
                        best = (int)e;
                        break;
                    }
                }
            }
        }
    }
    if (best == 0)
        return 0;
    q[3] = best;
    for (i = 0; i < 4; i++) {
        h = *(int *)(best + 0xc4 + i * 4);
        if (h == -1)
            continue;
        pp = gm_FindChar(h);
        if (pp == 0)
            continue;
        ai_Unk4c5230(h);
        if (*(int *)(pp + 4) == h)
            ai_Unk4c5410(h, "gestik/herranwinken", 2);
        ai_Unk4c52e0(h, *(int *)(q[0] + 1), -1, 0, 1, "Detected");
        ai_Unk4c5410(h, "gestik/abfaellig", (unsigned short)util_Random(2) + 1);
    }
    *(int *)(best + 0xd4) = *(int *)(q[2] + 4);
    *(HETIME *)(best + 0x52) = ai_Time;
    gm_AddTime(best + 0x52, 0, 8, 0);
    cm_SendExSetHE(0x400, best);
    return 1;
}
