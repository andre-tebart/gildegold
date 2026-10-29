/* 0x4d2650, a callback: collects the objects "sp_CONQUER*" into ai_Unk930380[ai_Unk6f0d60++] */

char ai_Unk4d2650(char *o)
{
    if (strncmp(o, "sp_CONQUER", 10) == 0)
        ai_Unk930380[ai_Unk6f0d60++] = o;
    return 1;
}
