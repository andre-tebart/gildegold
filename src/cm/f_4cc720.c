/* 0x4cc720, sets the position (a) and the direction (b) of the model of the character record c and its id x (+0x24);
   a vector that is not within 8.0 of the current one replaces it */

void ai_Unk4cc720(char *c, float *a, float *b, int x)
{
    char *o;

    if (c != 0 && *(char **)(c + 0x184) != 0) {
        o = *(char **)(*(char **)(c + 0x184) + 0x34);
        if (d3_VecEqual((float *)(o + 0x4c), a, 8.0f) == 0)
            d3_SetObjectPos(o, a);
        o = *(char **)(*(char **)(c + 0x184) + 0x34);
        if (d3_VecEqual((float *)(o + 0x84), b, 8.0f) == 0)
            d3_SetObjectRot(o, b);
        *(int *)(c + 0x24) = x;
    }
}
