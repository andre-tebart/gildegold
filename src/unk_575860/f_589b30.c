/* 0x589b30, the 0x589aa0 counterpart over 0x51c920 (136 B): n = 0 read first, then each slot's byte +0, dword
   +4 and dword +8 from the table 0x6f5ec0; 0 on a failed read, 1 */

int ls_Read();

int ai_Unk589b30(int fh)
{
    int n;
    int i;
    char *p;

    n = 0;
    if (ls_Read(&n, 4, 1, fh) == 0)
        return 0;
    i = 0;
    p = (char *)(unsigned)&full_data_6f5ec4;
    while (i < n) {
        if (ls_Read(p - 4, 1, 1, fh) == 0)
            return 0;
        if (ls_Read(p, 4, 1, fh) == 0)
            return 0;
        if (ls_Read(p + 4, 4, 1, fh) == 0)
            return 0;
        i++;
        p += 0xc;
    }
    return 1;
}
