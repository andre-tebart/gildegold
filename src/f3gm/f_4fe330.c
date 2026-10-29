/* 0x4fe330, the season (word at +2 modulo 4) of a calendar record */

int gm_Season(char *d)
{
    return *(short *)(d + 2) % 4;
}
