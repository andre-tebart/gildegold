/* 0x493be0, a message that makes o's owner (character p) take a victim or let go */

int ai_Unk493be0(AGCH *p, AIOBJ *o)
{
    struct {
        int a;
        int id;
        int c[4];
    } rec[6];
    int info[6];
    AGCH *q, *w;
    int r, nn, cnt, i;
    unsigned char v;

    q = 0;
    r = util_RandomU(3);
    if (p == 0 || o == 0 || p->sel == 0 || p->unk194 < 2)
        return 0;
    if (p->sel == 7) {
        if (p->unk166 != 0) {
            float d;
            int x;

            if (amt_GetKindB(p->unk166, info) == 0)
                return 0;
            nn = amt_GetColumnOffices(info[1], 6, rec);
            if (nn == 0)
                return 0;
            cnt = nn;
            i = util_RandomU(nn);
            do {
                if (p->netId != rec[i].id && (w = AG_CH(rec[i].id)) != 0) {
                    d = gm_Liking(p->id, w->id, 1);
                    x = util_RandomU(30);
                    if (d < x + 40.0f) {
                        q = w;
                        break;
                    }
                }
                nn--;
                i = (i + 1) % cnt;
            } while (nn != 0);
        }
    } else if (p->sel == 10) {
        int flags;
        int x;

        flags = 0xc00;
        x = util_RandomU(30);
        if (ai_Unk4a5b50(p, 1, &flags, 0, x + 40.0f, (unsigned int *)&x) != 0)
            q = (AGCH *)&ai_CharsT[x & 0xffff];
    }
    if (q != 0) {
        v = 1;
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &q->netId);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = (unsigned char)util_RandomU(8);
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        v = (unsigned char)util_RandomU(8);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
        return 1;
    }
    if (p->sel != 3 && p->sel != 4 && r == 0)
        return 0;
    {
        int neg;

        v = 1;
        neg = -1;
        cm_BeginFieldBlock(o, o->objId);
        cm_AddFieldDelta(4, 1, 0x1e, &neg);
        cm_AddFieldDelta(1, 1, 0x22, &v);
        v = 0xff;
        cm_AddFieldDelta(1, 1, 0x1c, &v);
        cm_AddFieldDelta(1, 1, 0x1d, &v);
        cm_SendFieldDeltas();
    }
    return 1;
}
