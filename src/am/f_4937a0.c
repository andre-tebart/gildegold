/* 0x4937a0, side c acts on the duel with k (1 and 2 roll dice, 3 and 4 only change the kind) */

int ai_Unk4937a0(AGDUEL *d, AGCH *c, int k)
{
    if (ai_Unk493390(d, c, k) == 0)
        return 0;
    switch (k) {
    case 1:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk00);
        d->total += d->unk00;
        ai_Unk493420(d, c, 1);
        ai_Unk4933d0(d, c, k);
        return 1;
    case 2:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk04);
        d->total += d->unk04;
        ai_Unk493420(d, c, 0);
        ai_Unk4933d0(d, c, k);
        return 1;
    case 3:
        if (c->kind == 6)
            cm_SendMoveGoodsUnchecked(-1, c->netId, 0, d->unk04);
        d->total += d->unk04;
        ai_Unk4933d0(d, c, k);
        return 1;
    case 4:
        ai_Unk4933d0(d, c, k);
        return 1;
    default:
        return 0;
    }
}
