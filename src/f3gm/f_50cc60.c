/* 0x50cc60, sets the amount of every object in the first object (0xff) of the buildings of the type 0x1e to 2 */

void gm_ResetStock(void)
{
    unsigned char *b;
    char *l, *o;

    b = gm_BuildQuery(1, 0, 0x1e);
    while (b != 0) {
        l = gm_Query(*(void **)(b + 0x5d), 1, 0, 0xff);
        if (l != 0) {
            o = gm_Query(*(void **)(l + 0x14), 1, 5);
            while (o != 0) {
                if (*(int *)(o + 0xe) != 2)
                    *(int *)(o + 0xe) = 2;
                o = (char *)gm_QueryNext();
            }
        }
        b = gm_BuildNext();
    }
}
