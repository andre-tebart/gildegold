/* 0x4e95a0, the third step of the same power (a type level of 3+, bit 0x10000000, money 8+); see amt_CanUseMayorPower2 */

int amt_CanUseMayorPower3(char *c)
{
    AMTCHARVIEW *cv;
    int v;
    AMTBLDVIEW *it;

    cv = (AMTCHARVIEW *)c;
    if (cv->secondOffice >= 0x1e && cv->secondOffice <= 0x21) {
        switch (cv->secondOffice) {
        case 0x1f: v = 0x18; break;
        case 0x20: v = 0x19; break;
        case 0x21: v = 0x1a; break;
        case 0x1e: v = 0x17; break;
        default:
            return 0;
        }
        it = (AMTBLDVIEW *)gm_BuildQuery(1, 5, v);
        if (it != 0 && (it->flags5a & 1) == 0 && (unsigned char)ai_BTypes[it->type * 0x24d + 0x247] >= 3) {
            if ((cv->powerFlags & 0x10000000) != 0)
                return -1;
            return (((cv->money >= 8) - 1) & -3) + 1;
        }
    }
    return 0;
}
