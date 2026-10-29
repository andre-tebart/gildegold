/* 0x49f850, decide callback 0x35, the regulars' table ("stammtisch"): up to four kind-0x37 buildings of other
   owners with a table object 0x12d; a member leaves ('leav') when it likes the others too little, else joins
   ('join') a table with a free seat; 'noop' (kind 0) otherwise */

int ai_Unk49f850(int a, G5CH *p, int f, AIARG *o1, AIARG *o2)
{
    AIBLD *bl[4];
    unsigned char *ob[4];
    int free[4];
    AIBLD *b;
    unsigned char *o;
    AICHAR *c;
    int found, step, i, j, n, k, m, v;
    float sum;

    found = -1;
    step = ai_Unk5ed140[util_Random(0x10)];
    if (*(int *)((unsigned char *)p + 0x1c8) & 0x2000000)
        return 0;
    if (a != 0)
        return 0;
    if (f != 0)
        return 0;
    n = 0;
    j = util_Random(0x100);
    for (i = 0x100; i != 0; ) {
        b = &ai_Blds[j];
        if (b->type == 0x37 && b->owner != 0xffff && b->owner != p->id) {
            o = (unsigned char *)gm_Query(b->objList, 2, 6, 0, 0x12d);
            if (o != 0) {
                bl[n] = b;
                ob[n] = o;
                n++;
            }
        }
        i--;
        j = (j + step) % 0x100;
        if (n >= 4)
            break;
    }
    if (n == 0) {
        if (util_RandomF() > 0.33f)
            goto noop;
        return 0;
    }
    for (k = 0; k < n; k++) {
        free[k] = 0;
        for (m = 0; m < 4; m++) {
            v = ((int *)(ob[k] + 0x1c))[m];
            free[k] |= v == -1;
            if (found == -1)
                found = v == (int)p->netId ? k : -1;
        }
    }
    if (found != -1) {
        k = 0;
        sum = 0.0f;
        for (m = 0; m < 4; m++) {
            c = gm_FindChar(((int *)(ob[found] + 0x1c))[m]);
            if (c != 0 && c != (AICHAR *)p) {
                k++;
                sum = gm_Liking(p->id, *(unsigned short *)c, 1) + sum;
            }
        }
        if (k == 0 && util_Random(4) == 0) {
            o1->kind = 4;
            o1->unk04 = bl[found]->id;
            o1->unk08 = 'leav';
            return 0x35;
        }
        sum = sum / (k * 100);
        if (sum <= 0.666f)
            return 0;
        if (util_RandomF() + 0.166667f > sum)
            goto noop;
        o1->kind = 4;
        o1->unk04 = bl[found]->id;
        o1->unk08 = 'leav';
        return 0x35;
    }
    j = util_Random(n);
    for (i = n; i != 0; i--) {
        if (free[j] == 1)
            break;
        j = (j + 1) % n;
    }
    if (i == 0 || j == -1)
        return 0;
    if (util_RandomF() > 0.75f)
        goto noop;
    o1->kind = 4;
    o1->unk04 = bl[j]->id;
    o1->unk08 = 'join';
    o2->kind = 0x12;
    o2->unk04 = ai_Chars[bl[j]->owner].netId;
    o2->unk08 = 0;
    return 0x35;
noop:
    o1->kind = 0;
    o1->unk04 = -1;
    o1->unk08 = 'noop';
    return 0x35;
}
