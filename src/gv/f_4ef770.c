/* 0x4ef770, 1 when the +4 word of b is not -1 */

int gv_EntryIsUsed(int *a, int *b)
{
    return b[1] != -1;
}
