/* 0x46c600, frees a radio button group */

void d2_RadioClear(int g)
{
    memset(&d2_Radios[g], 0, sizeof(D2RADIO));
}
