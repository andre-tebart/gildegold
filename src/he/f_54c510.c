/* 0x54c510, a Ghidra miss: the test of a figure record for the accident simulation: alive (word 0 != 0xffff, +8 != 0), kind 0, with a boss (+0x16c) whose id (+1) is the one of its job record (+0x184 + 0x2c): then 0x47e840 (rec), else 0 */

int ai_IsLocalWorker();
float gm_AddHealth(unsigned id, int d);

int he_Unk54c510(int rec)
{
    int ok = 1;

    if (*(unsigned short *)rec == 0xffff)
        ok = 0;
    if (*(unsigned char *)(rec + 8) == 0)
        ok = 0;
    if (*(unsigned char *)(rec + 2) == 0 && ok && *(int *)(rec + 0x16c) != 0 && *(int *)(rec + 0x184) != 0
        && *(int *)(*(int *)(rec + 0x184) + 0x2c) == *(int *)(*(int *)(rec + 0x16c) + 1))
        return ai_IsLocalWorker(rec);
    return 0;
}
