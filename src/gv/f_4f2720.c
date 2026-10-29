/* 0x4f2720, copies the record i (0 .. 0x1c, 0x24 bytes at 0x611258) of the table to out; 0 for i > 0x1c */

int gv_GetTableRecord(int i, int *out)
{
    int *s;
    int k;

    if (i > 0x1c)
        return 0;
    s = (int *)(unk_Tab611258 + i * 0x24);
    for (k = 9; k != 0; k--)
        *out++ = *s++;
    return 1;
}
