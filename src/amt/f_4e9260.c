/* 0x4e9260, the office check run at the start of a round (main 0x53b2e0): every holder of a column-7 office (0x4e70f0: the deputies 0x1c / 0x1d,
   the town offices 0x1e .. 0x21, 0x22) whose character is gone, dead, holds another kind (+0x169) or is outside the kind's rank range
   (1 .. 6, 0x28 .. 0x2d, 0x22 .. 0x45, 0xd .. 0x12) loses the office with the kind-4 message; then, when the event 0x80 is not running,
   the four town offices are recalculated and the deputies and the 0x22 office are elected. No return value. */

void amt_CheckOffices(void)
{
    AMTREC list[7];
    AMTREC *p;
    int n, r, lo, hi;
    AMTCHARVIEW *c;

    n = amt_GetColumn7Offices(1, 7, list);
    for (p = list; n > 0; n--, p++) {
        if (p->owner != -1) {
            switch (p->kind) {
            case 0x1c: lo = 1; hi = 6; break;
            case 0x1d: lo = 0x28; hi = 0x2d; break;
            case 0x1e: lo = 0x22; hi = 0x27; break;
            case 0x1f: lo = 0x2e; hi = 0x33; break;
            case 0x20: lo = 0x3a; hi = 0x3f; break;
            case 0x21: lo = 0x40; hi = 0x45; break;
            case 0x22: lo = 0xd; hi = 0x12; break;
            default:
                continue;
            }
            c = (AMTCHARVIEW *)gm_FindChar(p->owner);
            if (c != 0 && c->health != 0 && (char)c->secondOffice == (char)p->kind &&
                c->rank >= lo && c->rank <= hi)
                continue;
            amt_SendOfficeLost((unsigned short *)c, p->kind);
            r = amt_SendOfficeMessage((char)p->idx, 0, 0, 4, 0xff);
            while (r != -1 && cm_GetSlotState(r) == 0)
                sv_PumpNet();
        }
    }
    if (he_FindEvent(1, 0, 0x80) == 0) {
        amt_CalcZuenfte(0x1e);
        amt_CalcZuenfte(0x1f);
        amt_CalcZuenfte(0x20);
        amt_CalcZuenfte(0x21);
    }
    amt_ElectDeputies();
    amt_ElectKind22();
}
