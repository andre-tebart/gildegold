/* 0x4ce1b0, the distance of the character c's model to the object o (x / z) */

float ai_Unk4ce1b0(char *c, char *o)
{
    char *p = *(char **)(*(char **)(c + 0x184) + 0x34);
    float dx = *(float *)(o + 0x4c) - *(float *)(p + 0x4c);
    float dz = *(float *)(o + 0x54) - *(float *)(p + 0x54);

    return (float)sqrt(dx * dx + dz * dz);
}
