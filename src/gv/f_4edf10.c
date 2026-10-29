/* 0x4edf10, marks the slot of the id as to be released (flag 4 of +4) and releases it (id -1, flags 0) when none of its 3 parts (+0x74 + 0x68 * n, flag 2) is in use */

void unk_Unk4edf10(int id)
{
    int *p;
    int i, j;
    unsigned char *q;

    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        while (*p != id) {
            p += 0x5c;
            i++;
            if ((int)p > ((unsigned)&gv_Effects + 0xb7f))
                return;
        }
        q = (unsigned char *)gv_Effects + 0x74 + i * 0x170;
        *(int *)(gv_Effects + 4 + i * 0x170) |= 4;
        j = 0;
        do {
            if ((*q & 2) != 0)
                return;
            j++;
            q += 0x68;
        } while (j < 3);
        *(int *)(gv_Effects + i * 0x170) = -1;
        *(int *)(gv_Effects + 4 + i * 0x170) = 0;
    }
}
