/* 0x4e6b70, the same for the kind 0x27 (the person's office) */

void amt_SetPersonOwner(int id)
{
    int i;

    i = 41;
    do {
        if (amt_Table[i].kind == 0x27) {
            amt_Table[i].owner = id;
            return;
        }
        i--;
    } while (i != 0);
}
