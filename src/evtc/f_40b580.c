/* 0x40b580, random number 0 .. *n - 1 */
int evtc_Random(int *n)
{
    if (!*n)
        return 0;
    return rand() % *n;
}
