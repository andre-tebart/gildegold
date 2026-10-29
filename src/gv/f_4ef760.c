/* 0x4ef760, 1 when the +4 word of b is -1 */

int gv_EntryIsFree(int *a, int *b)
{
    return b[1] == -1;
}
