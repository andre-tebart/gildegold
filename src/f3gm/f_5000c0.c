/* 0x5000c0, removes the object instance with the owner id (+2) from the list *list: like 0x4ffff0 but by the id field */

int gm_RemoveOwned(int *list, int id)
{
    unsigned short *prev, *cur;
    int n;

    n = 0;
    prev = 0;
    if ((unsigned short *)*list == 0)
        return -1;
    cur = (unsigned short *)*list;
    do {
        if (*(int *)(cur + 1) == id) {
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
