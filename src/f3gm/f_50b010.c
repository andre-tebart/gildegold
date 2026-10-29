/* 0x50b010, the first building of the class n (1 .. 5: the types 0xf, 0x17 .. 0x1a); 0 for 6 / 7 */


unsigned char *gm_FirstBuilding(int n)
{
    unsigned char *r;

    r = 0;
    switch (n) {
    case 1:
        r = gm_BuildQuery(1, 5, 0xf);
        return r;
    case 2:
        r = gm_BuildQuery(1, 5, 0x17);
        return r;
    case 3:
        r = gm_BuildQuery(1, 5, 0x18);
        return r;
    case 4:
        r = gm_BuildQuery(1, 5, 0x19);
        return r;
    case 5:
        r = gm_BuildQuery(1, 5, 0x1a);
        break;
    case 6:
    case 7:
        return 0;
    }
    return r;
}
