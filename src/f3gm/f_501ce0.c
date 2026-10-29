/* 0x501ce0, queries the main object of the building b for the side effect (the object 0xfd of a home, the 0x54 of a type 4 building, else walks the kind 2 objects while they are 0xfd) */

void gm_GetGebaeudeHauptraum(unsigned char *b)
{
    char *k;
    short *s;

    gm_BuildingClass(*b);
    k = ai_BTypes + *b * 0x24d;
    if (gm_IsHomeType(b) != 0) {
        gm_Query(*(int *)(b + 0x5d), 1, 0, 0xfd);
        return;
    }
    if (*k != 4) {
        s = (short *)gm_Query(*(int *)(b + 0x5d), 1, 4, 2);
        while (s != 0 && *s == 0xfd)
            s = (short *)gm_QueryNext();
        return;
    }
    gm_Query(*(int *)(b + 0x5d), 1, 0, 0x54);
}
