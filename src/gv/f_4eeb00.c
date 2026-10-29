/* 0x4eeb00, sets the flags of the slot of the id to 4 (to be released) and releases it (id -1, flags 0) when none of its 3 parts is in use; 1 when the slot exists */

int unk_Unk4eeb00(int id)
{
    int *p;
    int i, j;

    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        do {
            if (*p == id) {
                j = 0;
                *(int *)(gv_Effects + 4 + i * 0x170) = 4;
                p = (int *)(gv_Effects + 0x74 + i * 0x170);
                do {
                    if (*p != 0)
                        return 1;
                    j++;
                    p += 0x1a;
                } while (j < 3);
                *(int *)(gv_Effects + i * 0x170) = -1;
                *(int *)(gv_Effects + 4 + i * 0x170) = 0;
                return 1;
            }
            p += 0x5c;
            i++;
        } while ((int)p < (unsigned)((unsigned)gv_Effects + 0xb80));
    }
    return 0;
}
