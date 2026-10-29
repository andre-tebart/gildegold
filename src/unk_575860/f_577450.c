/* 0x577450, a step predicate: 1 for the kind 0x26 with the step word 0x1d0 while the flag S+0x38 is not negative
   (which sets the flag to 1), and for the kinds 0x1a / 0x1c; else 0 */

int ai_Unk577450(int *p)
{
    if (U_I(S + 0x38) >= 0 && p[0] == 0x26 && p[3] == 0x1d0) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    if (p[0] == 0x1a || p[0] == 0x1c)
        return 1;
    return 0;
}
