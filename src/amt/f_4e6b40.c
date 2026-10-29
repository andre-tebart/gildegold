/* 0x4e6b40, the office of the kind 0x26 (the historical event's office) gets the owner id */

void amt_SetEventOwner(int id)
{
    int i;

    i = 41;
    do {
        if (amt_Table[i].kind == 0x26) {
            amt_Table[i].owner = id;
            return;
        }
        i--;
    } while (i != 0);
}
