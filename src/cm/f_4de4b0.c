/* 0x4de4b0, a callback of the fall animation: a blood puddle ("ub_Blutlache") at the position of the object o->+0x14 */

void ai_Unk4de4b0(float a, char *o)
{
    int v[3];

    v[0] = 0;
    v[1] = 0;
    v[2] = 0;
    a = (float)ai_Unk4e52f0(0x168) * ai_Unk5eb7dc * ai_Unk5edc68;
    d3t_PlayAnim(d3_CreateObject(0, *(char **)(*(char **)(o + 0x14) + 0x34) + 0x4c, v, "ub_Blutlache"), "sonstiges\\ub_Blutlache.baf", 0x12);
}
