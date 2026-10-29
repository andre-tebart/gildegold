/* 0x476c70, empties the item list drawn next to the pointer (all 6 entries free) */

void d2_ClearPointerItems(void)
{
    int i;

    for (i = 0; i < 6; i++) {
        d2_PointerItems[i].amount = 0;
        d2_PointerItems[i].id = -1;
    }
}
