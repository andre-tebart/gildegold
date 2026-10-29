/* 0x573fb0, the run of the event 0x9e: -2 / -1 free; the state + 1; the figures +0xb0 (a) and +0xac (b) (0x505df0); from a's
   +0x194 (n) a is charged -n and b n / 2 (at least 2) (0x50cc50); while the state stays below 2 the due clock +0x52 = now +
   24 hours (0x4fe140), else the event is freed */

void he_Unk573fb0(int ev)
{
    char *a, *b;
    int n, x;

    if (HM_I(ev, 0x70) == -1 || HM_I(ev, 0x70) == -2) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    HM_I(ev, 0x70) = HM_I(ev, 0x70) + 1;
    a = (char *)gm_FindChar(HM_I(ev, 0xb0));
    b = (char *)gm_FindChar(HM_I(ev, 0xac));
    if (a == 0) {
        he_Unk53e1f0((int *)ev);
        return;
    }
    n = HM_I(a, 0x194);
    if (n != 0) {
        gm_SendMoneyDelta(HM_I(a, 4), -n);
        x = n / 2;
        if (x <= 2)
            x = 2;
        if (b != 0)
            gm_SendMoneyDelta(HM_I(b, 4), x);
    }
    if (HM_I(ev, 0x70) < 2) {
        gm_AddTime(ev + 0x52, 0x18, 0, 0);
        return;
    }
    he_Unk53e1f0((int *)ev);
}
