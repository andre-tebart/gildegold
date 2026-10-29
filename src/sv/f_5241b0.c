/* 0x5241b0, the number of used member slots (+0x8c, 8 of them) of the group record g */

int sv_UsedGroupSlots(char *g)
{
    int n, i;
    int *p;

    n = 0;
    p = (int *)(g + 0x8c);
    i = 8;
    do {
        if (*p != -1)
            n++;
        p++;
        i--;
    } while (i != 0);
    return n;
}
