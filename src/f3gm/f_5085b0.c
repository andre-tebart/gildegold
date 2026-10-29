/* 0x5085b0, a / the divisor of the player slot k, rounded */

int gm_ConvertMoney(int a, unsigned char k)
{
    return (int)((double)a / PLTAB(PLW54(k)) + 0.5);
}
