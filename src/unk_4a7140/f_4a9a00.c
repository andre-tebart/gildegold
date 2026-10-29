/* 0x4a9a00, decide callback of the leisure actions: picks one of the actions of the table 0x600e10 that p (ctx 1)
   or p with its partner (ctx 2, from the job list 0x45) may do, unless in already names one (kind 0x10), and asks
   that action's callback */

/* the callbacks above as 0x4a9a00 calls them */
#define G10_CB(fn) ((int (*)(int, AIPER *, int *, unsigned char *, unsigned char *))(fn))

int ai_Unk4a9a00(int a, AIPER *p, int c, unsigned char *in, unsigned char *m)
{
    int ctx[0x88];
    int l1[19], l2[19];
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
        for (t = ai_Unk600e10; t < &ai_Unk600e10[19]; t++) {
            if ((ord_DispatchOrder(*t, p, ctx) & 2) != 0)
                l1[n1++] = *t;
        }
        if (ctx[0x87] != 0) {
            ctx[0] = 2;
            n2 = 0;
            for (t = ai_Unk600e10; t < &ai_Unk600e10[19]; t++) {
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
    case 0x12:
        return G10_CB(ai_Unk4a8760)(a, p, ctx, in, m);
    case 0x13:
    case 0x14:
        return G10_CB(ai_Unk4a8a50)(a, p, ctx, in, m);
    default:
        return 0;
    case 0x1c:
        return G10_CB(ai_Unk4a8ad0)(a, p, ctx, in, m);
    case 0x1d:
        return G10_CB(ai_Unk4a8bf0)(a, p, ctx, in, m);
    case 0x21:
        return G10_CB(ai_AlwaysTrue)(a, p, ctx, in, m);
    case 0x24:
        return G10_CB(ai_Unk4a8c20)(a, p, ctx, in, m);
    case 0x25:
        return G10_CB(ai_Unk4a8c80)(a, p, ctx, in, m);
    case 0x27:
        return G10_CB(ai_Unk4a8d20)(a, p, ctx, in, m);
    case 0x28:
        return G10_CB(ai_Unk4a8db0)(a, p, ctx, in, m);
    case 0x29:
        return G10_CB(ai_Unk4a8ee0)(a, p, ctx, in, m);
    case 0x2a:
        return G10_CB(ai_Unk4a8f20)(a, p, ctx, in, m);
    case 0x2b:
        return G10_CB(ai_Unk4a9030)(a, p, ctx, in, m);
    case 0x2c:
        return G10_CB(ai_Unk4a9080)(a, p, ctx, in, m);
    case 0x2d:
        return G10_CB(ai_Unk4a9150)(a, p, ctx, in, m);
    case 0x31:
        return G10_CB(ai_Unk4a95d0)(a, p, ctx, in, m);
    case 0x32:
        return G10_CB(ai_Unk4a9660)(a, p, ctx, in, m);
    case 0x35:
        return G10_CB(ai_Unk4a9700)(a, p, ctx, in, m);
    case 0x37:
        return G10_CB(ai_Unk4a9880)(a, p, ctx, in, m);
    }
}
