/* 0x4bda00, a copy of list p */

RTPOINT *rt_CopyPoints(RTPOINT *p)
{
    RTPOINT *n;
    RTPOINT *first;

    n = 0;
    first = 0;
    for (; p != 0; p = p->next) {
        n = rt_AddPoint(n);
        if (first == 0)
            first = n;
        memcpy(n->loadSlots, p->loadSlots, sizeof(n->loadSlots));
        n->buildingId = p->buildingId;
        memcpy(n->unloadSlots, p->unloadSlots, sizeof(n->unloadSlots));
        n->cartId = p->cartId;
    }
    return first;
}
