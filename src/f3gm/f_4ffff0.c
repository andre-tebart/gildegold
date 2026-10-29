/* 0x4ffff0, removes the object instance with the prototype id from the singly linked list whose head pointer is *list (the next pointer sits at +0x3f of each node): frees its extra part (0x500140 of the pointer at +0x14), the node itself (0x500080), relinks, marks the record free and decrements the count 0x6f1bdc. Returns 0, -1 for an empty list, -2 when not found */

int gm_RemoveObject(int *list, unsigned id)
{
    unsigned short *prev, *cur;
    int n;

    if (list == 0)
        return -1;
    if ((unsigned short *)*list == 0)
        return -1;
    n = 0;
    prev = 0;
    cur = (unsigned short *)*list;
    do {
        if (*cur == id) {
            if (*(int *)(cur + 10) != 0)
                gm_FreeList(cur + 10);
            gm_FreeObjectExtra(cur);
            if (n == 0)
                *list = *(int *)((char *)cur + 0x3f);
            else
                *(int *)((char *)prev + 0x3f) = *(int *)((char *)cur + 0x3f);
            *cur = 0;
            f3gm_ObjectCount--;
            return 0;
        }
        n++;
        prev = cur;
        cur = *(unsigned short **)((char *)cur + 0x3f);
    } while (cur != 0);
    return -2;
}
