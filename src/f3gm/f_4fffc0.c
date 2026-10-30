/* 0x4fffc0, the first free record of the object instance table (0x5008 records of 0x43 bytes), 0 when it is full */

short *gm_FindFreeObject(void)
{
    int i;
    short *p;

    i = 0;
    p = (short *)ai_ObjectsV;
    while (i <= 0x5007) {
        if (*p == 0)
            return p;
        i++;
        p = (short *)((char *)p + 0x43);
    }
    return 0;
}
