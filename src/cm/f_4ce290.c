/* 0x4ce290, a bomb object "ob_BOMBE" at the position p in a free slot of the 32 (object, time) of cm_Bombs */

char *ai_Unk4ce290(char *p)
{
    float v[3];
    int i;
    char *o;

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    for (i = 0; i < 32; i++) {
        if (*(int *)(cm_BombsV + i * 8) == 0)
            break;
    }
    if (i >= 32)
        return 0;
    o = d3_CreateObject(0, p, v, "ob_BOMBE");
    *(char **)(cm_BombsV + i * 8) = o;
    *(int *)(cm_BombsV + i * 8 + 4) = d3_Time;
    return cm_BombsV + i * 8;
}
