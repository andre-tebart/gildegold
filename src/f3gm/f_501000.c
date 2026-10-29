/* 0x501000, the building of the type 0x47 (group (1, 0, 0x47)) whose int at +0x65 is the player p, 0 when none */

int gm_FindPlayerBuilding(int p)
{
    int b;

    b = gm_BuildQuery(1, 0, 0x47);
    while (b != 0) {
        if (*(int *)(b + 0x65) == p)
            return b;
        b = gm_BuildNext();
    }
    return 0;
}
