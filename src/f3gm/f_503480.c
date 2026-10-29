/* 0x503480, removes the object instance with the id from the list of the building b (0x4ffff0): 0 when it was there, -3 otherwise */


int gm_RemoveObjectFromBuilding(int b, int id)
{
    int r;

    r = gm_RemoveObject((int *)(b + 0x5d), id);
    return (r != -2) ? 0 : -3;
}
