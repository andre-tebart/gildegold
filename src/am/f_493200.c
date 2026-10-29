/* 0x493200, starts a duel between p and a fresh opponent of kind k (1, 2 or 4) */

int ai_Unk493200(AGCH *p, AGDUEL *d, int k)
{
    AGFIN *fin;
    unsigned int stats;
    int m, r;
    double x;
    float old;

    fin = (AGFIN *)gm_PlayerRecord((AICHAR *)p);
    if (p == 0 || p->kind != 6 || d == 0 || (k != 1 && k != 2 && k != 4))
        return 0;
    d->unk00 = 0;
    stats = ai_Unk492fd0(k);
    d->total = 0;
    m = gm_ConvertMoney(gm_GetWorth(p->id), ai_OwnPlayer);
    x = m;
    if (10000.0 > x)
        x = 10000.0;
    r = gm_PlayerFactor((int)((x / (log10(x) + 1.0) * 0.10000000149011612) * (k * 0.25f)), ai_OwnPlayer);
    d->unk00 = r;
    d->unk04 = (int)(r * 0.1f);
    d->kind1 = 0;
    d->unk2c = 0;
    d->kind2 = 0;
    d->unk50 = 0;
    if (fin->unk58 < util_RandomF()) {
        d->side1 = p;
        d->unk14 = 0xffff;
        d->side2 = &ai_Unk6f0478;
        d->unk38 = stats;
        fin->unk58 = util_RandomF() * 0.11f + fin->unk58 + 0.2f;
    } else {
    d->side2 = p;
    d->unk38 = 0xffff;
    d->side1 = &ai_Unk6f0478;
    d->unk14 = stats;
    old = fin->unk58;
    fin->unk58 = (old - 0.2f) - util_RandomF() * 0.11f;
    }
    d->phase = 1;
    return 1;
}
