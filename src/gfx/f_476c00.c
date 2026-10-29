/* 0x476c00, sets the entry of picture id in the item list drawn next to the pointer (first match, else the
   first free one); amount 0 frees the entry, a full list is no effect */

void d2_SetPointerItem(int id, int amount)
{
    int i;

    for (i = 0; i < 6; i++)
        if (d2_PointerItems[i].id == id)
            break;
    if (i == 6) {
        for (i = 0; i < 6; i++)
            if (d2_PointerItems[i].id == -1)
                break;
        if (i == 6)
            return;
    }
    d2_PointerItems[i].id = id;
    d2_PointerItems[i].amount = amount;
    if (amount == 0) {
        d2_PointerItems[i].amount = 0;
        d2_PointerItems[i].id = -1;
    }
}
