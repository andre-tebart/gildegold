/* 0x4eded0, the effect slot (gv_Effects) whose id (+0) is id, 0 when none; -1 never matches */

char *gv_FindEffect(int id)
{
    int *p;
    int i;

    if (id != -1) {
        i = 0;
        p = (int *)gv_Effects;
        do {
            if (*p == id)
                return gv_Effects + i * 0x170;
            p += 0x5c;
            i++;
        } while ((int)p < (int)(gv_Effects + 8 * 0x170));
    }
    return 0;
}
