/* 0x4e9400, 1 when the town object of the title group k (1 .. 3, 5: the buildings 0x18, 0x19, 0x1a, 0x17) exists and is not flagged
   (+0x5a bit 0); with the net flag 0x80 set it recalculates the four town offices instead and returns 0 (the income collector 0x4fa4b0) */

int amt_HasTownObject(int k)
{
    int v;
    AMTBLDVIEW *it;

    if (he_FindEvent(1, 0, 0x80) != 0) {
        amt_CalcZuenfte(0x1e);
        amt_CalcZuenfte(0x1f);
        amt_CalcZuenfte(0x20);
        amt_CalcZuenfte(0x21);
        return 0;
    }
    switch (k) {
    case 1: v = 0x18; break;
    case 2: v = 0x19; break;
    case 3: v = 0x1a; break;
    default:
        return 0;
    case 5: v = 0x17;
    }
    it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, v);
    if (it != 0) {
        if ((it->flags5a & 1) == 0)
            return 1;
    }
    return 0;
}
