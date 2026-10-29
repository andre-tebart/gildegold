/* 0x4a9fa0, check callback of the leisure actions: whether p may still do the action in (kind 0x10) with the
   targets of out (0x4f81c0 without flags 0x60) */

int ai_Unk4a9fa0(AIPER *p, unsigned char *in, unsigned char *out)
{
    int ctx[0x88];
    AIJOB *j;

    if (in[0] == 0x10) {
        memset(ctx, 0, sizeof(ctx));
        ctx[0x87] = 0;
        if ((p->flags1c8 & 0x100) != 0 && *(int *)(in + 8) == 4) {
            for (j = he_FindEvent(1, 0, 0x45); j != 0; j = he_NextEvent()) {
                if (j->unkac == p->netId) {
                    ctx[0x87] = (int)gm_FindChar(j->unkb0);
                    goto found;
                }
            }
            return 0;
        }
found:
        ctx[0] = *(int *)(in + 8);
        switch (*(int *)(in + 4)) {
        case 0x12:
        case 0x2c:
        case 0x31:
        case 0x32:
        case 0x35:
        case 0x37:
            ctx[0x85] = *(int *)(out + 4);
            break;
        case 0x13:
        case 0x14:
            ctx[1] = *(int *)(out + 4);
            ctx[2] = *(int *)(out + 8);
            break;
        default:
            return 0;
        case 0x1c:
            ctx[0x85] = *(int *)(out + 4);
            ctx[1] = *(int *)(out + 8);
            break;
        case 0x1d:
        case 0x21:
        case 0x24:
        case 0x29:
        case 0x2d:
            break;
        case 0x25:
        case 0x28:
        case 0x2a:
            ctx[0x85] = *(int *)(out + 4);
            ctx[0x86] = *(int *)(out + 8);
            break;
        case 0x27:
            ctx[0x85] = *(int *)(out + 4);
            break;
        case 0x2b:
            ctx[1] = *(int *)(in + 0x10);
            break;
        }
        return (ord_DispatchOrder(*(int *)(in + 4), p, ctx) & 0x60) == 0;
    }
    return 0;
}
