/* 0x42ec00 */

int util_Random(unsigned short n)
{
    if (n)
        return rand() % n;
    return 0;
}
