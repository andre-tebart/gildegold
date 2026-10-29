/* 0x5049c0, 1 when the player p has a building (0x50b010 (p) is not 0) */


int gm_HasBuilding(int p)
{
    return gm_FirstBuilding(p) != 0;
}
