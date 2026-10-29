/* 0x4ef740, 1 when the +4 words of a and b are equal */

int gv_EntrySameId(int *a, int *b)
{
    return b[1] == a[1];
}
