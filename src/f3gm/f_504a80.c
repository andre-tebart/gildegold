/* 0x504a80, the object 0x115 (player 1) or 0x142 (the others) of the building of the player p */

char *gm_PlayerSeat(int p)
{
    char *b;

    b = gm_FirstBuilding(p);
    if (p == 1)
        return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x115);
    return gm_Query(*(int *)(b + 0x5d), 2, 6, 0, 0x142);
}
