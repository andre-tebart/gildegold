/* 0x49d0f0, decide callback 0x29: as 0x28 (house first), then what 0x4aa580 finds; flag 0x100 of +0x1e8
   makes it rare */

int ai_Unk49d0f0(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    unsigned char *h;
    AIARG s1;
    AIARG s2;
    AIARG t1;
    AIARG t2;
    int r;

    if (f != 0)
        return 0;
    if ((*(int *)((char *)p + 0x1e8) & 0x100) && util_RandomU(4) != 0)
        return 0;
    h = *(unsigned char **)((char *)p + 0x170);
    if (h == 0) {
        if (((unsigned char *)p)[2] != 3) {
            s1.unk08 = 1;
            s1.kind = 5;
            s1.unk04 = 4;
            s2.kind = 0;
            s2.unk04 = 0;
            s2.unk08 = 0;
            r = ai_Unk495a50(0x28, p->id, 1, &s1, &s2);
            goto done;
        }
    } else if (((unsigned char *)p)[2] != 3) {
        if (gm_Query(*(int *)(h + 0x5d), 2, 6, 0, 0x15) == 0) {
            s1.kind = 2;
            s1.unk04 = 0x15;
            s1.unk08 = 1;
            s1.unk14 = 0.44f;
            s2.kind = 4;
            s2.unk04 = *(int *)(h + 1);
            s2.unk08 = 1;
            r = ai_Unk495a50(0x29, p->id, 1, &s1, &s2);
            goto done;
        }
        goto find;
    }
    if (!(((unsigned char *)p)[0x1e8] & 4))
        return 0;
find:
    memset(&t1, 0, sizeof(t1));
    memset(&t2, 0, sizeof(t2));
    if (ai_Unk4aa580(p, &t1, &t2) == 0)
        return 0;
    *o1 = t1;
    *o2 = t2;
    return 0x29;
done:
    if (r == 0)
        return r;
    *o1 = s1;
    *o2 = s2;
    return r;
}
