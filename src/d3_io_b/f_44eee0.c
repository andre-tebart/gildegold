/* 0x44eee0, index of an id in a table (0x27 when missing); 0x28 ends the table */

int bgi_Find(int id, BGIENT *t, int n)
{
    int i;

    for (i = 0; i < 0x40; i++, t++) {
        if (id == t->id)
            return i;
        if (t->id == 0x28)
            break;
    }
    return 0x27;
}
