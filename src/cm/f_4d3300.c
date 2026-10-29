/* 0x4d3300, a character falls: the sound "Koerperfaellt" and a blood puddle "ub_Blutlache" (random turn) at its place */

void ai_Unk4d3300(char *c)
{
    float rot[3];
    char *b;

    rot[0] = 0;
    rot[1] = 0;
    rot[2] = 0;
    rot[1] = (float)(ai_Unk4e52f0(0x168) * ai_Unk5eb7dc * ai_Unk5edc68);
    sb_PlaySample(0, "Koerperfaellt", 1, 0x3f, 0x7f, 0, 0);
    b = (char *)d3_CreateObject(0, *(char **)(*(char **)(c + 0x14) + 0x34) + 0x4c, rot, "ub_Blutlache");
    d3t_PlayAnim(b, "sonstiges\\ub_Blutlache.baf", 0x12);
}
