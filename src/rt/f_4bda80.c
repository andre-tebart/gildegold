/* 0x4bda80, point n of list p */

RTPOINT *rt_PointAt(RTPOINT *p, int n)
{
    int i;

    for (i = 0; p != 0 && n != i; i++)
        p = p->next;
    return p;
}
