/* 0x4f36b0, the last free (int +0x25 = 0) record of the 512 at 0x91f700 (stops at the first one found), -1 when none */

int unk_Unk4f36b0(void)
{
    int r, i, j;

    r = -1;
    i = 0;
    do {
        j = i;
        if (*(int *)(gv_Graveyard + 0x25 + i * 0x2d) == 0) {
            j = 0x200;
            r = i;
        }
        i = j + 1;
    } while (i < 0x200);
    return r;
}
