/* 0x4e86e0, 1 when one of the offices of the column 7 (the records 36 .. 1, at most 7 free ones skipped) has an owner who exists (the special-amount
   menu 0x5c1130 enables its list button with it) */

int amt_HasColumn7Holder(void)
{
    int i, n;

    n = 7;
    i = 36;
    do {
        if (n == 0)
            return 0;
        if (amt_Kinds[amt_Table[i].kind].col == 7) {
            if (amt_Table[i].owner != -1) {
                if (gm_FindChar(amt_Table[i].owner) != 0)
                    return 1;
            } else
                n--;
        }
        i--;
    } while (i != 0);
    return 0;
}
