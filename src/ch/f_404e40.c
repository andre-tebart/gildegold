/* 0x404e40 */

int ch_Random(unsigned short n)
{
    int r = 0;

    if (n)
        r = rand() % n;
    return r;
}
