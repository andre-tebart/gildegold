/* 0x5642e0, picks the target of a fire (ROUGH): the first building of the list (0x501190 (1, 1, ..), checks 0x502520 / 0x5024f0, kind 3 buildings are skipped) near the scene object of the reference building (0x458a20 positions, 0x453e80 against the range), at random (0x42ec00) with up to 0x10 tries; returns the building or 0 */

int he_Unk5642e0(int ref)
{
    int b, n;

    for (n = 0; n < 0x10; n++) {
        b = gm_BuildQuery(1, 1, (unsigned short)util_Random(0x100));
        if (b == 0)
            continue;
        if (gm_IsHomeType(b) != 0 || gm_IsType10(b) != 0)
            continue;
        if (*(unsigned char *)b == 3)
            continue;
        if (b != ref)
            return b;
    }
    return 0;
}
