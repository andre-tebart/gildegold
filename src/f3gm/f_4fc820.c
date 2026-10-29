/* 0x4fc820, the health of the character c now (-1 for a free slot or an inactive one): the +0x1c float when the age (+10) is above the lifetime float (+0x20), else the line from +0x10 to +0x1c over the lifetime, plus the int +0x24, not below 0 */

float gm_CharacterHealth(short *c)
{
    float f;
    float a;

    if (*c == -1)
        return -1.0f;
    if (((char *)c)[8] == 0)
        return -1.0f;
    a = (float)*(unsigned short *)((char *)c + 10);
    if (a > *(float *)((char *)c + 0x20))
        f = *(float *)((char *)c + 0x1c);
    else
        f = ((*(float *)((char *)c + 0x1c) - *(float *)((char *)c + 0x10)) / *(float *)((char *)c + 0x20)) * a + *(float *)((char *)c + 0x10);
    f = (float)(f + *(int *)((char *)c + 0x24));
    if (f <= 0.0f)
        f = 0.0f;
    return f;
}
