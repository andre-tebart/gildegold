/* 0x4bdaa0, the length of list p */

int rt_PointCount(RTPOINT *p)
{
    int i;

    i = 0;
    for (; p != 0; p = p->next)
        i++;
    return i;
}
