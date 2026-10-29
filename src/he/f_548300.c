/* 0x548300, a Ghidra miss: counts +0xac (a byte) down in the state 0, frees the event at 0 or when it is freed (-2) */

void he_Unk548300(int ev)
{
    int st = *(int *)(ev + 0x70);

    if (st != -2) {
        if (st != 0)
            return;
        if (*(unsigned char *)(ev + 0xac) != 0) {
            *(unsigned char *)(ev + 0xac) = *(unsigned char *)(ev + 0xac) - 1;
            return;
        }
    }
    he_Unk53e1f0((int *)ev);
}
