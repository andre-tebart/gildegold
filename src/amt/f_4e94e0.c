/* 0x4e94e0, whether the town mayor c (secondOffice 0x1e .. 0x21) may use the second step of the town's guild-house power: the town's
   building (0x17 .. 0x1a by the office) must exist, be unflagged and of a type whose level (+0x247) is 2 or more, and c must have 5+
   money; returns 1 when it may, -1 when the power is used already (bit 0x8000000 of +0x1c8), 0 when the building does not qualify
   (the AI decide callback 0x4a0090, the office window 0x5a0f10 / 0x5a1000). No side effects */

int amt_CanUseMayorPower2(char *c)
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
        if (it != 0 && (it->flags5a & 1) == 0 && (unsigned char)ai_BTypes[it->type * 0x24d + 0x247] >= 2) {
            if ((cv->powerFlags & 0x8000000) != 0)
                return -1;
            return (((cv->money >= 5) - 1) & -3) + 1;
        }
    }
    return 0;
}
