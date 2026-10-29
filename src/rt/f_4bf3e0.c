/* 0x4bf3e0, the mark of point p, -1 if none */

int rt_FindMark(RTPOINT *p)
{
    int i;

    for (i = 0; i < 8; i++) {
        if (rt_Marks[i].obj != -1 && rt_Marks[i].p == p)
            return i;
    }
    return -1;
}
