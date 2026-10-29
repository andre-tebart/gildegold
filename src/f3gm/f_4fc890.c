/* 0x4fc890, the same without the int +0x24 and the floor */

float unk_Unk4fc890(short *c)
{
    float a;

    if (*c == -1)
        return -1.0f;
    if (((char *)c)[8] == 0)
        return -1.0f;
    a = (float)*(unsigned short *)((char *)c + 10);
    if (a <= *(float *)((char *)c + 0x20))
        return ((*(float *)((char *)c + 0x1c) - *(float *)((char *)c + 0x10)) / *(float *)((char *)c + 0x20)) * a + *(float *)((char *)c + 0x10);
    return *(float *)((char *)c + 0x1c);
}
