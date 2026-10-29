/* 0x459f90, writes v into the first dword of n consecutive 0x1c byte entries */

void d3s_MarkSpan(int v, int n, int *p)
{
    for (; n > 0; n--) {
        *p = v;
        p += 7;
    }
}
