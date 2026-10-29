/* 0x577290, 1 while the flag S+0x38 is not positive, and for the kind 0x29 with the step word 0x155 (which sets
   the flag to 1); else 0 */

int ai_Unk577290(int *p)
{
    if (U_I(S + 0x38) <= 0)
        return 1;
    if (p[0] == 0x29 && p[3] == 0x155) {
        U_I(S + 0x38) = 1;
        return 1;
    }
    return 0;
}
