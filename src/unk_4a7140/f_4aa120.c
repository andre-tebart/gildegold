/* 0x4aa120, decide callback of the visit actions: as 0x4a9a00 with the actions of the table 0x600e5c that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback */

int ai_Unk4aa120(int a, AIPER *p, int c, unsigned char *in, unsigned char *m)
{
    int ctx[0x88];
    int l1[3], l2[3];
    int n1, n2, k, kind;
    int *t;
    AIJOB *j;

    memset(ctx, 0, sizeof(ctx));
    n1 = 0;
    n2 = 0;
    ctx[0x87] = 0;
    if ((p->flags1c8 & 0x100) != 0) {
        for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
            if (j->unkac == p->netId) {
                ctx[0x87] = (int)gm_FindChar(j->unkb0);
                break;
            }
        }
    }
    if (in[0] != 0) {
        if (in[0] != 0x10)
            return 0;
        k = *(int *)(in + 4);
        ctx[0] = 1;
        if ((ord_DispatchOrder(k, p, ctx) & 2) == 0) {
            ctx[0] = 2;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
            *(int *)(in + 8) = 4;
        } else {
            *(int *)(in + 8) = 3;
        }
    } else {
        ctx[0] = 1;
        for (t = ai_Unk600e5c; t < &ai_Unk600e5c[3]; t++) {
            if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                l1[n1++] = *t;
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (t = ai_Unk600e5c; t < &ai_Unk600e5c[3]; t++) {
                if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                    l2[n2++] = *t;
            }
        }
        if (n1 == 0 && n2 == 0)
            return 0;
        if (n1 == 0 || (n2 != 0 && util_Random(2) != 0)) {
            ctx[0] = 2;
            k = l2[util_Random(n2)];
            kind = 4;
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        } else {
            ctx[0] = 1;
            kind = 3;
            k = l1[util_Random(n1)];
            if ((ord_DispatchOrder(k, p, ctx) & 2) == 0)
                return 0;
        }
        in[0] = 0x10;
        *(int *)(in + 4) = k;
        *(int *)(in + 8) = kind;
    }
    switch (k) {
    case 0:
        return G10_CB(ai_Unk4a91a0)(a, p, ctx, in, m);
    default:
        return 0;
    case 3:
        return G10_CB(ai_Unk4a9280)(a, p, ctx, in, m);
    case 4:
        return G10_CB(ai_Unk4a9350)(a, p, ctx, in, m);
    case 7:
        return G10_CB(ai_Unk4a9420)(a, p, ctx, in, m);
    case 8:
        return G10_CB(ai_Unk4a94e0)(a, p, ctx, in, m);
    }
}
