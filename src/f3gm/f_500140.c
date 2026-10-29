/* 0x500140, frees a whole list of object instances (and, recursively, the lists they hold at +0x14) */

int gm_FreeList(int *list)
{
    int o;

    if (list != 0) {
        o = *list;
        if (o == 0)
            return -1;
        do {
            *list = *(int *)(o + 0x3f);
            if (*(int *)(o + 0x14) != 0)
                gm_FreeList((int *)(o + 0x14));
            gm_FreeObjectExtra((unsigned short *)o);
            o = *list;
        } while (o != 0);
    }
    return 0;
}
