/* 0x4fe980, the object instance with the id (int at +2) in the instance table, 0 when none */

int gm_FindObjectInst(int id)
{
    int i;
    short *p;

    i = 0;
    p = (short *)ai_ObjectsV;
    while (*p == 0 || *(int *)(p + 1) != id) {
        i++;
        p = (short *)((char *)p + 0x43);
        if (i >= 0x5008)
            return 0;
    }
    return (int)ai_ObjectsV + i * 0x43;
}
