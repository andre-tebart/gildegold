/* 0x476b90, adds amount to the item list drawn next to the pointer: the entry of picture id (first match, else
   the first free one); no effect when amount is 0 or the list is full (6 entries) */
void d2_AddPointerItem(int id, int amount)
{
    int i;

    if (amount != 0) {
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
        d2_PointerItems[i].amount += amount;
    }
}
