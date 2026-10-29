/* 0x4225e0, 1 when p is NULL or a table-listed allocation, 0 for an untracked pointer */

int mem_IsValid(void *p)
{
    int i;
    MEMENTRY *e;

    if (p) {
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (e->ptr == p)
                break;
        return i != mem_Max;
    }
    return 1;
}
