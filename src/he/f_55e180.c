/* 0x55e180, a Ghidra miss: the allocation handler of the event 0x33: the player 0x505df0 (+0xac) must exist and his kind byte (+2) must not be 0x14 (else free), the run clock is the start clock */

void he_Unk55e180(int ev)
{
    int p;

    p = gm_FindChar(*(int *)(ev + 0xac));
    if (p != 0 && *(char *)(p + 2) != 0x14) {
        *(HETIME *)(ev + 0x52) = *(HETIME *)(ev + 0x44);
        return;
    }
    he_Unk53e1f0((int *)ev);
}
