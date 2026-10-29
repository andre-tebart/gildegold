/* 0x548380, a Ghidra miss: frees the event in the state 0 or -2 */

void he_Unk548380(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st == -2 || st == 0)
        he_Unk53e1f0((int *)ev);
}
