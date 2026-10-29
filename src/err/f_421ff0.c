/* 0x421ff0, x86 call at the end of the n-byte window p (code bytes): 1 = a direct near call E8, its
   32-bit relative target plus base stored to *target; 2 = an indirect FF /2 call (register or memory,
   including the FF 15 disp32 IAT form); 0 = no call */

char err_CheckCall(char *p, int n, int base, int *target)
{
    int *rel;

    *target = 0;
    if (n >= 5 && p[-5] == (char)0xe8) {
        rel = (int *)(p - 4);
        *target = *rel + base;
        return 1;
    }
    if (n >= 3 && p[-3] == -1 && p[-2] == 0x14)
        return 2;
    if (n >= 6 && p[-6] == -1 && p[-5] == 0x15)
        return 2;
    if (n >= 2 && p[-2] == -1 && (unsigned char)p[-1] < 0x40)
        return 2;
    if (n >= 2 && p[-2] == -1 && (p[-1] & 0xf8) == 0xd0)
        return 2;
    if (n >= 3 && p[-3] == -1 && (p[-2] & 0xf8) == 0x50)
        return 2;
    if (n >= 7 && p[-7] == -1 && (p[-6] & 0xf8) == 0x90)
        return 2;
    return 0;
}
