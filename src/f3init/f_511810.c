/* 0x511810, scene object callback: an object of the type 0 / 1 / 3 / 4 (+0x220) gets the flag 4 in +0x213 */


int ai_Unk511810(char *o)
{
    int t;

    t = *(int *)(o + 0x220);
    if (t == 3 || t == 4 || t == 1 || t == 0)
        o[0x213] |= 4;
    return t;
}
