/* 0x422620 */

void mem_Free(void *p)
{
    int i, size;
    MEMENTRY *e;

    if (p) {
        mem_Count--;
        for (i = 0, e = mem_Table; i < mem_Max; e++, i++)
            if (e->ptr == p)
                break;
        if (i != mem_Max) {
            size = e->size;
            e->ptr = NULL;
            free(p);
            mem_Blocks--;
            e->group->blocks--;
            mem_Bytes -= size;
            e->group->bytes -= size;
        }
    }
}
