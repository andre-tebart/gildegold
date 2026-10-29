/* 0x5085f0, a * the factor of the player slot k */

int gm_PlayerFactor(int a, unsigned char k)
{
    return PLTAB(PLW54(k)) * a;
}
