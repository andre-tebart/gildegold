/* 0x4bda60, the index of q in list p (the length if not in it) */

int rt_PointIndex(RTPOINT *p, RTPOINT *q)
{
    int i;

    i = 0;
    for (; p != 0 && q != p; p = p->next)
        i++;
    return i;
}
