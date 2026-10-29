/* 0x476c90, number of entries in use (id != -1) of the item list drawn next to the pointer */

int d2_CountPointerItems(void)
{
    int i, n = 0;

    for (i = 0; i < 6; i++)
        if (d2_PointerItems[i].id != -1)
            n++;
    return n;
}
