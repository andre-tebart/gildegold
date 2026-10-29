/* 0x476b40, frees all fades */

void d2_FreeFades(void)
{
    int i;

    for (i = 0; i < 0x20; i++)
        if (d2_Fades[i] != 0) {
            d2_ReleaseFade(d2_Fades[i]);
            d2_Fades[i] = 0;
        }
}
