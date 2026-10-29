/* 0x468860, first free entry of the object list, -1 if it is full */

int d2_FindFreeObj(void)
{
    int i;

    for (i = 0; i < 0x2ff; i++)
        if (!d2_Objs[i].used)
            return i;
    return -1;
}
