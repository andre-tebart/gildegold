/* 0x498b80, decide callback: upgrade the character's building */

int ai_Unk498b80(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIARG s1;
    AIARG s2;
    int item, obj;
    AIBLD *b;
    int r;

    if (((unsigned char *)p)[2] != 3 && (f != 0 || o1->kind == 0)) {
        b = *(AIBLD **)((char *)p + 0x170);
        if (b != 0 && o1->kind == 0 && ai_Unk4a5540(&item, &obj, b, 0) != 0) {
            s1.kind = 2;
            s1.unk04 = item;
            s1.unk08 = 1;
            if (obj != -1) {
                s2.kind = 1;
                s2.unk04 = obj;
            } else {
                s2.kind = 4;
                s2.unk04 = *(int *)((char *)b + 1);
            }
            s2.unk08 = 1;
            s2.unk14 = 0.44f;
            r = ai_Unk495a50(0x15, p->id, 1, &s1, &s2);
            if (r != 0) {
                *o1 = s1;
                *o2 = s2;
                return r;
            }
        }
    }
    return 0;
}
