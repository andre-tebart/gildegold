/* 0x46c9f0, takes a free object and puts it into the draw order list */

int d2_NewObject(void)
{
    int n, i;
    D2OBJ *o;

    n = d2_FindFreeObj();
    if (n == -1)
        return -1;
    o = &d2_Objs[n];
    for (i = 0x2ff; i >= 0; i--)
        if (d2_DrawOrder[i]) {
            i++;
            if (i >= 0)
                goto found;
            break;
        }
    i = 0;
found:
    d2_DrawOrder[i] = o;
    if (n > d2_HighestObj)
        d2_HighestObj = n;
    d2_Objs[n].used = 1;
    return n;
}
